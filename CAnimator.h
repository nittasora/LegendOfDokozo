#pragma once

#include "Information.h"
#include "GameTimer.h"

#define ANIMATION_LOOP  -1
#define ANIMATION_END   -2

enum ANIMTYPE
{
	ANIM_NONE,
	ANIM_PLAYER,
	ANIM_PLATTACKEFFECT,
	ANIM_ENEMY,
	ANIM_ITEM
};

class CGameObject;
class CAnimator;

// アニメーションのスーパークラス
class CAnim
{
public:
	virtual void Init();
	virtual void Start();
	virtual void Update();

	CAnimator *animator;
private:
};

class Anim_None : public CAnim
{
public:
	void Update();

};

class Anim_Player : public CAnim
{
public:
	void Update();

private:
	int stepAnim[PLAYER_STEP_MAX] = {
		 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, ANIMATION_LOOP
	};
	int framCount = 0;
};

class Anim_PlAttackEffect : public CAnim
{
public:
	void Start();
	void Update();

private:
	int animFrame[PLAYER_ATTACK_EFFECT_STEP_MAX] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ANIMATION_END
	};
	int framCount = 0;
};

class Anim_Enemy : public CAnim
{
public:
	void Init();
	void Update();
	void Start();

private:
	void CommonAnim();

	int stepAnim[ENEMY_STEP_MAX] = {
		 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, ANIMATION_LOOP
	};
	int destroyAnim[ENEMY_DESTROY_MAX] = {
		 SOUTH, SOUTH_WEST, WEST, NORTH_WEST, NORTH, NORTH_EAST, EAST, SOUTH_EAST, ANIMATION_LOOP
	};
	int framCount = 0;
	int subFramCount = 0;
	float subTime;
};

class Anim_ITEM : public CAnim
{
public:
	void Init();
	void Update();

private:
	void CommonAnim();

	int stepAnim[ENEMY_STEP_MAX] = {
		 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, ANIMATION_LOOP
	};
	int framCount = 0;
	float subTime;
};

class CAnimator
{
public:
	bool Init(int);
	void UnInit();

	int frameX;
	int frameY;
	Float2 pos;
	bool isActive;
	bool isInit = false;
	CAnim *anim;
	CGameObject *pOwner;
	float time;
	float speed;

protected:
};

