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

	if (position->angle == 0) {

		collider.x = static_cast<int>(position->position.x);
		collider.y = static_cast<int>(position->position.y);
		collider.w = position->width * position->scale;
		collider.h = position->height * position->scale;
	}

	else {

		collider.x = findMinXPoint(destPolygon).x;
		collider.y = findMinYPoint(destPolygon).y;
		collider.w = findMaxXPoint(destPolygon).x - collider.x;
		collider.h = findMaxYPoint(destPolygon).y - collider.y;
	}
}

void ColliderComponent::setPolygon(Polygon polygon) {

	srcPolygon = polygon;
	destPolygon = polygon;
	advanced = true;
}

bool ColliderComponent::isAdvanced() {
	
	return advanced;
}

Point ColliderComponent::findMinXPoint(Polygon points) {

	Point minPoint = points[0][0];

	for (auto convex : points) 
	for (auto point : convex) {

		if (point.x < minPoint.x) {

			minPoint = point;
		}
	}

	return minPoint;
}

Point ColliderComponent::findMinYPoint(Polygon points) {

	Point minPoint = points[0][0];

	for (auto convex : points) 
	for (auto point : convex) {

		if (point.y < minPoint.y) {

			minPoint = point;
		}
	}	

	return minPoint;
}

Point ColliderComponent::findMaxXPoint(Polygon points) {

	Point maxPoint = points[0][0];

	for (auto convex : points) 
	for (auto point : convex) {

		if (point.x > maxPoint.x) {

			maxPoint = point;
		}
	}	

	return maxPoint;
}

Point ColliderComponent::findMaxYPoint(Polygon points) {

	Point maxPoint = points[0][0];

	for (auto convex : points) 
	for (auto point : convex) {

		if (point.y > maxPoint.y) {

			maxPoint = point;
		}
	}

	return maxPoint;
}