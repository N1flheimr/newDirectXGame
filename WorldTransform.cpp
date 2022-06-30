#include "WorldTransform.h"

void WorldTransform::Set(Matrix4& matWorld) {
	matWorld.Rotation(rotation_);
	matWorld.Scale(scale_);
	matWorld.Transform(translation_);
}