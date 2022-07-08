#include "WorldTransform.h"

void WorldTransform::Set() {
	Matrix4 matRot;
	matRot.Identity();

	Matrix4 matScale;
	matScale.Identity();

	Matrix4 matTrans;
	matTrans.Identity();

	matScale.Scale(scale_);
	matTrans.Translation(translation_);
	matRot.Rotation(rotation_);

	matWorld_.Identity();

	matWorld_ *= matScale;
	matWorld_ *= matRot;
	matWorld_ *= matTrans;
}