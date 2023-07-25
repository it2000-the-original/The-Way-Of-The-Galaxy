#include "Vector2D.h"

Vector2D::Vector2D() {

	x = y = 0;
}

Vector2D::Vector2D(float mX, float mY) {

	x = mX;
	y = mY;
}

Vector2D& Vector2D::Add(const Vector2D vec) {

	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D vec) {

	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D vec) {

	x *= vec.x;
	y *= vec.y;
	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D vec) {

	x /= vec.x;
	y /= vec.y;
	return *this;
}

Vector2D operator+(Vector2D v1, const Vector2D v2) {

	return v1.Add(v2);
}

Vector2D operator-(Vector2D v1, const Vector2D v2) {

	return v1.Subtract(v2);
}

Vector2D operator*(Vector2D v1, const Vector2D v2) {

	return v1.Multiply(v2);
}

Vector2D operator/(Vector2D v1, const Vector2D v2) {

	return v1.Divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D vec) {

	return Add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D vec) {

	return Subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D vec) {

	return Multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D vec) {

	return Divide(vec);
}

bool Vector2D::operator!=(const Vector2D vec) {

	return x != vec.x and y != vec.y;
}

bool Vector2D::operator==(const Vector2D vec) {

	return x == vec.x and y == vec.y;
}

Vector2D& Vector2D::operator*(const int i) {

	x *= i;
	y *= i;
	return *this;
}

Vector2D& Vector2D::Zero() {

	x = 0;
	y = 0;
	return *this;
}

Vector2D& Vector2D::Round(int precision) {

	x = round(x * pow(10, precision)) / pow(10, precision);
	y = round(y * pow(10, precision)) / pow(10, precision);
	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec) {

	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
}