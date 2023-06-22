#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "CollisionsManager.h"

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
	virtual void init();
	virtual void update();
	virtual void draw();
	virtual ~Component();

	// Collision checking method
	virtual void onCollision2D(Collision2D collision);
};

class Entity {

private:

	bool active = true;
	bool GroupBitSet[maxGroups];

	bool ComponentBitSet[maxComponents];
	Component* ComponentArray[maxComponents];

public:

	Manager& manager;
	std::vector<Component*> components;

	Entity(Manager& mManager);

	void update();
	void draw();
	bool isActive();
	void destroy();

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

	bool hasGroup(int mGroup);
	void addGroup(int mGroup);
	void delGroup(int mGroup);

	~Entity();
};

class Manager {

private:

	std::vector<Entity*> entities;

	// a way to render all entitties in a specific order,
	// every entity has a group defined by a size_t value

	std::vector<Entity*> groupedEntities[maxGroups];

	void refreshInactiveEntities();
	void refreshGroupedEntities();

public:

	void update();
	void draw();
	void refersh();
	void addToGroup(Entity* mEntity, int mGroup);
	std::vector<Entity*>& getGroup(int mGroup);
	Entity& addEntity();
};