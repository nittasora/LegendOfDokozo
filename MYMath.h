#pragma once

#include <math.h>

#define DISCHECK(mx, my, yx, yy, ax, ay) ( mx - yx < ax && mx - yx > -ax && my - yy < ay && my - yy > -ay )

struct Float2
{
	float x, y;
};

struct Float3
{
	float x, y, z;
};

struct Float4
{
	float x, y, z, w;
};

struct HitResult
{
	bool isHit;
	Float2 position;
};

//構造体に変える
Float2 MakeFloat2(float, float);
Float3 MakeFloat3(float, float, float);
Float3 MakeFloat3(Float2*, float);
Float4 MakeFloat4(float, float, float, float);
Float4 MakeFloat4(Float3*, float);
Float4 MakeFloat4(Float2*, Float2*);
Float2 operator+(Float2, Float2);
Float3 operator+(Float3, Float3);
Float2 operator-(Float2, Float2);
Float3 operator-(Float3, Float3);
Float2 operator*(Float2, Float2);
Float2 operator*(Float2, float);
Float3 operator*(Float3, Float3);
Float2 operator/(Float2, Float2);
Float2 operator/(Float2, float);
Float3 operator/(Float3, Float3);

//円周率
const float MYMATH_PI = acosf(-1);
//初期化用
const Float2 Float2zero = { 0.0f, 0.0f };
const Float3 Float3zero = { 0.0f, 0.0f, 0.0f };
const Float4 Float4zero = { 0.0f, 0.0f, 0.0f, 0.0f };

#define MYMATH_DISTANCE_1(x, y) sqrtf(x * x + y * y)
#define MYMATH_DISTANCE_2(x1, y1, x2, y2) sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))
#define MYMATH_RADIAN(x1, y1, x2, y2) atan2f(y2 - y1, x2 - x1)
#define MYMATH_DEGREE(x1, y1, x2, y2) (atan2f(y2 - y1, x2 - x1) * (180.0f / MYMATH_PI))
#define MYMATH_CROSSVECTOR(x1, y1, x2, y2) ((x1 * y2) - (y1 * x2))

namespace MYMath {
	//機能	:2点間の距離を出す
	//引数1	:1つめの座標
	//引数2	:2つめの座標
	//戻り値:2点間の距離
	float CalcuDistance2D(const Float2*, const Float2*);
	float CalcuDistance2D(const Float3*, const Float3*);
	float CalcuDistance2D(const Float4*, const Float4*);
	//機能	:ラジアンを出す
	//引数1	:1つめの座標
	//引数2	:2つめの座標
	//戻り値:ラジアン
	float CalcuRadian2D(const Float2*, const Float2*);
	float CalcuRadian2D(const Float3*, const Float3*);
	float CalcuRadian2D(const Float4*, const Float4*);
	//機能	:度数を出す
	//引数1	:1つめの座標
	//引数2	:2つめの座標
	//戻り値:度数
	float CalcuDegree2D(const Float2*, const Float2*);
	float CalcuDegree2D(const Float3*, const Float3*);
	float CalcuDegree2D(const Float4*, const Float4*);
	//機能	:角度(ラジアン)と距離から座標を出す
	//引数1	:半径
	//引数2	:角度
	//戻り値:2つめの座標
	Float2 CalcuPosByRadian2D(const float, const float);
	Float3 CalcuPosByRadian2D(const float, const float, const float);
	Float4 CalcuPosByRadian2D(const float, const float, const Float2);
	//機能	:角度(度数)と距離から座標を出す
	//引数1	:半径
	//引数2	:角度
	//戻り値:2つめの座標
	Float2 CalcuPosByDegree2D(const float, const float);
	Float3 CalcuPosByDegree2D(const float, const float, const float);
	Float4 CalcuPosByDegree2D(const float, const float, const Float2);
	//機能	:特定の角度分等距離で回転
	//引数1	:1つめの座標
	//引数2	:2つめの座標
	//引数2	:角度
	//戻り値:2つめの座標
	Float2 CalcuPosToAlpha2D(const Float2*, const Float2*, float);
	Float3 CalcuPosToAlpha2D(const Float3*, const Float3*, float);
	Float4 CalcuPosToAlpha2D(const Float4*, const Float4*, float);
	//機能	:単位ベクトル(精度極低)
	//引数1	:1つめの座標
	//引数2	:2つめの座標
	//戻り値:単位ベクトル
	Float2 UnitVector2D(const Float2*, const Float2*);
	//機能	:外積
	//引数1	:1つめのベクトル
	//引数2	:2つめのベクトル
	//戻り値:外積
	float CrossVector2D(const Float2*, const Float2*);
	//機能	:四角形当たり(簡易)
	//引数1	:1つめの左上座標
	//引数2	:1つめの右下座標
	//引数3	:2つめの左上座標
	//引数4	:2つめの右下座標
	//戻り値:当たったか
	bool HitBoxToBox2D(const Float3*, const Float3*, const Float3*, const Float3*);
	//機能	:線分と線分交差
	//引数1	:1つめの座標
	//引数2	:1つめの方向
	//引数3	:2つめの座標
	//引数4	:2つめの方向
	//引数5	:交点
	//戻り値:当たったか
	bool CrossLineToLine2D(const Float2*, const Float2*, const Float2*, const Float2*, Float2*);
}
