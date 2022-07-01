#include "WorldTransform.h"

void WorldTransform::Set() {
	matWorld_.Scale(scale_);
	matWorld_.Rotation(rotation_);
	matWorld_.Transform(translation_);
	TransferMatrix();
}