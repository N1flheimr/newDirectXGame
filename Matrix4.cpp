#include "Matrix4.h"
#include <cmath>

void Matrix4::Identity() {
	*this = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

void Matrix4::Scale(const Vector3 scale)
{
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
	m[3][3] = 1;
}

void Matrix4::Rotation(const Vector3 rot) {
	Matrix4 rotX;
	Matrix4 rotY;
	Matrix4 rotZ;

	rotX = {
		1,0,0,0,
		0,cosf(rot.x),sinf(rot.x),0,
		0,-sinf(rot.x),cosf(rot.x),0,
		0,0,0,1
	};
	rotY = {
		cosf(rot.y),0,-sinf(rot.y),0,
		0,1,0,0,
		sinf(rot.y),0,cosf(rot.y),0,
		0,0,0,1
	};
	rotZ = {
		cosf(rot.z),sinf(rot.z),0,0,
		-sinf(rot.z),cosf(rot.z),0,0,
		0,0,1,0,
		0,0,0,1
	};
	*this *= rotZ;
	*this *= rotX;
	*this *= rotY;
}

void Matrix4::Transform(const Vector3 trans) {
	m[3][0] = trans.x;
	m[3][1] = trans.y;
	m[3][2] = trans.z;
	m[3][3] = 1;
}
