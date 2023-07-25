#pragma once

struct Point {

	float x, y;
	bool internal = false;

	/* The internal variable is used to know in case of a convex decomposition
	 * of a polygon, to know if a point is at the start of a side condivided
	 * by two polygons, this is for ignore that sides, in the calculation of
	 * the MTV (Minimal Translation Vector) and to avoid bad effects. */

	Point();
	Point(float px, float py);
	Point(float px, float py, bool mInternal);

	// operators methods to apply

	Point Add(Point poi);
	Point Subtract(Point poi);
	Point Multiply(Point poi);
	Point Divide(Point poi);

	// Defining operators......

	friend Point operator+(Point p1, const Point p2);
	friend Point operator-(Point p1, const Point p2);
	friend Point operator*(Point p1, const Point p2);
	friend Point operator/(Point p1, const Point p2);

	Point operator+=(Point p);
	Point operator-=(Point p);
	Point operator*=(Point p);
	Point operator/=(Point p);
};
