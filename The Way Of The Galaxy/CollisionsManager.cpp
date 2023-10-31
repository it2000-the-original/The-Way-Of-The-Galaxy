#include "CollisionsManager.h"
#include "ECS/Components.h"
#include "Engine.h"
#include <math.h>
#include <cmath>

// Struct that define the status of a collision

struct SATstatus {

	bool collision;
	double wallAngle;
	double penetration;
	double penetrationAngle;
};

double getProjection(Vector2D axis, Point point);
double findMinimumPoint(std::vector<Point> points, Vector2D axis);
double findMaximumPoint(std::vector<Point> points, Vector2D axis);
SATstatus polygon_polygon_SAT(Polygon polygonA, Polygon polygonB);

Collision2D CollisionsManager::AABB(const ColliderComponent* colA, const ColliderComponent* colB) {

	Collision2D collision;

	if (colA->collider.x + colA->collider.w >= colB->collider.x and
		colB->collider.x + colB->collider.w >= colA->collider.x and
		colA->collider.y + colA->collider.h >= colB->collider.y and
		colB->collider.y + colB->collider.h >= colA->collider.y) {

		collision.collision = true;

		// Calculating various penetrations
		double xpenetrationA = colA->collider.x + colA->collider.w - colB->collider.x;
		double xpenetrationB = colB->collider.x + colB->collider.w - colA->collider.x;
		double ypenetrationA = colA->collider.y + colA->collider.h - colB->collider.y;
		double ypenetrationB = colB->collider.y + colB->collider.h - colA->collider.y;

		double xpenetration;
		double ypenetration;

		if (fabs(xpenetrationA) < fabs(xpenetrationB)) {

			xpenetration = xpenetrationA;
		}

		else xpenetration = -xpenetrationB;

		if (fabs(ypenetrationA) < fabs(ypenetrationB)) {

			ypenetration = ypenetrationA;
		}

		else ypenetration = -ypenetrationB;

		if (fabs(xpenetration) <= fabs(ypenetration)) {

			collision.penetration.x = xpenetration;
			collision.angle = -atan2(xpenetration, 0);
		}

		else {

			collision.penetration.y = ypenetration;
			collision.angle = atan2(0, ypenetration);
		}

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

					collision.angle = Astatus.wallAngle - M_PI;
				}

				else {

					MTV += Vector2D(
						Bstatus.penetration * sin(Bstatus.penetrationAngle),
						Bstatus.penetration * cos(Bstatus.penetrationAngle)
					);

					collision.angle = Bstatus.wallAngle;
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

		std::vector<Collision2D> walls;
		std::vector<int> wallsColliders;

		for (int j = 0; j < signed(colliders.size()); j++) {

			if (i != j) {

				Collision2D collision = areInCollision(colliders[i], colliders[j]);

				if (collision.collision and collision.colliderB == wallId) {

					walls.push_back(collision);
					wallsColliders.push_back(j);
				}

				else if (collision.collision) {

					for (auto& c : colliders[i]->entity->components) {

						c->onCollision2D(collision);
					}
				}
			}
		}

		// Check if one of the signed penetrations are sufficient
		// to go out the collision

		bool singlePenetration = false;

		for (int j = 0; j < walls.size(); j++) {

			ColliderComponent collider = *colliders[i];
			collider.collider.x -= walls[j].penetration.x;
			collider.collider.y -= walls[j].penetration.y;
			collider.updatePolygon();

			bool _collision = false;

			for (int z = 0; z < wallsColliders.size(); z++) {

				Collision2D collision2 = areInCollision(&collider, colliders[wallsColliders[z]]);

				if (collision2.penetration != Vector2D().Zero()) {

					_collision = true;
					break;
				}
			}
			
			if (!_collision) {

				for (auto& c : colliders[i]->entity->components) {

					c->onCollision2D(walls[j]);
				}

				singlePenetration = true;
				break;
			}
		}

		// Apply all walls penetrations if there isn't
		// one of the to go out of the penetration

		if (!singlePenetration) {

			for (auto& wall : walls)
			for (auto& c : colliders[i]->entity->components) {

				c->onCollision2D(wall);
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

double getProjection(Vector2D axis, Point point) { 
	
	return axis.x * point.x + axis.y * point.y;
}

double findMinimumPoint(std::vector<Point> points, Vector2D axis) {

	double minPoint = getProjection(axis, points[0]);

	for (int i = 1; i < signed(points.size()); i++) {

		double dot = getProjection(axis, points[i]);
		if (dot < minPoint) minPoint = dot;
	}

	return minPoint;
}

double findMaximumPoint(std::vector<Point> points, Vector2D axis) {

	double maxPoint = getProjection(axis, points[0]);

	for (int i = 1; i < signed(points.size()); i++) {

		double dot = getProjection(axis, points[i]);
		if (dot > maxPoint) maxPoint = dot;
	}

	return maxPoint;
}

SATstatus polygon_polygon_SAT(Polygon polygonA, Polygon polygonB) {

	SATstatus status;
	bool firstPenetration = true;

	status.collision = true;

	for (int i = 0; i < signed(polygonA.size()); i++) {

		Vector2D axis;
		double angle;

		if (i < signed(polygonA.size()) - 1) {

			axis = Vector2D(-(polygonA[i + 1].y - polygonA[i].y), polygonA[i + 1].x - polygonA[i].x);
			angle = atan2(polygonA[i + 1].y - polygonA[i].y, polygonA[i + 1].x - polygonA[i].x);
		}

		else {

			axis = Vector2D(-(polygonA[0].y - polygonA[i].y), polygonA[0].x - polygonA[i].x);
			angle = atan2(polygonA[0].y - polygonA[i].y, polygonA[0].x - polygonA[i].x);
		}

		double magnitude = sqrt(pow(axis.x, 2) + pow(axis.y, 2));

		if (magnitude != 0) axis *= Vector2D(1 / magnitude, 1 / magnitude);

		double polygonApmax = findMaximumPoint(polygonA, axis);
		double polygonApmin = findMinimumPoint(polygonA, axis);
		double polygonBpmax = findMaximumPoint(polygonB, axis);
		double polygonBpmin = findMinimumPoint(polygonB, axis);

		if (polygonApmax <= polygonBpmin or polygonApmin >= polygonBpmax) {

			// Found hole
			status.collision = false;
			status.penetration = 0;
			status.penetrationAngle = 0;
			break;
		}

		else if (!polygonA[i].internal) {

			double penetration = fabs(polygonApmin - polygonBpmax);

			if (firstPenetration or penetration < status.penetration) {

				firstPenetration = false;
				status.penetration = penetration;
				status.wallAngle = angle;
				status.penetrationAngle = atan2(axis.x, axis.y);
			}
		}
	}

	return status;
}
