#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Polygon.h"
#include "Rectangle.h"
#include "ECS.h"

struct Point;
class PositionComponent;

class ColliderComponent : public Component {

private:

	PositionComponent* position;

	Polygon srcPolygon;
	bool advanced = false;

public:

	std::size_t id;
	Rectangle collider;
	Polygon destPolygon;

	ColliderComponent(std::size_t mId);
	ColliderComponent(std::size_t mId, Polygon mPolygon);

	void init() override;
	void update() override;

	void setPolygon(Polygon polygon);
	bool isAdvanced();
};