#include "ConvexDecomposition.h"

std::vector<Polygon> decompose(Polygon polygon) {

    std::vector<cxd::Vertex> vertices = Point_Vertex_Conversion(polygon);

    cxd::ConcavePolygon concavePolygon(vertices);

    concavePolygon.convexDecomp();

    std::vector<cxd::ConcavePolygon> subConvexs;
    concavePolygon.returnLowestLevelPolys(subConvexs);

    // Convert output to my values

    std::vector<Polygon> polygons = ConcavePolygon_Polygon_Conversion(subConvexs);

    findParentPoints(polygons, polygon);
    findInternalPoints(polygons, polygon);

    return polygons;
}

std::vector<cxd::Vertex> Point_Vertex_Conversion(Polygon polygon) {

    std::vector<cxd::Vertex> vertices;

    for (const auto& point : polygon) {

        vertices.push_back(cxd::Vec2({point.x, point.y}));
    }

    return vertices;
}

std::vector<Polygon> ConcavePolygon_Polygon_Conversion(std::vector<cxd::ConcavePolygon> subConvexs) {

    std::vector<Polygon> polygons;

    for (auto subConvex : subConvexs) {

        Polygon polygon;

        std::vector<cxd::Vertex> subConvexVertices = subConvex.getVertices();

        for (int i = subConvexVertices.size(); i--; i <= 0) {

            polygon.insert(std::begin(polygon), Point(subConvexVertices[i].position.x, subConvexVertices[i].position.y));
        }

        polygons.push_back(polygon);
    }

    return polygons;
}

void findInternalPoints(std::vector<Polygon>& polygons, Polygon polygon) {

    for (int i = 0; i < polygons.size(); i++)
    for (int j = 0; j < polygons[i].size(); j++)

    if (!polygons[i][j].internal)
    
    for (int k = 0; k < polygons.size(); k++)

    if (k != i)
    
    for (int l = 0; l < polygons[k].size(); l++) {

        if (!polygons[k][l].internal and polygons[i][j] == polygons[k][l]) {

            if (j < polygons[i].size() - 1) {

                if (l > 0 and polygons[i][j+1] == polygons[k][l-1]) {

                    polygons[i][j].internal = true;
                    polygons[k][l-1].internal = true;
                }

                else if (polygons[i][j+1] == polygons[k][polygons[k].size()-1]) {

                    polygons[i][j].internal = true;
                    polygons[k][polygons[k].size()-1].internal = true;
                }
            }

            else {

                if (l > 0 and polygons[i][0] == polygons[k][l-1]) {

                    polygons[i][j].internal = true;
                    polygons[k][l-1].internal = true;
                }

                else if (polygons[i][0] == polygons[k][polygons[k].size()-1]) {

                    polygons[i][j].internal = true;
                    polygons[k][polygons[k].size()-1].internal = true;
                }
            }
        }
    }
}

void findParentPoints(std::vector<Polygon>& polygons, Polygon polygon) {

    for (int i = 0; i < polygons.size(); i++) {

        if (!exist(polygons[i][0], polygon)) {

            polygons[i][0].parentX = polygons[i+1][polygons[i+1].size()-1].x;
            polygons[i][0].parentY = polygons[i+1][polygons[i+1].size()-1].y;
            polygons[i+1][0].parentX = polygons[i][1].x; 
            polygons[i+1][0].parentY = polygons[i][1].y; 

            i++;
        }
    }
}

bool exist(Point point, Polygon polygon) {

    for (const auto& _point : polygon) {

        if (point == _point) return true;
    }

    return false;
}