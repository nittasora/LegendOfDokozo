#pragma once

#include "Information.h"
#include "input.h"

enum {
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP
};

enum MOVETYPE
{
	MOVE_NONE,
	MOVE_BG_SCROLL_TOPDOWN,
	MOVE_PLAYER,
	MOVE_ENEMY,
	MOVE_ITEM
};

class CGameObject;
class CCharaController;

class CCharaMove
{
public:
	virtual void Init();
	virtual void Update();
	virtual void DeathAct();
	void SetKnockBack(CGameObject*);
	void CheckHP();
	void AddForce();
	void AddForce(const Float2*);
	void SlowDown(const float*);

	CCharaController *controller;
	int direction = SOUTH;
	int frameCount[OBJECT_FLG_NUM] = { 0, 0, 0, 0 };

private:

};

class CharaMove_None : public CCharaMove
{
public:
	void Update();
};

class CharaMove_BackGround_TopDown : public CCharaMove
{
public:
	void Update();
};

class CharaMove_Player : public CCharaMove
{
public:
	void Update();
	void DeathAct();

	Float2 avoidSpeed = Float2zero;
	float incSp = 0.0f;
};

class CharaMove_Enemy : public CCharaMove
{
public:
	void Init();
	void Update();
	void DeathAct();


	int frame;
	int moveFrame;
private:
	void DirCheck();
	void DirCheck_Player();
	void DirCheck_Roll();
	void SpeedCheck();
	void SpeedCheck(float);
};

class CharaMove_Item : public CCharaMove
{
public:
	void Init();
	void Update();
	void DeathAct();

private:
	int frame;
	int moveFrame;
};

class CCharaController
{
public:
	bool Init(int);
	void UnInit();

	bool isActive;
	bool isInit = false;
	CGameObject *pOwner;
	CCharaMove *pMove;
	Float2 moveSpeed;
	Float2 dirSpeed;
	float maxMoveSpeed;
	float accelForce;
	float stopForce;
};

