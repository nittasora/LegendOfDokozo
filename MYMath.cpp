#include "MYMath.h"

Float2 MakeFloat2(float x, float y) {
	Float2 Result = { x, y };
	return Result;
}

Float3 MakeFloat3(float x, float y, float z) {
	Float3 Result = { x, y, z };
	return Result;
}

Float3 MakeFloat3(Float2 *pos2d, float z) {
	Float3 Result = { pos2d->x, pos2d->y, z };
	return Result;
}

Float4 MakeFloat4(float x, float y, float z, float w) {
	Float4 Result = { x, y, z, w };
	return Result;
}

Float4 MakeFloat4(Float3 *pos3d, float w) {
	Float4 Result = { pos3d->x, pos3d->y, pos3d->z, w };
	return Result;
}

Float4 MakeFloat4(Float2 *pos2d1, Float2 *pos2d2) {
	Float4 Result = { pos2d1->x, pos2d1->y, pos2d1->x, pos2d2->y };
	return Result;
}

Float2 operator+(Float2 _Float2_1, Float2 _Float2_2) {
	Float2 result;

	result.x = _Float2_1.x + _Float2_2.x;
	result.y = _Float2_1.y + _Float2_2.y;

	return result;
}

Float3 operator+(Float3 _Float3_1, Float3 _Float3_2) {
	Float3 result;

	result.x = _Float3_1.x + _Float3_2.x;
	result.y = _Float3_1.y + _Float3_2.y;
	result.z = _Float3_1.z + _Float3_2.z;

	return result;
}

Float2 operator-(Float2 _Float2_1, Float2 _Float2_2) {
	Float2 result;

	result.x = _Float2_1.x - _Float2_2.x;
	result.y = _Float2_1.y - _Float2_2.y;

	return result;
}

Float3 operator-(Float3 _Float3_1, Float3 _Float3_2) {
	Float3 result;

	result.x = _Float3_1.x - _Float3_2.x;
	result.y = _Float3_1.y - _Float3_2.y;
	result.z = _Float3_1.z - _Float3_2.z;

	return result;
}

Float2 operator*(Float2 _Float2_1, Float2 _Float2_2) {
	Float2 result;

	result.x = _Float2_1.x * _Float2_2.x;
	result.y = _Float2_1.y * _Float2_2.y;

	return result;
}

Float3 operator*(Float3 _Float3_1, Float3 _Float3_2) {
	Float3 result;

	result.x = _Float3_1.x * _Float3_2.x;
	result.y = _Float3_1.y * _Float3_2.y;
	result.z = _Float3_1.z * _Float3_2.z;

	return result;
}

Float2 operator*(Float2 _Float2_1, float _float) {
	Float2 result;

	result.x = _Float2_1.x * _float;
	result.y = _Float2_1.y * _float;

	return result;
}

Float3 operator*(Float3 _Float3_1, float _float) {
	Float3 result;

	result.x = _Float3_1.x * _float;
	result.y = _Float3_1.y * _float;
	result.z = _Float3_1.z * _float;

	return result;
}

Float2 operator/(Float2 _Float2_1, Float2 _Float2_2) {
	Float2 result;

	result.x = _Float2_1.x / _Float2_2.x;
	result.y = _Float2_1.y / _Float2_2.y;

	return result;
}

Float2 operator/(Float2 _Float2_1, float _float) {
	Float2 result;

	result.x = _Float2_1.x / _float;
	result.y = _Float2_1.y / _float;

	return result;
}

Float3 operator/(Float3 _Float3_1, Float3 _Float3_2) {
	Float3 result;

	result.x = _Float3_1.x / _Float3_2.x;
	result.y = _Float3_1.y / _Float3_2.y;
	result.z = _Float3_1.z / _Float3_2.z;

	return result;
}

