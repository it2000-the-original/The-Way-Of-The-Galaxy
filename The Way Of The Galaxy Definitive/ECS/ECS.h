#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

/* This is the header file that define all of the classes to manage for the
 * Entity Component System, in fact there is a Component class that acts as a polymorphism
 * to create other components, an entity class that define his components and a manager class
 * to manage all of the entities in the system. */

// declaration of the three classes
class Component;
class Entity;
class Manager;

const int maxComponents = 32;
const int maxGroups = 32;

// two methods to use to define the position to put new components

inline int getNewComponentID() {

	static int newID = 0;
	return newID++;
}

template <typename T> inline int getComponentID() noexcept {

	static int ID = getNewComponentID();
	return ID;
}

class Component {

public:

	Entity* entity;         
	virtual void init()   {}
	virtual void update() {}
	virtual void draw()   {}
	virtual ~Component()  {}
};

class Entity {

private:

	bool active = true;
	bool GroupBitSet[maxGroups];

	std::vector<Component*> components;
	bool ComponentBitSet[maxComponents];
	Component* ComponentArray[maxComponents];

public:

	Manager& manager;

	Entity(Manager& mManager) : manager(mManager) {}

	void update() {

		for (auto& c : components) c->update();
	}

	void draw() {

		for (auto& c : components) c->draw();
	}

	bool isActive() { return active; }
	void destroy() { active = false; }

	template <typename T> const bool hasComponent() {

		// Return a bool value from a position of ComponentBitSet
		// dictated by the function getComponentID.
		// If call this function with a new typename never gived
		// it will generate a new value, but if we give to it
		// a typename that we have already gived before,
		// it will return the same value of the first time.

		return ComponentBitSet[getComponentID<T>()];
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {

		// Creating and call the constructor of a new entity, give to it the same
		// parameters given at the call of the function and put the pointer in
		// the component std::vector.

		T* c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		components.emplace_back(std::move(c));

		ComponentArray[getComponentID<T>()] = c;
		ComponentBitSet[getComponentID<T>()] = true;

		c->init();

		std::cout << "Added component: " << typeid(c).name() << std::endl;

		return *c;
	}

	template <typename T> T& getComponent() const {

		// Create a variable that contain an element of
		// ComponentArray in a position returned by getComponentID

		auto ptr = ComponentArray[getComponentID<T>()];
		return *static_cast<T*>(ptr);
	}

	bool hasGroup(int mGroup) {

		return GroupBitSet[mGroup];
	}

	// you can find the definition of this function in ECS.cpp

	void addGroup(int mGroup);

	void delGroup(int mGroup) {

		GroupBitSet[mGroup] = false;
	}

	~Entity() {

		// Delete from the memory all components of components vector

		for (int i = components.size() - 1; i >= 0; i--) {

			delete components[i];
			components.erase(components.begin() + i);
		}

		std::cout << "Removed entity" << std::endl;
	}
};

class Manager {

private:

	std::vector<Entity*> entities;

	// a way to render all entitties in a specific order,
	// every entity has a group defined by a size_t value

	std::vector<Entity*> groupedEntities[maxGroups];

	void refreshInactiveEntities() {

		// Refresh and delete every inactive entity in the entities vector

		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](Entity* mEntity) {

			if (!mEntity->isActive()) {

				delete mEntity;
				return true;
			}

			else {

				return false;
			}

			}), std::end(entities));
	}

	void refreshGroupedEntities() {

		// Remove and destroy every entity from a group of entities that
		// is inactive or doesn't make part of the group where it is

		for (auto i = 0; i < maxGroups; i++) {

			auto& v = groupedEntities[i];

			v.erase(std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity) {

				if (!mEntity->hasGroup(i)) return true;
				else if (!mEntity->isActive())  return true;

				return false;

				}), std::end(v));
		}
	}

public:

	void update() {

		// I must use this method to update all entities because the number
		// of elements in the entities vector is not constant during this phase,
		// this is because in some components they could be methods tha during the
		// update can add or remove components, generating conflicts with the other form

		for (int i = 0; i < entities.size(); i++) entities[i]->update();
	}

	void draw() {

		for (auto& e : entities) e->draw();
	}

	void refersh() {

		refreshGroupedEntities();
		refreshInactiveEntities();
	}

	void addToGroup(Entity* mEntity, int mGroup) {

		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(int mGroup) {

		return groupedEntities[mGroup];
	}

	Entity& addEntity() {

		Entity* e = new Entity(*this);
		entities.emplace_back(std::move(e));
		
		std::cout << "Added entity" << std::endl;

		return *e;
	}
};