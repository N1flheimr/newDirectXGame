#include "Vector3.h"
#include <cmath>

Vector3& Vector3::Zero() {
	x = 0.f;
	y = 0.f;
	z = 0.f;
	return *this;
}

Vector3 Vector3::Cross(const Vector3& v)const {
	Vector3 vecCross = { (y * v.z) - (z * v.y) ,(z * v.x) - (x * v.z) ,(x * v.y) - (y * v.x) };
	return vecCross;
}

float Vector3::Magnitude()const {
	return sqrtf(powf(x, 2.f) + powf(y, 2.f) + powf(z, 2.f));
}

float Vector3::Dot(const Vector3& v)const {
	return x * v.x + y * v.y + z * v.z;
}

Vector3& Vector3::Normalize() {
	float mag = Magnitude();
	if (mag != 0) {
		return *this /= mag;
	}
	return *this;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return v * s;
}

const Vector3 operator*(float s, const Vector3& v) {
	Vector3 temp(v);
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return v / s;
}

float DegreeToRad(float deg) {
	if (deg == 0) {
		return 0.1f;
	}
	const float PI = 3.1415;
	return deg * (PI / 180);
}

float RadToDegree(float rad) {
	const float PI = 3.1415;
	return rad * (180 / PI);
}