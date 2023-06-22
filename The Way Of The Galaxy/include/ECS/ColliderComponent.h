#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "CollisionsManager.h"
#include "PositionComponent.h"

struct Point;                        // Pre-declaration of the Point struct for the 2 lines of code under here
using Convex = std::vector<Point>;   // A std::vector of points that define a convex polygon
using Polygon = std::vector<Convex>; // A std::vector of convex polygons to create a concave polygons

struct Point {

	float x, y;
	bool internal = false;

	/* The internal variable is used to know in case of a convex decomposition
	 * of a polygon, to know if a point is at the start of a side condivided
	 * by two polygons, this is for ignore that sides, in the calculation of
	 * the MTV (Minimal Translation Vector) and to avoid bad effects. */

	Point() {

		x = 10.0;
		y = 10.0;
	}

	Point(float px, float py) {

		x = px;
		y = py;
	}

	Point(float px, float py, bool mInternal) {

		x = px;
		y = py;
		internal = mInternal;
	}

	// operators methods to apply

	Point Add(Point poi) {

		x += poi.x;
		y += poi.y;
		return *this;
	}

	Point Subtract(Point poi) {

		x -= poi.x;
		y -= poi.y;
		return *this;
	}

	Point Multiply(Point poi) {

		x *= poi.x;
		y *= poi.y;
		return *this;
	}

	Point Divide(Point poi) {

		x /= poi.x;
		y /= poi.y;
		return *this;
	}

	// Defining operators......

	friend Point operator+(Point p1, const Point p2) { return p1.Add(p2);      }
	friend Point operator-(Point p1, const Point p2) { return p1.Subtract(p2); }
	friend Point operator*(Point p1, const Point p2) { return p1.Multiply(p2); }
	friend Point operator/(Point p1, const Point p2) { return p1.Divide(p2);   }

	Point operator+=(Point p) { return Add(p);      }
	Point operator-=(Point p) { return Subtract(p); }
	Point operator*=(Point p) { return Multiply(p); }
	Point operator/=(Point p) { return Divide(p);   }

	// A function to get the projection of a vector between the origin and
	// this point to another. Created to simplify the operations in the SAT algorithm
	float getProjection(Point point) { return x * point.x + y * point.y; }

};

class ColliderComponent : public Component {

private:

	PositionComponent* position;

	Polygon srcPolygon;
	bool advanced = false;

public:

	std::size_t id;
	SDL_Rect collider;
	Polygon destPolygon;

	ColliderComponent(std::size_t mId) {

		id = mId;
	}

	ColliderComponent(std::size_t mId, Polygon mPolygon) {

		id = mId;
		advanced = true;
		srcPolygon = mPolygon;
		destPolygon = mPolygon;
	}

	void init() override {

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

	void update() override {

		// Updating the position of collider...
		collider.x = static_cast<int>(position->position.x);
		collider.y = static_cast<int>(position->position.y);
		collider.w = position->width * position->scale;
		collider.h = position->height * position->scale;

		// Updating the position of the polygons...
		for (int i = 0; i < destPolygon.size(); i++)
		for (int j = 0; j < destPolygon[i].size(); j++)

		destPolygon[i][j] = srcPolygon[i][j] + Point(
			float(position->position.x),
			float(position->position.y)
		);
	}

	void setPolygon(Polygon polygon) {

		srcPolygon = polygon;
		destPolygon = polygon;
		advanced = true;
	}

	bool isAdvanced() { return advanced; }
};