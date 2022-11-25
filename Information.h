#pragma once

#include <Windows.h>
#include <random>
#include "MYMath.h"
#include "direct3d.h"
#include "XAudio2.h"

#define PI 3.141592f       // 円周率

#define	SCREEN_WIDTH	(960)					// ウインドウの幅
#define	SCREEN_HEIGHT	(768)					// ウインドウの高さ

#define STRING_MAX_LEN 256

#define MAX_SPRITE  256
// ポリゴン２つで１つの四角形（スプライト）。ポリゴンは３頂点なので、１スプライトは６頂点。
#define VERTEX_PER_SPRITE  (3*2)
#define VERTEX_BUFFER_SIZE  (MAX_SPRITE*sizeof(VERTEX_POSTEX)*VERTEX_PER_SPRITE)

// ゲームモードの数
#define GAME_MODE_NUM 2

enum GAME_MODE
{
	GAME_MODE_TITLE,
	GAME_MODE_PLAY,
	//	GAME_MODE_RESULT
};

enum DIRECTION
{
	SOUTH,
	WEST,
	EAST,
	NORTH,
	SOUTH_WEST,
	SOUTH_EAST,
	NORTH_WEST,
	NORTH_EAST
};

#define OBJECT_FLG_NUM 4

#define OBJECT_FLG_NONE 0x00

#define OBJECT_FLG_KNOCKBACK 0x01
#define OBJECT_FLG_AVOID 0x02
#define OBJECT_FLG_STRONGATTACK 0x04
#define OBJECT_FLG_ATTACK 0x08
#define OBJECT_FLG_FALL 0x40

#define OBJECT_FLG_WALK 0x04
#define OBJECT_FLG_RUN 0x08
#define OBJECT_FLG_IDOL 0x10
#define OBJECT_FLG_FIRE 0x20

#define OBJECT_FLG_CLOSE_NOW 0x02
#define OBJECT_FLG_CLOSE 0x04
#define OBJECT_FLG_OPEN_NOW 0x08
#define OBJECT_FLG_OPEN 0x10
#define OBJECT_FLG_CLOSE_START 0x20

#define OBJECT_FLG_MOVE_START 0x02
#define OBJECT_FLG_WIDTH_MOVE 0x04
#define OBJECT_FLG_HEIGHT_MOVE 0x08
#define OBJECT_FLG_MOVE_END 0x10

#define OBJECT_FLG_GET 0x01

#define GAME_ORDER_NONE					0x00
#define GAME_ORDER_CONTINUE				0x01
#define GAME_ORDER_MODECHANGE_TITLE		0x02
#define GAME_ORDER_MODECHANGE_PLAY		0x04
#define GAME_ORDER_MODECHANGE_RESULT	0x08

#define KEY_MAX 99
#define GOLD_MAX 999

#define MAP_NUM 4

#define MAP_ONE_SIZE_X (15)
#define MAP_ONE_SIZE_Y (10)
#define MAP_SIZE_X (MAP_ONE_SIZE_X * 16)
#define MAP_SIZE_Y (MAP_ONE_SIZE_Y * 8)
#define MAP_CHIP_SIZE 128.0f
#define MAPCHIP_PIC_ALL_SIZE 1024.0f
#define MAPCHIP_PIC_ONE_SIZE 32.0f
#define MAPVHIP_UV_SIZE (MAPCHIP_PIC_ONE_SIZE / MAPCHIP_PIC_ALL_SIZE)
#define MAPCHIP_NUM ((int)MAPCHIP_PIC_ALL_SIZE / (int)MAPCHIP_PIC_ONE_SIZE)

#define SLIDENUM 4

#define PLAYER_STEP_MAX 33

#define PLAYER_ANIM_STAND_SPEED 10
#define PLAYER_ANIM_WALK_SPEED 20
#define PLAYER_ANIM_RUN_SPEED 40

#define PLAYER_ATTACK_EFFECT_STEP_MAX 11

#define PLAYER_PARAME_MAX 100.0f

#define ATTACK_0_PIC_ALL_SIZE_X 1024.0f
#define ATTACK_0_PIC_ONE_SIZE_X (ATTACK_0_PIC_ALL_SIZE_X / 4.0f)
#define ATTACK_0_UV_SIZE_X (ATTACK_0_PIC_ONE_SIZE_X / ATTACK_0_PIC_ALL_SIZE_X)
#define ATTACK_0_PIC_ALL_SIZE_Y 768.0f
#define ATTACK_0_PIC_ONE_SIZE_Y (ATTACK_0_PIC_ALL_SIZE_Y / 3.0f)
#define ATTACK_0_UV_SIZE_Y (ATTACK_0_PIC_ONE_SIZE_Y / ATTACK_0_PIC_ALL_SIZE_Y)


#define ENEMY_STEP_MAX PLAYER_STEP_MAX
#define ENEMY_DESTROY_MAX 9

#define ENEMY_ANIM_STAND_SPEED 10
#define ENEMY_ANIM_WALK_SPEED 20
#define ENEMY_ANIM_RUN_SPEED 40

#define ENEMY_ANIM_DESTROY_ROTATION_TIME 8

#define ENEMY_MAX 10
#define ENEMY_PIC_ALL_SIZE_X 1104.0f
#define ENEMY_PIC_ALL_SIZE_Y 720.0f
#define ENEMY_PIC_TYPE_1_SIZE_X 120.0f
#define ENEMY_PIC_TYPE_1_SIZE_Y 72.0f
#define ENEMY_PIC_TYPE_2_SIZE_X 120.0f
#define ENEMY_PIC_TYPE_2_SIZE_Y 112.0f
#define ENEMY_PIC_TYPE_3_SIZE_X 144.0f
#define ENEMY_PIC_TYPE_3_SIZE_Y 120.0f
#define ENEMY_PIC_TYPE_4_SIZE_X 132.0f
#define ENEMY_PIC_TYPE_4_SIZE_Y 104.0f
#define ENEMY_PIC_TYPE_5_SIZE_X 228.0f
#define ENEMY_PIC_TYPE_5_SIZE_Y 120.0f

#define ITEM_STEP_MAX PLAYER_STEP_MAX

#define ITEM_ANIM_STAND_SPEED 40

#define ITEM_MAX 10
// 規格が違うので注意
#define ITEM_PIC_TYPE_1_SIZE_X 20.0f
#define ITEM_PIC_TYPE_1_SIZE_Y 28.0f
#define ITEM_PIC_TYPE_2_SIZE_X 32.0f
#define ITEM_PIC_TYPE_2_SIZE_Y 32.0f
#define ITEM_PIC_TYPE_3_SIZE_X 16.0f
#define ITEM_PIC_TYPE_3_SIZE_Y 18.0f
#define ITEM_PIC_TYPE_4_SIZE_X 48.0f
#define ITEM_PIC_TYPE_4_SIZE_Y 50.0f

// 頂点１つあたりを表す構造体
struct VERTEX_POSTEX {
	float x, y, z;  // 頂点の位置

	float u, v;  // テクスチャのUV座標
};

struct FRECT
{
	float left;
	float right;
	float top;
	float bottom;
};

struct FHP
{
	float now;
	float max;
	float protectKnockBack;
	int protectStop;
};

struct FSP
{
	float now;
	float max;
};

struct FATTACK
{
	float power;
	int stopFrame;
	float knockBack;
	int dontHitFrame;
};

struct ENEMYDATA
{
	int tag[ENEMY_MAX];
	Float2 position[ENEMY_MAX];
};

typedef ENEMYDATA ITEMDATA;