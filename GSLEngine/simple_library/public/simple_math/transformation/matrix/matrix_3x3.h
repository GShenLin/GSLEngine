#pragma once

struct fmatrix_3x3
{
	float m11; float m12; float m13;
	float m21; float m22; float m23;
	float m31; float m32; float m33;

public:
	fmatrix_3x3();

	fmatrix_3x3(
	float in_m11, float in_m12, float in_m13,
	float in_m21, float in_m22, float in_m23,
	float in_m31, float in_m32, float in_m33);

	//行列式
	float Determinant();

	//单位化
	static fmatrix_3x3 identity();
};