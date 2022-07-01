#include "WorldTransform.h"

void WorldTransform::Set() {
	matWorld_.Rotation(rotation_);
	matWorld_.Scale(scale_);
	matWorld_.Transform(translation_);
}