#include <vector>
#include "Point.h"
#include "Polygon.h"
#include "ConcavePolygon.h"

Polygon decompose(std::vector<Point> polygon);
std::vector<cxd::Vertex> Point_Vertex_Conversion(std::vector<Point> polygon);
Polygon ConcavePolygon_Polygon_Conversion(std::vector<cxd::ConcavePolygon> subPolygons);
void findInternalPoints(Polygon& polygons, std::vector<Point> polygon);
void findParentPoints(Polygon& polygons, std::vector<Point> polygon);
bool exist(Point point, std::vector<Point> polygon);