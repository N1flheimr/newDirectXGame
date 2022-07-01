#pragma once
#include "Vector3.h"
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	//単位行列
	void Identity();

	//スケーリング設定
	void Scale(const Vector3 scale);
	//回転設定
	void Rotation(const Vector3 rot);
	//平行移動設定
	void Translation(const Vector3 trans);
};
