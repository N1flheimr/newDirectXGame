#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成

	//メンバー関数
	float Magnitude() const;
	//内積
	float Dot(const Vector3& v)const;
	//外積
	Vector3 Cross(const Vector3& v)const;
	//ゼロベクトル
	Vector3& Zero();
	//正規化する
	Vector3& Normalize();

	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
	Vector3& operator+=(float s);
};

const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);