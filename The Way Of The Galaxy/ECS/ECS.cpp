#include "ECS.h"

void Component::init() {}

void Component::update() {}

void Component::draw() {}

Component::~Component() {}

void Component::onCollision2D(Collision2D collision) {}


Entity::Entity(Manager& mManager) : manager(mManager) {}

void Entity::update() {

	for (auto& c : components) c->update();
}

void Entity::draw() {

	for (auto& c : components) c->draw();
}

bool Entity::isActive() { 
	
	return active;
}

void Entity::destroy() {
	
	active = false;
}

bool Entity::hasGroup(int mGroup) {

	return GroupBitSet[mGroup];
}

void Entity::addGroup(int mGroup) {

	GroupBitSet[mGroup] = true;
	manager.addToGroup(this, mGroup);
}

void Entity::delGroup(int mGroup) {

	GroupBitSet[mGroup] = false;
}

Entity::~Entity() {

	// Delete from the memory all components of components vector

	for (int i = int(components.size()) - 1; i >= 0; i--) {

		delete components[i];
		components.erase(components.begin() + i);
	}

	std::cout << "Removed entity" << std::endl;
}


void Manager::refreshInactiveEntities() {

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

void Manager::refreshGroupedEntities() {

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

void Manager::update() {

	for (int i = 0; i < entities.size(); i++) entities[i]->update();
}

void Manager::draw() {

	for (auto& e : entities) e->draw();
}

void Manager::refersh() {

	refreshGroupedEntities();
	refreshInactiveEntities();
}

void Manager::addToGroup(Entity* mEntity, int mGroup) {

	groupedEntities[mGroup].emplace_back(mEntity);
}

std::vector<Entity*>& Manager::getGroup(int mGroup) {

	return groupedEntities[mGroup];
}

Entity& Manager::addEntity() {

	Entity* e = new Entity(*this);
	entities.emplace_back(std::move(e));

	std::cout << "Added entity" << std::endl;

	return *e;
}