#include "Collision.h"
#include "Components.h"
#include <math.h>
#include <cmath>

// Declaring static memeber of the class
std::vector<ColliderComponent*> Collision::colliders;

// Struct that define the status of a collision

struct SATstatus {

	bool collision;
	float penetration;
	float penetrationAngle;
};

float findMinimumPoint(std::vector<Point> points, Point axis);   // Find the smaller projection point in the axis
float findMaximumPoint(std::vector<Point> points, Point axis);   // Find the highter projection point in the axis
SATstatus polygon_polygon_SAT(Convex polygonA, Convex polygonB); // Separating Axis Theorem that return the status of the collision

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {

	if (
		recA.x + recA.w >= recB.x and
		recB.x + recB.w >= recA.x and
		recA.y + recA.h >= recB.y and
		recB.y + recB.h >= recA.y
		) {

		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent colA, const ColliderComponent colB) {

	if (AABB(colA.collider, colB.collider)) {

		return true;
	}

	return false;
}

bool Collision::SAT(const ColliderComponent colA, const ColliderComponent colB) {

	bool isInCollision = false;

	if (AABB(colA, colB)) {

		for (auto polygonA : colA.destPolygon)
		for (auto polygonB : colB.destPolygon) {

			SATstatus Astatus = polygon_polygon_SAT(polygonA, polygonB);
			SATstatus Bstatus = polygon_polygon_SAT(polygonB, polygonA);

			if (Astatus.collision and Bstatus.collision) {

				isInCollision = true;
				break;
			}
		}
	}

	return isInCollision;
}

Vector2D Collision::MTV(const ColliderComponent colA, const ColliderComponent colB) {

	Point MTV = Point(0, 0);

	if (AABB(colA, colB)) {

		for (auto polygonA : colA.destPolygon)
		for (auto polygonB : colB.destPolygon) {

			for (int i = 0; i < polygonA.size(); i++) polygonA[i] -= MTV;

			SATstatus Astatus = polygon_polygon_SAT(polygonA, polygonB);
			SATstatus Bstatus = polygon_polygon_SAT(polygonB, polygonA);

			if (Astatus.collision and Bstatus.collision) {

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
	
	return Vector2D(round(MTV.x), round(MTV.y));
}


void Collision::checkCollisionSAT(ColliderComponent* collider, std::size_t id, Function func) {

	for (auto cc : Collision::colliders) {

		if (cc->id == id) {

			if (Collision::SAT(*collider, *cc)) func();
		}
	}
}


void Collision::subtractCollisionMTV(ColliderComponent* collider, std::size_t id) {

	for (auto cc : Collision::colliders) {

		if (cc->id == id) {

			// Subtracting the mtv between collider and cc from collider
			collider->entity->getComponent<PositionComponent>().position -= Collision::MTV(*collider, *cc);
		}
	}
}

void Collision::refreshColliders() {

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

		float magnitude = sqrt(pow(axis.x, 2) + pow(axis.y, 2));

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
