#include "CollisionsManager.h"
#include "Components.h"
#include <math.h>
#include <cmath>

// Struct that define the status of a collision

struct SATstatus {

	bool collision;
	float penetration;
	float penetrationAngle;
};

float findMinimumPoint(std::vector<Point> points, Point axis);
float findMaximumPoint(std::vector<Point> points, Point axis);
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
	Point MTV = Point(0, 0);

	if (AABB(colA, colB).collision) {

		for (auto polygonA : colA->destPolygon)
		for (auto polygonB : colB->destPolygon) {

			for (int i = 0; i < polygonA.size(); i++) polygonA[i] -= MTV;

			SATstatus Astatus = polygon_polygon_SAT(polygonA, polygonB);
			SATstatus Bstatus = polygon_polygon_SAT(polygonB, polygonA);

			if (Astatus.collision and Bstatus.collision) {

				collision.collision = true;

				if (Astatus.penetration < Bstatus.penetration) {

					MTV += Point(
						-Astatus.penetration * sin(Astatus.penetrationAngle),
						-Astatus.penetration * cos(Astatus.penetrationAngle)
					);
				}

				else {

					MTV += Point(
						Bstatus.penetration * sin(Bstatus.penetrationAngle),
						Bstatus.penetration * cos(Bstatus.penetrationAngle)
					);
				}

			}
		}
	}
	
	collision.penetration = Vector2D(round(MTV.x), round(MTV.y));

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

	for (int i = 0; i < colliders.size(); i++) {

		for (int j = 0; j < colliders.size(); j++) {

			Collision2D collision = areInCollision(colliders[i], colliders[j]);

			if (i != j and collision.collision) {

				for (auto& c : colliders[i]->entity->components) {

					c->onCollision2D(collision);
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

float findMinimumPoint(std::vector<Point> points, Point axis) {

	float minPoint = axis.getProjection(points[0]);

	for (int i = 1; i < points.size(); i++) {

		float dot = axis.getProjection(points[i]);
		if (dot < minPoint) minPoint = dot;
	}

	return minPoint;
}

float findMaximumPoint(std::vector<Point> points, Point axis) {

	float maxPoint = axis.getProjection(points[0]);

	for (int i = 1; i < points.size(); i++) {

		float dot = axis.getProjection(points[i]);
		if (dot > maxPoint) maxPoint = dot;
	}

	return maxPoint;
}

SATstatus polygon_polygon_SAT(Convex polygonA, Convex polygonB) {

	SATstatus status;
	bool firstPenetration = true;

	status.collision = true;

	for (int i = 0; i < polygonA.size(); i++) {

		Point axis;

		if (i < polygonA.size() - 1) {

			axis = Point(-(polygonA[i + 1].y - polygonA[i].y), polygonA[i + 1].x - polygonA[i].x);
		}

		else {

			axis = Point(-(polygonA[0].y - polygonA[i].y), polygonA[0].x - polygonA[i].x);
		}

		float magnitude = float(sqrt(pow(axis.x, 2) + pow(axis.y, 2)));

		if (magnitude != 0) axis *= Point(1 / magnitude, 1 / magnitude);

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
