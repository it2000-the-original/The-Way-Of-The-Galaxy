#pragma once
#include <vector>
#include "Point.h"
#include "ConcavePolygon.h"


using Polygon = std::vector<Point>;

std::vector<Polygon> decompose(Polygon polygon);
std::vector<cxd::Vertex> Point_Vertex_Conversion(Polygon polygon);
std::vector<Polygon> ConcavePolygon_Polygon_Conversion(std::vector<cxd::ConcavePolygon> subPolygons);
void findInternalPoints(std::vector<Polygon>& polygons, Polygon polygon);
void findParentPoints(std::vector<Polygon>& polygons, Polygon polygon);
bool exist(Point point, Polygon polygon);