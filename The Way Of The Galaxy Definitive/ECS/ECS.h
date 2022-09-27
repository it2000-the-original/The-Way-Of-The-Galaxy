#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

class Component;
class Entity;
class Manager;

inline int getNewComponentID() {

	static int newID = 0;
	return newID++;
}

template <typename T> inline int getComponentID() noexcept {

	static int ID = getNewComponentID();
	return ID;
}

const int maxComponents = 32;
const int maxGroups = 32;

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
	std::vector<Component*> components;
	Component* ComponentArray[maxComponents];
	bool GroupBitSet[maxGroups];
	bool ComponentBitSet[maxComponents];

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

		return ComponentBitSet[getComponentID<T>()];
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {

		T* c = new T(std::forward<TArgs>(mArgs)...);
		c->entity = this;
		components.emplace_back(std::move(c));

		ComponentArray[getComponentID<T>()] = c;
		ComponentBitSet[getComponentID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const {

		auto ptr = ComponentArray[getComponentID<T>()];
		return *static_cast<T*>(ptr);
	}

	bool hasGroup(int mGroup) {

		return GroupBitSet[mGroup];
	}

	void addGroup(int mGroup);

	void delGroup(int mGroup) {

		GroupBitSet[mGroup] = false;
	}

	~Entity() {

		for (int i = components.size() - 1; i >= 0; i--) {

			delete components[i];
			components[i] = nullptr;
			components.erase(components.begin() + i);
		}

		for (auto& c : components) {

			c = nullptr;
		}

		std::cout << "Removed entity" << std::endl;
	}
};

class Manager {

private:

	std::vector<Entity*> entities;
	std::vector<Entity*> groupedEntities[maxGroups];

public:

	void update() {

		// I use this metod to update every entity instead of "auto& e : entities" because if i add an entity while
		// they are updating, it generates an exception with the vector class and the crash of the program but i 
		// need so mutch to add entityes during the uppdate. If you that are reading this have an idea to fix this, 
		// write an issue on Github.

		for (int i = 0; i < entities.size(); i++) entities[i]->update();
	}

	void draw() {

		for (auto& e : entities) e->draw();
	}

	void refersh() {
		
		for (auto i = 0; i < maxGroups; i++) {

			auto& v(groupedEntities[i]);
			v.erase(

				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity) {

						if (!mEntity->hasGroup(i)) {

							mEntity->destroy();
							return true;
						}

						else if (!mEntity->isActive()) {

							return true;
						}
						
						return false;
					}),

				std::end(v));
		}
		
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](Entity* mEntity) {

			if (!mEntity->isActive()) {

				delete mEntity;
				mEntity = nullptr;
				return true;
			}
			
			else {

				return false;
			}
		}),

		std::end(entities));
	}

	Entity& addEntity() {

		Entity* e = new Entity(*this);
		entities.emplace_back(std::move(e));
		return *e;
	}

	void addToGroup(Entity* mEntity, int mGroup) {

		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(int mGroup) {

		return groupedEntities[mGroup];
	}
};