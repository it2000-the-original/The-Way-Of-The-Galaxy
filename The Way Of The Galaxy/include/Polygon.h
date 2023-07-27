#pragma once
#include <vector>

struct Point;

using Convex = std::vector<Point>;   // A std::vector of points that define a convex polygon
using Polygon = std::vector<Convex>; // A std::vector of convex polygons to create a concave polygons
