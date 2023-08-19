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

	// Updating the position of the polygons...
	for (int i = 0; i < signed(destPolygon.size()); i++)
	for (int j = 0; j < signed(destPolygon[i].size()); j++) {

		destPolygon[i][j] = srcPolygon[i][j] + Point(
			float(position->position.x),
			float(position->position.y)
		);

		Point centerPoint = position->getCenterPoint();

		float distance = sqrt(
			pow(destPolygon[i][j].x - centerPoint.x, 2) + 
			pow(destPolygon[i][j].y - centerPoint.y, 2)
		);

		float angle = atan2(
			destPolygon[i][j].y - centerPoint.y,
			destPolygon[i][j].x - centerPoint.x
		);

		angle += position->angle * 3.1415 / 180;
		destPolygon[i][j].x = centerPoint.x + distance * cos(angle);
		destPolygon[i][j].y = centerPoint.y + distance * sin(angle);
	}

	// Updating the position of collider...

	collider = position->getVisualRectangle();
}

void ColliderComponent::setPolygon(Polygon polygon) {

	srcPolygon = polygon;
	destPolygon = polygon;
	advanced = true;
}

bool ColliderComponent::isAdvanced() {
	
	return advanced;
}