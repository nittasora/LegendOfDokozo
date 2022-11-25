#pragma once

#include "Information.h"
#include "CAnimator.h"
#include "CCharaController.h"
#include "CCollider.h"
#include "CGame.h"

enum FLIP
{
	FLIP_NONE,
	FLIP_VERTICAL,
	FLIP_HORIZONTAL
};

enum OBJECTTAG
{
	// *** 背景 ***
	TAG_BACKGROUND,

	// *** プレイヤー ***
	TAG_PLAYER,
	//TAG_PLAYER_CAN_WALK_WATER,
	TAG_PLAYERATTACK,

	// *** 旧敵 ***
	//TAG_ENEMY,

	// *** マップチップ種別 ***
	// 床
	TAG_FLOOR,
	// 壁
	TAG_WALL,
	TAG_WALL_HP_1,
	TAG_WALL_HP_2,
	// 壁
	TAG_INVISIBLE_WALL,
	// 水
	TAG_WATER,
	// 1マップ上がる
	TAG_FLOOR_UP_1,
	// 2マップ上がる
	TAG_FLOOR_UP_2,
	// 1マップ下がる
	TAG_FLOOR_DOWN_1,
	// 2マップ下がる
	TAG_FLOOR_DOWN_2,

	// *** 敵種別 ***
	// 敵無し
	TAG_ENEMY_NULL,
	// 敵やっつけ
	TAG_ENEMY_DESTROY,
	TAG_ENEMY_SPIRIT_1,
	TAG_ENEMY_SPIRIT_2,
	TAG_ENEMY_SPIRIT_3,
	TAG_ENEMY_SPIRIT_4,
	TAG_ENEMY_SPIRIT_5,
	TAG_ENEMY_SPIRIT_6,
	TAG_ENEMY_ORC_1,
	TAG_ENEMY_ORC_2,
	TAG_ENEMY_PLANT_1,
	TAG_ENEMY_PLANT_2,
	TAG_ENEMY_PLANT_FLOWER,
	TAG_ENEMY_SKULL,
	TAG_ENEMY_GHOST_1,
	TAG_ENEMY_GHOST_2,
	TAG_ENEMY_EYE_1,
	TAG_ENEMY_EYE_2,
	TAG_ENEMY_SHADOW,
	TAG_ENEMY_LIZARD_1,
	TAG_ENEMY_LIZARD_2,
	TAG_ENEMY_DEATH,
	TAG_ENEMY_GOLEM_1,
	TAG_ENEMY_GOLEM_2,
	TAG_ENEMY_DRAGON_1,
	TAG_ENEMY_DRAGON_2,
	TAG_ENEMY_DRAGON_3,
	TAG_ENEMY_ME,
	TAG_ITEM_NULL,
	TAG_ITEM_HEART,
	TAG_ITEM_COIN_1,
	TAG_ITEM_COIN_2,
	TAG_ITEM_KEY,
	TAG_ITEM_STATES_UP,
	TAG_ITEM_DOOR_KEY,
	TAG_ITEM_DOOR_LOCK,
	TAG_ITEM_FLOOR_KEY,
	TAG_ITEM_FLOOR_STATES_UP,
	TAG_ITEM_DOOR_CRYSTAL_1,
	TAG_ITEM_DOOR_CRYSTAL_2,
	TAG_ITEM_DOOR_CRYSTAL_3,
	TAG_ITEM_DOOR_CRYSTAL_4,
	TAG_ITEM_DOOR_CRYSTAL_5,
	TAG_ITEM_DOOR_CRYSTAL_6,
	TAG_ITEM_DOOR_CRYSTAL_7,
	TAG_ITEM_DOOR_CRYSTAL_8,
	TAG_ITEM_CRYSTAL_1,
	TAG_ITEM_CRYSTAL_2,
	TAG_ITEM_CRYSTAL_3,
	TAG_ITEM_CRYSTAL_4,
	TAG_ITEM_CRYSTAL_5,
	TAG_ITEM_CRYSTAL_6,
	TAG_ITEM_CRYSTAL_7,
	TAG_ITEM_CRYSTAL_8,
	TAG_ITEM_CRYSTAL_BIG,

	//マグマ
	TAG_MAGMA,
	//穴
	TAG_HOLE,
	//毒
	TAG_POISONOUS,

	TAG_ENEMY_MOMMY,
	TAG_ENEMY_SLIME,

	TAG_ITEM_HEART_SHOP,
	TAG_ITEM_KEY_SHOP,
	TAG_ITEM_STATES_UP_SHOP,
	TAG_ITEM_NECKLACE,
	TAG_ITEM_MOVE_BLOCK_1,
	TAG_ITEM_MOVE_BLOCK_2,
	TAG_ITEM_FLOOR_DOWN,
	TAG_ITEM_FLOOR_UP,

	TAG_FLOOR_UP_3,
	TAG_FLOOR_DOWN_3,

	TAG_ITEM_LASTANCHOR,
	TAG_ITEM_MUSICBOX_1,

};

struct FPOSINFO
{
	Float2 position;
	Float2 size;
	Float2 Scale;
};

struct FUVINFO
{
	Float2 offset;
	Float2 size;
	int flip;
};

struct FDATA_GAMEOBJECT;

class CGameObject	//ゲームオブジェクト
{
public:
	~CGameObject();
	void Init(FDATA_GAMEOBJECT);
	FRECT GameObject_GetXY();
	FRECT GameObject_GetUV();
	void SetUV(FUVINFO*);

	FPOSINFO transform;
	FUVINFO uv;
	CAnimator animator;
	CCharaController controller;
	CCollider collider;
	int flg;
	int tag;
	bool isActive = false;

	CGame *game;
	FHP hp;
	FSP sp;
	FATTACK attack;
	Float2 startPosition;

protected:
	Float2 localPosition;
	CGameObject* pOwner;
};

struct FDATA_GAMEOBJECT
{
	FPOSINFO transform;
	FUVINFO uv;
	int animType;
	int moveType;
	int colliderType;
	//int actionType;
	Float2 localPosition;
	CGameObject* pOwner;
	int tag;
	FHP hp;
	FSP sp;
	FATTACK attack;
};

