#pragma once
#include <SDL2/SDL.h>
#include "Components.h"
#include <string>

struct Point {

	float x, y;
	bool internal = false;

	Point() {

		x = 10.0;
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

	Point Add(Point poi) {

		x += poi.x;
		y += poi.y;
		return *this;
	}

	Point Multiply(Point poi) {

		x *= poi.x;
		y *= poi.y;
		return *this;
	}

	Point Subtract(Point poi) {

		x -= poi.x;
		y -= poi.y;
		return *this;
	}
	
	friend Point operator*(Point p1, Point p2) {

		return p1.Multiply(p2);
	}

	friend Point operator+(Point p1, const Point p2) {

		return p1.Add(p2);
	}

	friend Point operator-(Point p1, const Point p2) {

		return p1.Subtract(p2);
	}
	
	Point operator*=(Point p2) {

		return Multiply(p2);
	}

	Point operator+=(Point p2) {

		return Add(p2);
	}

	Point operator-=(Point p2) {

		return Subtract(p2);
	}

	float getProjection(Point point) {

		return x * point.x + y * point.y;
	}

};

class ColliderComponent : public Component {

public:

	SDL_Rect collider;
	bool polygon = false;
	std::vector<std::vector<Point>> srcPolygons;
	std::vector<std::vector<Point>> destPolygons;
	std::size_t id;

	PositionComponent* position;

	ColliderComponent(std::size_t t) {

		id = t;
	}

	ColliderComponent(std::size_t t, std::vector<std::vector<Point>> mPolygons) {

		id = t;
		srcPolygons = mPolygons;
		destPolygons = mPolygons;
		polygon = true;
	}

	void init() override {

		if (!entity->hasComponent<PositionComponent>()) {

			entity->addComponent<PositionComponent>();
		}

		position = &entity->getComponent<PositionComponent>();
		Engine::colliders.push_back(this);

		if (!polygon) {

			destPolygons = srcPolygons = { {

				Point(0.0, 0.0),
				Point(float(position->width), 0.0),
				Point(float(position->width), float(position->height)),
				Point(0.0, float(position->height))
			} };
		}
	}

	void update() override {

		collider.x = static_cast<int>(position->position.x);
		collider.y = static_cast<int>(position->position.y);
		collider.w = position->width * position->scale;
		collider.h = position->height * position->scale;

		for (int i = 0; i < destPolygons.size(); i++)
		for (int j = 0; j < destPolygons[i].size(); j++)
		destPolygons[i][j] = srcPolygons[i][j] + Point(float(position->position.x), float(position->position.y));
	}
};