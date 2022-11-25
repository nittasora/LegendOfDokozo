#pragma once

#include "Information.h"

enum COLLIDERTYPE
{
	COLLIDER_NONE,
	COLLIDER_TRANSFORM,
	COLLIDER_PLAYER
};

struct COLLIDER_RESULT {

	bool isHit;
	Float2 d;

};

class CGameObject;

class CCollider
{
public:
	bool Init(int);
	void Update();
	FRECT Collider_GetXY();
	void HitReaction_Play(CGameObject*, COLLIDER_RESULT*);

	bool isActive;
	CGameObject *pOwner;
	Float2 offset;
	Float2 size;
	int dontHitFrame;
};

// ２つのコライダーが衝突しているか判定結果を返す関数
COLLIDER_RESULT Collider_Test(CCollider* pThis, CCollider* pOther);
bool Collider_Test_Bool(CCollider* pThis, CCollider* pOther);
