#pragma once

class CGame;

#include "Information.h"
#include "input.h"
#include "GameTimer.h"
#include "CObjectGenerator.h"
#include "CDraw.h"
#include "CGameObject.h"

enum PLAY_SCENE
{
	PLAY_NORMAL,
	PLAY_SLIDE,
	PLAY_CHANGMAP,
	PLAY_GAMEOVER,
	PLAY_ITEM_GET,
	PLAY_CRYSTAL_GET,
	PLAY_GAMECLEAR,
};

enum DLIDEDIRE
{
	SLIDE_LEFT,
	SLIDE_RIGHT,
	SLIDE_TOP,
	SLIDE_BOTTOM,
};

enum FADE_STATES
{
	FADEIN,
	FADEOUT,
	FADENOW,
	FADEEND,
};

enum SAVE_SITUATION
{
	NO_SAVE,
	FACE_SAVE,
	FACE_CLEAR,
	BACK_SAVE,
	BACK_CLEAR
};

enum WORLD_Z
{
	FACE_WORLD,
	BACK_WORLD,
	WORLD_MAX
};

class CMode_Base
{
public:
	virtual bool Init();
	virtual int Update();
	virtual void UnInit();

	void SethWnd(const HWND*);
	void SetCGame(CGame*);

protected:
	HWND hWnd;
	int drawCount;
	CGame *game;
};

class CTitle : public CMode_Base
{
public:
	bool Init();
	int Update();
	void UnInit();

private:
	void Draw();

	int flg;

	Float2 speed;

	CGameObject *backGround[3];
	CGameObject *logo[2];
	CDraw *titleDraw;
	CDraw *sysDraw;
};

class CPlay : public CMode_Base
{
public:
	bool Init();
	int Update();
	void UnInit();

	// シーン制御関係
	int scene;
	int fade;
	int fadeCount;

	int haveKeyNum;
	int haveGoldNum;
	int haveCrystal[8];

	// マップの種類関係
	int mapPointX;
	int mapPointY;
	int mapPointZ;
	int mapLastPointX;
	int mapLastPointY;
	int mapLastPointZ;

	bool canWalkWater;

	int worldPointZ;

	XA_BGM *bgm;

	// ゲームオーバー関係
	CGameObject *backGround;
	CGameObject *gameOverText;

	// ゲームクリア関係
	CGameObject *gameClearBackGround[2];
	CGameObject *gameClearText[12];
	CDraw *gameClearDraw;

	// 所持数関係
	CGameObject *keyNumberObj[2];
	CGameObject *coinNumberObj[3];

	// フェード関係
	CGameObject *fadeObj;
	CDraw *fadeDraw;

	// システム関係
	CGameObject *sysMainObj;
	CGameObject *sysMapPoint;
	CDraw *sysDraw;

	// マップ関係変数
	int mapChipID[MAP_NUM][MAP_SIZE_X][MAP_SIZE_Y];
	int mapChipHitID[MAP_NUM][MAP_SIZE_X][MAP_SIZE_Y];
	Float2 mapAnchor[MAP_NUM][MAP_SIZE_X / MAP_ONE_SIZE_X][MAP_SIZE_Y / MAP_ONE_SIZE_Y];
	CGameObject *mapChip[MAP_SIZE_X][MAP_SIZE_Y];
	CDraw *mapChip_Draw;
	Float2 mapCamera;

	// 見えない壁(プレイヤー以外用)
	CGameObject *invisibleWall[4];

	// プレイヤー関係
	CGameObject *player;
	CDraw *player_Draw;
	CGameObject *plAttackEffect;
	CGameObject *lifeGage;
	CGameObject *staminaGage;
	CGameObject *lifeGageFrame;
	CGameObject *staminaGageFrame;
	CDraw *plAttackEffect_Draw;

	// 敵関係
	int nowEnemyGroup;
	ENEMYDATA enemyData[MAP_NUM][MAP_SIZE_X / MAP_ONE_SIZE_X][MAP_SIZE_Y / MAP_ONE_SIZE_Y];
	CGameObject *enemy[2][ENEMY_MAX];
	CDraw *enemy_and_item_Draw;

	// 敵関係
	int nowItemGroup;
	ITEMDATA itemData[MAP_NUM][MAP_SIZE_X / MAP_ONE_SIZE_X][MAP_SIZE_Y / MAP_ONE_SIZE_Y];
	CGameObject *item[2][ITEM_MAX];

	// 最後にゲットしたアイテム
	CGameObject *getItem;

	// 処理を無視するフレーム数を指定
	void SetStop(int);
	void UpdataItem();
	void SetAnchor(int);
	void saveSaveData(int);
	void saveItem();

private:
	void CheckAndLoadSaveData();
	void InitItem();

	// マップ関係関数
	void LoadData();
	void SetMap(int);
	void SetEnemy();
	void SetItem();

	void NormalHitTest();
	void NormalDraw();

	bool NowContinue();

	void SlideMap();
	void SlideDraw();

	void GameOver();
	void GameOverDraw();

	void GameClear();
	void GameClearDraw();

	void ItemGet();
	void CrystalGet();

	int gameOrder;

	int stopFrame;
	int nextStopFrame;
	int frame;
	int slideDire;
};

class CGame
{
public:
	CGame(const HWND*);
	~CGame();

	bool Update();

	CMode_Base *pGameMode[GAME_MODE_NUM];
	CTitle *title;
	CPlay *play;

private:
	bool isInit;
	int gameMode;
	int gameOrder;
};

