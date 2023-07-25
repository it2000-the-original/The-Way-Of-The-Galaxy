#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Point.h"
#include "CollisionsManager.h"
#include "PositionComponent.h"

using Convex = std::vector<Point>;   // A std::vector of points that define a convex polygon
using Polygon = std::vector<Convex>; // A std::vector of convex polygons to create a concave polygons

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
		for (int i = 0; i < signed(destPolygon.size()); i++)
		for (int j = 0; j < signed(destPolygon[i].size()); j++)

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