float MYMath::CalcuDistance2D(const Float2 *pos1, const Float2 *pos2) {
	return MYMATH_DISTANCE_2(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuDistance2D(const Float3 *pos1, const Float3 *pos2) {
	return MYMATH_DISTANCE_2(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuDistance2D(const Float4 *pos1, const Float4 *pos2) {
	return MYMATH_DISTANCE_2(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuRadian2D(const Float2 *pos1, const Float2 *pos2) {
	return MYMATH_RADIAN(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuRadian2D(const Float3 *pos1, const Float3 *pos2) {
	return MYMATH_RADIAN(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuRadian2D(const Float4 *pos1, const Float4 *pos2) {
	return MYMATH_RADIAN(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuDegree2D(const Float2 *pos1, const Float2 *pos2) {
	return MYMATH_DEGREE(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuDegree2D(const Float3 *pos1, const Float3 *pos2) {
	return MYMATH_DEGREE(pos1->x, pos1->y, pos2->x, pos2->y);
}

float MYMath::CalcuDegree2D(const Float4 *pos1, const Float4 *pos2) {
	return MYMATH_DEGREE(pos1->x, pos1->y, pos2->x, pos2->y);
}

Float2 MYMath::CalcuPosByRadian2D(const float radius, const float radian) {
	return MakeFloat2(cosf(radian) * radius, sinf(radian) * radius);
}

Float3 MYMath::CalcuPosByRadian2D(const float radius, const float radian, const float z) {
	return MakeFloat3(cosf(radian) * radius, sinf(radian) * radius, z);
}

Float4 MYMath::CalcuPosByRadian2D(const float radius, const float radian, const Float2 ZandW) {
	return MakeFloat4(cosf(radian) * radius, sinf(radian) * radius, ZandW.x, ZandW.y);
}

Float2 MYMath::CalcuPosByDegree2D(const float radius, const float degree) {
	float radian = degree * (MYMATH_PI / 180.0f);
	return MakeFloat2(cosf(radian) * radius, sinf(radian) * radius);
}

Float3 MYMath::CalcuPosByDegree2D(const float radius, const float degree, const float z) {
	float radian = degree * (MYMATH_PI / 180.0f);
	return MakeFloat3(cosf(radian) * radius, sinf(radian) * radius, z);
}

Float4 MYMath::CalcuPosByDegree2D(const float radius, const float degree, const Float2 ZandW) {
	float radian = degree * (MYMATH_PI / 180.0f);
	return MakeFloat4(cosf(radian) * radius, sinf(radian) * radius, ZandW.x, ZandW.y);
}

Float2 MYMath::CalcuPosToAlpha2D(const Float2 *pos1, const Float2 *pos2, float alpha) {
	return MakeFloat2(pos1->x + ((pos2->x - pos1->x) * cosf(alpha)) - ((pos2->y - pos1->y) * sinf(alpha)), pos1->y + ((pos2->x - pos1->x) * sinf(alpha)) + ((pos2->y - pos1->y) * cosf(alpha)));
}

Float3 MYMath::CalcuPosToAlpha2D(const Float3 *pos1, const Float3 *pos2, float alpha) {
	return MakeFloat3(pos1->x + ((pos2->x - pos1->x) * cosf(alpha)) - ((pos2->y - pos1->y) * sinf(alpha)), pos1->y + ((pos2->x - pos1->x) * sinf(alpha)) + ((pos2->y - pos1->y) * cosf(alpha)), pos1->z);
}

Float4 MYMath::CalcuPosToAlpha2D(const Float4 *pos1, const Float4 *pos2, float alpha) {
	return MakeFloat4(pos1->x + ((pos2->x - pos1->x) * cosf(alpha)) - ((pos2->y - pos1->y) * sinf(alpha)), pos1->y + ((pos2->x - pos1->x) * sinf(alpha)) + ((pos2->y - pos1->y) * cosf(alpha)), pos1->z, pos1->w);
}

Float2 MYMath::UnitVector2D(const Float2 *pos1, const Float2 *pos2) {
	Float2 manhattan = *pos2 - *pos1;
	float distance = 0;

	distance = MYMATH_DISTANCE_1(manhattan.x, manhattan.y);

	if (distance > 0) {
		return MakeFloat2(manhattan.x / distance, manhattan.y / distance);
	}
	return MakeFloat2(0.0f, 0.0f);
}

//Float2 MYMath::UnitVector2D(const Float2 *pos1, const Float2 *pos2) {
//	float degree = MYMath::CalcuDegree2D(pos2, pos1);
//
//	degree += 180.0f;
//
//	return MYMath::CalcuPosByDegree2D(1.0f, degree);
//}

float MYMath::CrossVector2D(const Float2 *vec1, const Float2 *vec2) {
	return (vec1->x * vec2->y) - (vec1->y * vec2->x);
}

bool MYMath::HitBoxToBox2D(const Float3 *LeftTop1, const Float3 *RightBottom1, const Float3 *LeftTop2, const Float3 *RightBottom2) {
	if (LeftTop1->x > RightBottom2->x) {
		return false;
	}
	if (RightBottom1->x < LeftTop2->x) {
		return false;
	}
	if (LeftTop1->y < RightBottom2->y) {
		return false;
	}
	if (RightBottom1->y < LeftTop2->y) {
		return false;
	}
	return true;
}

bool MYMath::CrossLineToLine2D(const Float2 *start1, const Float2 *dirvec1, const Float2 *start2, const Float2 *dirvec2, Float2 *crosspoint) {
	float cross = MYMATH_CROSSVECTOR(dirvec1->x, dirvec1->y, dirvec2->x, dirvec2->y);

	if (cross == 0.0f) {
		return false;
	}

	Float2 vector = *start2 - *start1;

	float cross_vec_dir1 = MYMATH_CROSSVECTOR(vector.x, vector.y, dirvec1->x, dirvec1->y);
	float cross_vec_dir2 = MYMATH_CROSSVECTOR(vector.x, vector.y, dirvec2->x, dirvec2->y);

	float distance1 = cross_vec_dir2 / cross;
	float distance2 = cross_vec_dir1 / cross;

	float errorvalue = 0.0001f;

	if (distance1 + errorvalue < 0 || distance1 - errorvalue > 1 || distance2 + errorvalue < 0 || distance2 - errorvalue > 1) {
		return false;
	}

	*crosspoint = *start1 + ((*dirvec1) * distance1);

	return true;
}
