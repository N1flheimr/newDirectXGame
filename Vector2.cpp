#include "Vector2.h"
#include <cmath>

Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

float Vector2::Magnitude()const {
	return sqrtf(pow(x, 2) + pow(y, 2));
}

Vector2& Vector2::Norm() {
	float mag = Magnitude();
	if (mag != 0) {
		return *this /= mag;
	}
	return *this;
}

float Vector2::Dot(const Vector2& v)const {
	return x * v.x + y * v.y;
}

float Vector2::Cross(const Vector2& v)const {
	return x * v.y + y * v.x;
}

Vector2 Vector2::operator+()const {
	return *this;
}

Vector2 Vector2::operator-()const {
	return Vector2(-*this);
}

Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s) {
	Vector2 temp(v);
	return s * v;
}

const Vector2 operator*(float s, const Vector2 v) {
	return v * s;
}

const Vector2 operator/(const Vector2& v, float s) {
	Vector2 temp(v);
	return v / s;
}