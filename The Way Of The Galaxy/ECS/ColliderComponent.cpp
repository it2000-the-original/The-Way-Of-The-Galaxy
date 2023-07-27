#include "CollisionsManager.h"
#include "ECS/Components.h"
#include "Engine.h"
#include "Point.h"

ColliderComponent::ColliderComponent(std::size_t mId) {

	id = mId;
}

ColliderComponent::ColliderComponent(std::size_t mId, Polygon mPolygon) {

	id = mId;
	advanced = true;
	srcPolygon = mPolygon;
	destPolygon = mPolygon;
}

void ColliderComponent::init() {

	if (!entity->hasComponent<PositionComponent>()) {

		entity->addComponent<PositionComponent>();
	}

	position = &entity->getComponent<PositionComponent>();

	if (!advanced) {

		destPolygon = srcPolygon = { {

			Point(0.0, 0.0),
			Point(float(position->width), 0.0),
			Point(float(position->width), float(position->height)),
			Point(0.0, float(position->height))
		} };
	}

	// Adds the initialized collider in the colliders static vector
	Engine::collisions.colliders.push_back(this);
}

void ColliderComponent::update() {

	// Updating the position of collider...
	collider.x = static_cast<int>(position->position.x);
	collider.y = static_cast<int>(position->position.y);
	collider.w = position->width * position->scale;
	collider.h = position->height * position->scale;

	// Updating the position of the polygons...
	for (int i = 0; i < signed(destPolygon.size()); i++)
	for (int j = 0; j < signed(destPolygon[i].size()); j++)

	destPolygon[i][j] = srcPolygon[i][j] + Point(
		float(position->position.x),
		float(position->position.y)
	);
}

void ColliderComponent::setPolygon(Polygon polygon) {

	srcPolygon = polygon;
	destPolygon = polygon;
	advanced = true;
}

bool ColliderComponent::isAdvanced() {
	
	return advanced;
}