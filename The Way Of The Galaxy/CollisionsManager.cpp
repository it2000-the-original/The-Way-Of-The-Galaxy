#include "CollisionsManager.h"
#include "ECS/Components.h"
#include <math.h>
#include <cmath>

// Struct that define the status of a collision

struct SATstatus {

	bool collision;
	float penetration;
	float penetrationAngle;
};

float getProjection(Vector2D axis, Point point);
float findMinimumPoint(std::vector<Point> points, Vector2D axis);
float findMaximumPoint(std::vector<Point> points, Vector2D axis);
SATstatus polygon_polygon_SAT(Convex polygonA, Convex polygonB);

Collision2D CollisionsManager::AABB(const ColliderComponent* colA, const ColliderComponent* colB) {

	Collision2D collision;

	if (colA->collider.x + colA->collider.w >= colB->collider.x and
		colB->collider.x + colB->collider.w >= colA->collider.x and
		colA->collider.y + colA->collider.h >= colB->collider.y and
		colB->collider.y + colB->collider.h >= colA->collider.y) {

		collision.collision = true;

		// Calculating various penetrations
		int xpenetrationA = colA->collider.x + colA->collider.w - colB->collider.x;
		int xpenetrationB = colB->collider.x + colB->collider.w - colA->collider.x;
		int ypenetrationA = colA->collider.y + colA->collider.h - colB->collider.y;
		int ypenetrationB = colB->collider.y + colB->collider.h - colA->collider.y;

		int xpenetration;
		int ypenetration;

		if (fabs(xpenetrationA) < fabs(xpenetrationB)) {

			xpenetration = xpenetrationA;
		}

		else xpenetration = -xpenetrationB;

		if (fabs(ypenetrationA) < fabs(ypenetrationB)) {

			ypenetration = ypenetrationA;
		}

		else ypenetration = -ypenetrationB;

		if (fabs(xpenetration) <= fabs(ypenetration)) {

			collision.penetration.x = float(xpenetration);
		}

		else collision.penetration.y = float(ypenetration);

		collision.colliderA = colA->id;
		collision.colliderB = colB->id;
	}

	return collision;
}

Collision2D CollisionsManager::SAT(const ColliderComponent* colA, const ColliderComponent* colB) {

	Collision2D collision;
	Vector2D MTV;

	if (AABB(colA, colB).collision) {

		for (auto polygonA : colA->destPolygon)
		for (auto polygonB : colB->destPolygon) {

			for (int i = 0; i < signed(polygonA.size()); i++) {

				polygonA[i].x -= MTV.x;
				polygonA[i].y -= MTV.y;
			}

			SATstatus Astatus = polygon_polygon_SAT(polygonA, polygonB);
			SATstatus Bstatus = polygon_polygon_SAT(polygonB, polygonA);

			if (Astatus.collision and Bstatus.collision) {

				collision.collision = true;

				if (Astatus.penetration < Bstatus.penetration) {

					MTV += Vector2D(
						-Astatus.penetration * sin(Astatus.penetrationAngle),
						-Astatus.penetration * cos(Astatus.penetrationAngle)
					);
				}

				else {

					MTV += Vector2D(
						Bstatus.penetration * sin(Bstatus.penetrationAngle),
						Bstatus.penetration * cos(Bstatus.penetrationAngle)
					);
				}

			}
		}
	}
	
	collision.penetration = MTV;

	collision.colliderA = colA->id;
	collision.colliderB = colB->id;

	return collision;
}

Collision2D CollisionsManager::areInCollision(ColliderComponent* colA, ColliderComponent* colB) {

	Collision2D collision;

	if (!colA->isAdvanced() and !colB->isAdvanced()) {

		collision = AABB(colA, colB);
		return collision;
	}

	collision = SAT(colA, colB);
	return collision;
}

void CollisionsManager::update() {

	for (int i = 0; i < signed(colliders.size()); i++) {

		for (int j = 0; j < signed(colliders.size()); j++) {

			if (i != j) {

				Collision2D collision = areInCollision(colliders[i], colliders[j]);

				if (collision.collision) {

					for (auto& c : colliders[i]->entity->components) {

						c->onCollision2D(collision);
					}
				}
			}
		}
	}
}

void CollisionsManager::refresh() {

	colliders.erase(std::remove_if(std::begin(colliders), std::end(colliders), [](ColliderComponent* mCollider) {

		return !mCollider->entity->isActive();
	}),

	std::end(colliders));
}

/////////////////////////// External functions

float getProjection(Vector2D axis, Point point) { 
	
	return axis.x * point.x + axis.y * point.y;
}

float findMinimumPoint(std::vector<Point> points, Vector2D axis) {

	float minPoint = getProjection(axis, points[0]);

	for (int i = 1; i < signed(points.size()); i++) {

		float dot = getProjection(axis, points[i]);
		if (dot < minPoint) minPoint = dot;
	}

	return minPoint;
}

float findMaximumPoint(std::vector<Point> points, Vector2D axis) {

	float maxPoint = getProjection(axis, points[0]);

	for (int i = 1; i < signed(points.size()); i++) {

		float dot = getProjection(axis, points[i]);
		if (dot > maxPoint) maxPoint = dot;
	}

	return maxPoint;
}

SATstatus polygon_polygon_SAT(Convex polygonA, Convex polygonB) {

	SATstatus status;
	bool firstPenetration = true;

	status.collision = true;

	for (int i = 0; i < signed(polygonA.size()); i++) {

		Vector2D axis;

		if (i < signed(polygonA.size()) - 1) {

			axis = Vector2D(-(polygonA[i + 1].y - polygonA[i].y), polygonA[i + 1].x - polygonA[i].x);
		}

		else {

			axis = Vector2D(-(polygonA[0].y - polygonA[i].y), polygonA[0].x - polygonA[i].x);
		}

		float magnitude = float(sqrt(pow(axis.x, 2) + pow(axis.y, 2)));

		if (magnitude != 0) axis *= Vector2D(1 / magnitude, 1 / magnitude);

		float polygonApmax = findMaximumPoint(polygonA, axis);
		float polygonApmin = findMinimumPoint(polygonA, axis);
		float polygonBpmax = findMaximumPoint(polygonB, axis);
		float polygonBpmin = findMinimumPoint(polygonB, axis);

		if (polygonApmax < polygonBpmin or polygonApmin > polygonBpmax) {

			// Found hole
			status.collision = false;
			status.penetration = 0;
			status.penetrationAngle = 0;
			break;
		}

		else if (!polygonA[i].internal) {

			float penetration = fabs(polygonApmin - polygonBpmax);

			if (firstPenetration or penetration < status.penetration) {

				firstPenetration = false;
				status.penetration = penetration;
				status.penetrationAngle = atan2(axis.x, axis.y);
			}
		}
	}

	return status;
}
