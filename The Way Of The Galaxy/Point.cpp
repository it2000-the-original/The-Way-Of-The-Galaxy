#include "Point.h"
#include <cmath>

Point::Point() {

	x = 10.0;
	y = 10.0;
}

Point::Point(double px, double py) {

	x = px;
	y = py;
}

Point::Point(double px, double py, bool mInternal) {

	x = px;
	y = py;
	internal = mInternal;
}

Point Point::Add(Point poi) {

	x += poi.x;
	y += poi.y;
	return *this;
}

Point Point::Subtract(Point poi) {

	x -= poi.x;
	y -= poi.y;
	return *this;
}

Point Point::Multiply(Point poi) {

	x *= poi.x;
	y *= poi.y;
	return *this;
}

Point Point::Divide(Point poi) {

	x /= poi.x;
	y /= poi.y;
	return *this;
}

Point Point::Round(int i) {

	x = round(x * pow(10, i)) / pow(10, i);
	y = round(y * pow(10, i)) / pow(10, i);
	return *this;
}

Point operator+(Point p1, const Point p2) {
	
	return p1.Add(p2);
}

Point operator-(Point p1, const Point p2) {
	
	return p1.Subtract(p2);
}

Point operator*(Point p1, const Point p2) {
	
	return p1.Multiply(p2);
}

Point operator/(Point p1, const Point p2) {
	
	return p1.Divide(p2);
}

Point Point::operator+=(Point p) {
	
	return Add(p);
}

Point Point::operator-=(Point p) {
	
	return Subtract(p);
}

Point Point::operator*=(Point p) {
	
	return Multiply(p);
}

Point Point::operator/=(Point p) {
	
	return Divide(p);
}

bool Point::operator==(Point point) {

	return (x == point.x and y == point.y) or
	(parentX == point.x and parentY == point.y);
}