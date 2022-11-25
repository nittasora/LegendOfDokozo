#include "CGame.h"
#include <stdio.h>

#define TITLE_FLG_FADEOUT 0
#define TITLE_FLG_WAIT 1
#define TITLE_FLG_FADEIN 2

#define GAMEOVER_STEP 21
#define GAMEOVER_DESTROY_WAIT GAMEOVER_STEP + 20
#define GAMEOVER_SE GAMEOVER_STEP + 60
#define GAMEOVER_SE_WAIT GAMEOVER_SE + 660

#define GAMECLEAR_STEP 12
#define GAMECLEAR_DIRCOUNT 8
#define GAMECLEAR_ROTATE_LOOP 160
#define GAMECLEAR_ROTATE (GAMECLEAR_STEP + GAMECLEAR_ROTATE_LOOP)
#define GAMECLEAR_STOP_START 360
#define GAMECLEAR_STOP_END 1080
#define GAMECLEAR_CAMERA_MOVE (GAMECLEAR_STOP_END - GAMECLEAR_STOP_START)
#define GAMECLEAR_TEXT_FADE (GAMECLEAR_ROTATE + GAMEOVER_STEP + 20)
#define GAMECLEAR_END GAMECLEAR_STOP_END + 20

#define ITEMGET_STEP 12
#define ITEMGET_END 300

#pragma region // ゲームの基本形

bool CMode_Base::Init() {
	return false;
}

int CMode_Base::Update() {
	return 0;
}

void CMode_Base::UnInit() {

}

void CMode_Base::SethWnd(const HWND *other) {
	this->hWnd = *other;
}

void CMode_Base::SetCGame(CGame *other) {
	this->game = other;
}

#pragma endregion

#pragma region // ゲームのタイトル

bool CTitle::Init() {
	for (int index = 0; index < 3; index++) {
		this->backGround[index] = new CGameObject;
		this->backGround[index]->game = this->game;
		CObjGene::CreateTitleBack(this->backGround[index], index);
	}
	for (int index = 0; index < 2; index++) {
		this->logo[index] = new CGameObject;
		this->logo[index]->game = this->game;
	}
	CObjGene::CreateTitleText(this->logo[0], 0);
	CObjGene::CreateBackGround(this->logo[1], SYSTEM_TEXT, 0);
	this->titleDraw = new CDraw("assets/image/title.png", MAX_SPRITE);
	this->sysDraw = new CDraw("assets/image/systexture.png", MAX_SPRITE);

	CDraw::camera = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };

	this->flg = TITLE_FLG_FADEOUT;
	this->speed = MakeFloat2(0.0f, 25.0f);

	XA_Play(SOUND_LABEL_BGM006);

	return true;
}

int CTitle::Update() {
	int gameOrder = GAME_ORDER_CONTINUE;

	Input_Update();  // このゲームで使うキーの押下状態を調べて保存

	switch (this->flg)
	{
	case TITLE_FLG_FADEOUT:
		CDraw::camera.y += 25.0f;
		if (CDraw::camera.y >= SCREEN_HEIGHT * 9) {
			CDraw::camera.y = SCREEN_HEIGHT * 9;
			this->flg = TITLE_FLG_WAIT;
		}
		break;
	case TITLE_FLG_WAIT:
		if (Input_GetKeyTrigger('S')) {
			this->flg = TITLE_FLG_FADEIN;
			XA_Stop(SOUND_LABEL_BGM006);
			XA_Play(SOUND_LABEL_SE011);
		}
		break;
	case TITLE_FLG_FADEIN:
	default:
		CDraw::camera = CDraw::camera + this->speed;
		this->speed.y -= 5.0f;
		if (CDraw::camera.y <= SCREEN_HEIGHT) {
			CDraw::camera.y = SCREEN_HEIGHT;
			gameOrder = GAME_ORDER_MODECHANGE_PLAY;
			XA_Stop(SOUND_LABEL_BGM006);
			XA_Stop(SOUND_LABEL_SE011);
		}
		break;
	}

	this->Draw();

	return gameOrder;
}

void CTitle::UnInit() {
	for (int index = 0; index < 3; index++) {
		delete this->backGround[index];
	}
	for (int index = 0; index < 2; index++) {
		delete this->logo[index];
	}
	delete this->titleDraw;
	delete this->sysDraw;
}

void CTitle::Draw() {
	this->titleDraw->SetVertex(this->backGround[0]);
	this->titleDraw->SetVertex(this->logo[0], Float2zero);
	this->titleDraw->DrawAct();

	this->sysDraw->SetVertex(this->logo[1], Float2zero);
	this->sysDraw->DrawAct();

	this->titleDraw->SetVertex(this->backGround[1]);
	this->titleDraw->SetVertex(this->backGround[2]);
	this->titleDraw->DrawAct();

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

#pragma endregion

#pragma region // ゲームの遊び画面

bool CPlay::Init() {
	// マップチップとマップチップ用CDraw初期化
	this->mapChip_Draw = new CDraw("assets/image/back.png", MAX_SPRITE);
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			this->mapChip[x][y] = new CGameObject;
			this->mapChip[x][y]->game = this->game;
			CObjGene::CreateBackGround(this->mapChip[x][y], MAPCHIP, x + (y * MAP_SIZE_X));
		}
	}

	// プレイヤーとプレイヤー用CDraw初期化
	this->player = new CGameObject;
	this->player->game = this->game;
	CObjGene::CreatePlayer(this->player, 64.0f + 128.0f * 2, 94.0f + 128.0f * 2);
	this->player_Draw = new CDraw("assets/image/character_0.png", 1);

	// プレイヤーのエフェクト関係初期化
	this->plAttackEffect = new CGameObject;
	this->plAttackEffect->game = this->game;
	CObjGene::CreatePLAtackEffect(this->plAttackEffect, this->player);
	this->lifeGage = new CGameObject;
	this->lifeGage->game = this->game;
	CObjGene::CreateBackGround(this->lifeGage, GAGE, 0);
	this->lifeGage->transform.position = MakeFloat2(SCREEN_WIDTH - 568.0f, SCREEN_HEIGHT - 96.0f);
	this->staminaGage = new CGameObject;
	this->staminaGage->game = this->game;
	CObjGene::CreateBackGround(this->staminaGage, GAGE, 1);
	this->staminaGage->transform.position = MakeFloat2(SCREEN_WIDTH - 186.0f, SCREEN_HEIGHT - 96.0f);
	this->lifeGageFrame = new CGameObject;
	this->lifeGageFrame->game = this->game;
	CObjGene::CreateBackGround(this->lifeGageFrame, GAGE, 2);
	this->lifeGageFrame->transform.position = MakeFloat2(SCREEN_WIDTH - 568.0f, SCREEN_HEIGHT - 96.0f);
	this->staminaGageFrame = new CGameObject;
	this->staminaGageFrame->game = this->game;
	CObjGene::CreateBackGround(this->staminaGageFrame, GAGE, 2);
	this->staminaGageFrame->transform.position = MakeFloat2(SCREEN_WIDTH - 186.0f, SCREEN_HEIGHT - 96.0f);
	this->plAttackEffect_Draw = new CDraw("assets/image/attack_0.png", 10);

	// システム関係初期化
	this->sysMainObj = new CGameObject;
	this->sysMainObj->game = this->game;
	CObjGene::CreateSysObj(this->sysMainObj);
	this->sysMapPoint = new CGameObject;
	this->sysMapPoint->game = this->game;
	CObjGene::CreateSysMapPoint(this->sysMapPoint);
	this->sysDraw = new CDraw("assets/image/systexture.png", MAX_SPRITE);

	// フェード関係初期化
	this->fadeObj = new CGameObject;
	this->fadeObj->game = this->game;
	CObjGene::CreateFade(this->fadeObj);
	this->fadeDraw = new CDraw("assets/image/fade.png", 2);

	const Float2 setPos[4] = {
		{0, -(SCREEN_HEIGHT * 2)},
		{0, SCREEN_HEIGHT * 2},
		{-(SCREEN_WIDTH * 2), 0},
		{SCREEN_WIDTH * 2, 0},
	};

	// 見えない壁関係初期化
	for (int index = 0; index < 4; index++) {
		this->invisibleWall[index] = new CGameObject;
		this->invisibleWall[index]->game = this->game;
		CObjGene::CreateInvisibleObject(this->invisibleWall[index], setPos[index].x, setPos[index].y, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
	}

	this->backGround = new CGameObject;
	this->backGround->game = this->game;
	CObjGene::CreateBackGround(this->backGround, GAMEOVER_BACKGROUND, 0);

	this->gameOverText = new CGameObject;
	this->gameOverText->game = this->game;
	CObjGene::CreateBackGround(this->gameOverText, GAMEOVER_TEXT, 0);

	// 敵関係初期化 + 空の敵を作る
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < ENEMY_MAX; y++) {
			this->enemy[x][y] = new CGameObject;
			this->enemy[x][y]->game = this->game;
			CObjGene::CreateNullEnemy(this->enemy[x][y]);
		}
	}
	// アイテム関係初期化 + 空の敵を作る
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < ITEM_MAX; y++) {
			this->item[x][y] = new CGameObject;
			this->item[x][y]->game = this->game;
			CObjGene::CreateNullItem(this->item[x][y]);
		}
	}
	this->enemy_and_item_Draw = new CDraw("assets/image/enemy.png", (ENEMY_MAX * 2) + (ITEM_MAX * 2));

	//
	for (int index = 0; index < 2; index++) {
		this->gameClearBackGround[index] = new CGameObject;
		CObjGene::CreateBackGround(this->gameClearBackGround[index], GAMECLEAR_BG, index);
	}
	for (int index = 0; index < 12; index++) {
		this->gameClearText[index] = new CGameObject;
		CObjGene::CreateBackGround(this->gameClearText[index], SYSTEM_TEXT, index);
	}
	this->gameClearDraw = new CDraw("assets/image/systexture.png", 14);
	this->gameClearText[8]->transform.position.y = -136.0f;
	this->gameClearText[9]->transform.position.y = -228.0f;
	this->gameClearText[8]->transform.Scale.y = 0.0f;
	this->gameClearText[9]->transform.Scale.y = 0.0f;
	this->gameClearText[1]->transform.position.y = -SCREEN_HEIGHT - 80.0f;
	this->gameClearText[2]->transform.position.y = -SCREEN_HEIGHT - 280.0f;
	this->gameClearText[3]->transform.position.y = -SCREEN_HEIGHT - 580.0f;
	this->gameClearText[4]->transform.position.y = -SCREEN_HEIGHT - 840.0f;
	this->gameClearText[5]->transform.position.y = -SCREEN_HEIGHT - 1032.0f;
	this->gameClearText[6]->transform.position.y = -SCREEN_HEIGHT - 1240.0f;
	this->gameClearText[7]->transform.position.y = -SCREEN_HEIGHT - 1432.0f;
	this->gameClearText[0]->transform.position.y = -(SCREEN_HEIGHT * 2) - 1432.0f + 180.0f;
	this->gameClearText[10]->transform.position.y = -(SCREEN_HEIGHT * 2) - 1432.0f - 312.0f;
	this->gameClearText[11]->transform.position.y = -(SCREEN_HEIGHT * 2) - 1432.0f - 408.0f;

	for (int index = 0; index < 2; index++) {
		this->keyNumberObj[index] = new CGameObject;
		CObjGene::CreateNumber(this->keyNumberObj[index]);
		this->keyNumberObj[index]->transform.position.x = 336.5f + (32.0f * index);
		this->keyNumberObj[index]->transform.position.y = 554.5f;
	}
	for (int index = 0; index < 3; index++) {
		this->coinNumberObj[index] = new CGameObject;
		CObjGene::CreateNumber(this->coinNumberObj[index]);
		this->coinNumberObj[index]->transform.position.x = 504.5f + (32.0f * index);
		this->coinNumberObj[index]->transform.position.y = 554.5f;
	}

	// データ読み込み
	this->CheckAndLoadSaveData();
	this->LoadData();

	this->stopFrame = 0;
	this->nextStopFrame = 0;
	this->scene = PLAY_CHANGMAP;

	this->fade = FADENOW;
	this->fadeCount = 0;
	this->frame = 0;

	CDraw::camera = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
	this->mapCamera = { 0, 0 };

	this->nowEnemyGroup = 0;
	this->nowItemGroup = 0;

	this->keyNumberObj[0]->animator.frameX = (this->haveKeyNum % 100) / 10;
	this->keyNumberObj[1]->animator.frameX = (this->haveKeyNum % 10) / 1;
	this->coinNumberObj[0]->animator.frameX = (this->haveGoldNum % 1000) / 100;
	this->coinNumberObj[1]->animator.frameX = (this->haveGoldNum % 100) / 10;
	this->coinNumberObj[2]->animator.frameX = (this->haveGoldNum % 10) / 1;

	this->bgm = new XA_BGM(SOUND_LABEL_BGM005);

	return true;
}

int CPlay::Update() {
	this->gameOrder = GAME_ORDER_CONTINUE;

	if (this->NowContinue()) {

		switch (this->fade)
		{
		case FADEIN:
			if (this->fadeCount < 64) {
				this->fadeObj->transform.position.x += (SCREEN_WIDTH * 4) / 64;
				this->fadeCount++;
			}
			else {
				this->fadeCount = 0;
				this->fade = FADEEND;
			}
			this->NormalDraw();
			break;
		case FADEOUT:
			if (this->fadeCount < 64) {
				this->fadeObj->transform.position.x -= (SCREEN_WIDTH * 4) / 64;
				this->fadeCount++;
			}
			else {
				this->fadeCount = 0;
				this->fade = FADENOW;
			}
			if (this->scene == PLAY_GAMEOVER) {
				this->GameOverDraw();
			}
			else if (this->scene == PLAY_GAMECLEAR) {
				this->GameClearDraw();
			}
			else {
				this->NormalDraw();
			}
			break;
		case FADENOW:
		case FADEEND:
		default:

			Input_Update();  // このゲームで使うキーの押下状態を調べて保存

			CDraw::Update();

			switch (this->scene)
			{
			case PLAY_CHANGMAP:
				this->SetMap(this->mapPointZ);
				this->SetAnchor(this->mapPointZ);
				this->nowEnemyGroup = (this->nowEnemyGroup + 1) % 2;
				this->SetEnemy();
				this->nowItemGroup = (this->nowItemGroup + 1) % 2;
				this->SetItem();
				this->player->startPosition = this->player->transform.position;
				this->player->collider.dontHitFrame = 0;
				this->player->transform.Scale = MakeFloat2(1.0f, 1.0f);
				this->player->controller.isActive = true;
				this->player->collider.isActive = true;
				this->player->flg &= ~OBJECT_FLG_FALL;
				this->sysMapPoint->transform.position.x = -SCREEN_WIDTH + 40.0f + (this->mapPointX * 20);
				this->sysMapPoint->transform.position.y = SCREEN_HEIGHT - 180.0f + (this->mapPointY * 20);

				if (this->mapPointZ == 0) {
					this->bgm->XA_Play_BGM(SOUND_LABEL_BGM005);
				}
				else if (this->mapPointZ == 1) {
					this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
				}
				else if (this->mapPointZ == 2) {
					this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
				}
				else if (this->mapPointZ == 3) {
					this->bgm->XA_Play_BGM(SOUND_LABEL_BGM001);
				}
				if (this->fade == FADENOW) {
					this->fade = FADEIN;
				}

				if (this->worldPointZ == FACE_WORLD) {
					this->saveSaveData(FACE_SAVE);
				}
				else {
					this->saveSaveData(BACK_SAVE);
				}

				this->saveItem();

				this->scene = PLAY_NORMAL;
				CDraw::camera = { (float)SCREEN_WIDTH + ((MAP_CHIP_SIZE * MAP_ONE_SIZE_X) * this->mapPointX), (float)SCREEN_HEIGHT + ((MAP_CHIP_SIZE * MAP_ONE_SIZE_Y) * this->mapPointY) };
				break;
			case PLAY_SLIDE:
				this->SlideMap();
				this->SlideDraw();
				break;
			case PLAY_GAMEOVER:
				if (this->fade == FADEEND) {
					this->GameOver();
					this->GameOverDraw();
				}
				else {
					this->gameOrder = GAME_ORDER_MODECHANGE_TITLE;
				}
				break;
			case PLAY_GAMECLEAR:
				if (this->fade == FADEEND) {
					this->GameClear();
					this->GameClearDraw();
				}
				else {
					if (this->worldPointZ == FACE_WORLD) {
						this->saveSaveData(FACE_CLEAR);
					}
					else {
						this->saveSaveData(BACK_CLEAR);
					}
					this->gameOrder = GAME_ORDER_MODECHANGE_TITLE;
				}
				break;
			case PLAY_ITEM_GET:
				this->ItemGet();
				this->NormalDraw();
				break;
			case PLAY_CRYSTAL_GET:
				this->CrystalGet();
				this->NormalDraw();
				break;
			case PLAY_NORMAL:
			default:
				this->plAttackEffect->collider.Update();
				this->player->collider.Update();

				for (int index = 0; index < ENEMY_MAX; index++) {
					this->enemy[this->nowEnemyGroup][index]->collider.Update();
					this->enemy[this->nowEnemyGroup][index]->controller.pMove->Update();
					this->enemy[this->nowEnemyGroup][index]->animator.anim->Update();
				}

				for (int index = 0; index < ITEM_MAX; index++) {
					this->item[this->nowItemGroup][index]->collider.Update();
					this->item[this->nowItemGroup][index]->controller.pMove->Update();
					this->item[this->nowItemGroup][index]->animator.anim->Update();
				}

				this->player->controller.pMove->Update();
				this->player->controller.pMove->AddForce();

				this->player->animator.anim->Update();

				this->plAttackEffect->animator.anim->Update();

				this->NormalHitTest();

				this->NormalDraw();

				for (int index = 0; index < ENEMY_MAX; index++) {
					this->enemy[this->nowEnemyGroup][index]->controller.pMove->CheckHP();
				}
				this->player->controller.pMove->CheckHP();
				break;
			}

			break;
		}

	}

	return this->gameOrder;
}

void CPlay::UnInit() {
	delete this->mapChip_Draw;
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			delete this->mapChip[x][y];
		}
	}

	delete this->player;
	delete this->player_Draw;

	delete this->plAttackEffect;
	delete this->lifeGage;
	delete this->staminaGage;
	delete this->lifeGageFrame;
	delete this->staminaGageFrame;
	delete this->plAttackEffect_Draw;

	delete this->sysMainObj;
	delete this->sysMapPoint;
	delete this->sysDraw;

	delete this->fadeObj;
	delete this->fadeDraw;

	delete this->backGround;
	delete this->gameOverText;

	for (int index = 0; index < 4; index++) {
		delete this->invisibleWall[index];
	}

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < ENEMY_MAX; y++) {
			delete this->enemy[x][y];
		}
	}
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < ITEM_MAX; y++) {
			delete this->item[x][y];
		}
	}
	delete this->enemy_and_item_Draw;

	for (int index = 0; index < 2; index++) {
		delete this->gameClearBackGround[index];
	}
	for (int index = 0; index < 12; index++) {
		delete this->gameClearText[index];
	}
	delete this->gameClearDraw;

	for (int index = 0; index < 2; index++) {
		delete this->keyNumberObj[index];
	}
	for (int index = 0; index < 3; index++) {
		delete this->coinNumberObj[index];
	}

	delete this->bgm;
}

void CPlay::CheckAndLoadSaveData() {
	char saveFilePath[STRING_MAX_LEN] = "save/save.csv";
	FILE *fp;
	bool saveEnable = true;
	bool createFace = true;

	fopen_s(&fp, saveFilePath, "r");

	if (fp != NULL) {
		// 一文字分の変数
		int c;
		// マップチップの種類
		int chipID;
		// 当たり判定があるかどうか
		int hitID;
		// 文字列格納用
		char buf[10];
		//
		int tmp[10];
		//
		int index = 0;

		// 文字列初期化
		memset(buf, 0, sizeof(buf));
		memset(tmp, 0, sizeof(tmp));

		for (int count = 0; count < 6; count++) {
			while (true) {
				c = fgetc(fp);

				// 区切り
				if (c == ',') {
					tmp[index] = atoi(buf);
					index++;
					memset(buf, 0, sizeof(buf));
					if (index >= 10) {
						saveEnable = false;
						break;
					}
				}
				// 次のチップへ
				else if (c == ';' || c == '\n') {
					tmp[index] = atoi(buf);
					memset(buf, 0, sizeof(buf));
					break;
				}
				else if (c == EOF) {
					saveEnable = false;
					break;
				}
				// 文字列連結
				else {
					strcat_s(buf, (const char*)&c);
				}
			}

			if (!saveEnable) {
				break;
			}

			switch (count)
			{
			case 0:
				switch (tmp[index])
				{
				case FACE_SAVE:
					this->worldPointZ = FACE_WORLD;
					break;
				case FACE_CLEAR:
					saveEnable = false;
					createFace = false;
					break;
				case BACK_SAVE:
					this->worldPointZ = BACK_WORLD;
					break;
				case NO_SAVE:
				case BACK_CLEAR:
				default:
					saveEnable = false;
					break;
				}
				break;
			case 1:
				this->player->hp.max = (float)tmp[index - 3];
				this->player->hp.now = (float)tmp[index - 2];
				this->player->sp.max = (float)tmp[index - 1];
				this->player->sp.now = (float)tmp[index];
				break;
			case 2:
				this->haveKeyNum = tmp[index - 1];
				this->haveGoldNum = tmp[index];
				break;
			case 3:
				this->mapPointX = tmp[index - 5];
				this->mapPointY = tmp[index - 4];
				this->mapPointZ = tmp[index - 3];
				this->mapLastPointX = tmp[index - 2];
				this->mapLastPointY = tmp[index - 1];
				this->mapLastPointZ = tmp[index];
				break;
			case 4:
				if (tmp[index] != 0) {
					this->canWalkWater = true;
				}
				else {
					this->canWalkWater = false;
				}
				break;
			case 5:
				for (int i = 0; i < 8; i++) {
					this->haveCrystal[i] = tmp[i];
				}
				break;
			}

			if (!saveEnable) {
				break;
			}

			memset(tmp, 0, sizeof(tmp));
			index = 0;
		}

		fclose(fp);
	}
	else {
		saveEnable = false;
	}

	if (!saveEnable) {
		this->player->hp.max = 15.0f;
		this->player->hp.now = this->player->hp.max;
		this->player->sp.max = 15.0f;
		this->player->sp.now = this->player->sp.max;

		this->haveKeyNum = 0;
		this->haveGoldNum = 0;

		// なかったことになりました
		if (createFace) {
			this->mapPointX = 0;
			this->mapPointY = 0;
			this->mapPointZ = 0;
			this->worldPointZ = FACE_WORLD;
		}
		else {
			this->mapPointX = 0;
			this->mapPointY = 0;
			this->mapPointZ = 0;
			this->worldPointZ = FACE_WORLD;
		}

		this->mapLastPointX = this->mapPointX;
		this->mapLastPointY = this->mapPointY;
		this->mapLastPointZ = this->mapPointZ;

		this->canWalkWater = false;

		for (int index = 0; index < 8; index++) {
			this->haveCrystal[index] = 0;
		}

		if (createFace) {
			this->saveSaveData(FACE_SAVE);
		}
		else {
			this->saveSaveData(BACK_SAVE);
		}

		this->InitItem();
	}

}

void CPlay::saveSaveData(int save_situation) {
	char saveFilePath[STRING_MAX_LEN] = "save/save.csv";
	FILE *fp;

	fopen_s(&fp, saveFilePath, "w");

	// 詰み防止
	if (this->player->hp.now < 1.0f) {
		this->player->hp.now = 1.0f;
	}

	fprintf_s(fp, "%d\n", save_situation);
	fprintf_s(fp, "%d,%d,%d,%d\n", (int)this->player->hp.max, (int)this->player->hp.now, (int)this->player->sp.max, (int)this->player->sp.now);
	fprintf_s(fp, "%d,%d\n", this->haveKeyNum, this->haveGoldNum);
	fprintf_s(fp, "%d,%d,%d,%d,%d,%d\n", this->mapPointX, this->mapPointY, this->mapPointZ, this->mapLastPointX, this->mapLastPointY, this->mapLastPointZ);
	if (this->canWalkWater) {
		fprintf_s(fp, "1\n");
	}
	else {
		fprintf_s(fp, "0\n");
	}
	for (int index = 0; index < 8; index++) {
		fprintf_s(fp, "%d", this->haveCrystal[index]);
		if (index < 7) {
			fprintf_s(fp, ",");
		}
		else {
			fprintf_s(fp, "\n");
		}
	}

	fclose(fp);
}

void CPlay::saveItem() {
	char saveItemFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	}, {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	} };
	FILE *fp;

	for (int z = 0; z < MAP_NUM; z++) {
		fopen_s(&fp, saveItemFilePath[this->worldPointZ][z], "w");

		for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
				for (int index = 0; index < ITEM_MAX; index++) {
					fprintf(fp, "%d,%d,%d\n", this->itemData[z][x][y].tag[index], (int)this->itemData[z][x][y].position[index].x, (int)this->itemData[z][x][y].position[index].y);
				}
			}
		}

		fclose(fp);
	}

}

void CPlay::InitItem() {
	char saveItemFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	}, {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	} };
	char itemFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"assets/item/item1.csv",
		"assets/item/item2.csv",
		"assets/item/item3.csv",
		"assets/item/item4.csv"
	}, {
		"assets/item/item1.csv",
		"assets/item/item2.csv",
		"assets/item/item3.csv",
		"assets/item/item4.csv"
	} };
	FILE *fp;


	for (int wz = 0; wz < 2; wz++) {
		for (int z = 0; z < MAP_NUM; z++) {

			fopen_s(&fp, itemFilePath[wz][z], "r");

			for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
				for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
					for (int index = 0; index < ITEM_MAX; index++) {
						if (fp != NULL) {
							// 一文字分の変数
							int c;
							// 文字列格納用
							char buf[10];

							// 文字列初期化
							memset(buf, 0, sizeof(buf));

							while (true) {
								c = fgetc(fp);

								if (c == ',' || c == '\n') {
									this->itemData[z][x][y].tag[index] = atoi(buf);
									memset(buf, 0, sizeof(buf));
									break;
								}
								// 文字列連結
								else {
									strcat_s(buf, (const char*)&c);
								}
							}

							while (true) {
								c = fgetc(fp);

								if (c == ',' || c == '\n') {
									this->itemData[z][x][y].position[index].x = (float)atoi(buf);
									memset(buf, 0, sizeof(buf));
									break;
								}
								// 文字列連結
								else {
									strcat_s(buf, (const char*)&c);
								}
							}

							while (true) {
								c = fgetc(fp);

								if (c == ',' || c == '\n') {
									this->itemData[z][x][y].position[index].y = (float)atoi(buf);
									memset(buf, 0, sizeof(buf));
									break;
								}
								// 文字列連結
								else {
									strcat_s(buf, (const char*)&c);
								}
							}

						}
						else {
							this->itemData[z][x][y].tag[index] = TAG_ITEM_NULL;
							this->itemData[z][x][y].position[index] = Float2zero;
						}
					}
				}
			}

			if (fp != NULL) {
				fclose(fp);
			}

			fopen_s(&fp, saveItemFilePath[wz][z], "w");

			for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
				for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
					for (int index = 0; index < ITEM_MAX; index++) {
						fprintf(fp, "%d,%d,%d\n", this->itemData[z][x][y].tag[index], (int)this->itemData[z][x][y].position[index].x, (int)this->itemData[z][x][y].position[index].y);
					}
				}
			}

			fclose(fp);
		}
	}

}

void CPlay::LoadData() {
	char mapFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"assets/map/map1.csv",
		"assets/map/map2.csv",
		"assets/map/map3.csv",
		"assets/map/map4.csv"
	}, {
		"assets/map/map1.csv",
		"assets/map/map2.csv",
		"assets/map/map3.csv",
		"assets/map/map4.csv"
	} };
	char anchorFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"assets/map/anchor1.csv",
		"assets/map/anchor2.csv",
		"assets/map/anchor3.csv",
		"assets/map/anchor4.csv"
	}, {
		"assets/map/anchor1.csv",
		"assets/map/anchor2.csv",
		"assets/map/anchor3.csv",
		"assets/map/anchor4.csv"
	} };
	char enemyFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"assets/enemy/enemy1.csv",
		"assets/enemy/enemy2.csv",
		"assets/enemy/enemy3.csv",
		"assets/enemy/enemy4.csv"
	}, {
		"assets/enemy/enemy1.csv",
		"assets/enemy/enemy2.csv",
		"assets/enemy/enemy3.csv",
		"assets/enemy/enemy4.csv"
	} };
	char itemFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"assets/item/item1.csv",
		"assets/item/item2.csv",
		"assets/item/item3.csv",
		"assets/item/item4.csv"
	}, {
		"assets/item/item1.csv",
		"assets/item/item2.csv",
		"assets/item/item3.csv",
		"assets/item/item4.csv"
	} };
	char saveItemFilePath[WORLD_MAX][MAP_NUM][STRING_MAX_LEN] = { {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	}, {
		"save/item1.csv",
		"save/item2.csv",
		"save/item3.csv",
		"save/item4.csv"
	} };
	FILE *fp;

	for (int z = 0; z < MAP_NUM; z++) {
		fopen_s(&fp, mapFilePath[this->worldPointZ][z], "r");

		if (fp != NULL) {
			// 一文字分の変数
			int c;
			// マップチップの種類
			int chipID;
			// 当たり判定があるかどうか
			int hitID;
			// 文字列格納用
			char buf[10];

			// 文字列初期化
			memset(buf, 0, sizeof(buf));

			for (int y = 0; y < MAP_SIZE_Y; y++) {
				for (int x = 0; x < MAP_SIZE_X; x++) {
					while (true) {
						c = fgetc(fp);

						// マップチップ用区切り
						if (c == ',') {
							chipID = atoi(buf);
							memset(buf, 0, sizeof(buf));
						}
						// 当たり判定用区切り又は開業 -> 次のチップへ
						else if (c == ';' || c == '\n') {
							hitID = atoi(buf);
							memset(buf, 0, sizeof(buf));
							break;
						}
						// 文字列連結
						else {
							strcat_s(buf, (const char*)&c);
						}
					}

					this->mapChipID[z][x][y] = chipID;
					this->mapChipHitID[z][x][y] = hitID;
				}
			}

			fclose(fp);
		}
		else {
			for (int y = 0; y < MAP_SIZE_Y; y++) {
				for (int x = 0; x < MAP_SIZE_X; x++) {
					if (y % 10 == 9 && x % 15 == 14) {
						this->mapChipID[z][x][y] = 1;
						this->mapChipHitID[z][x][y] = 1;
					}
					else if (y == MAP_SIZE_Y - 1 || y == 0) {
						this->mapChipID[z][x][y] = 1;
						this->mapChipHitID[z][x][y] = 1;
					}
					else if (x == MAP_SIZE_X - 1 || x == 0) {
						this->mapChipID[z][x][y] = 1;
						this->mapChipHitID[z][x][y] = 1;
					}
					else {
						this->mapChipID[z][x][y] = 0;
						this->mapChipHitID[z][x][y] = 0;
					}
				}
			}
		}

		fopen_s(&fp, anchorFilePath[this->worldPointZ][z], "r");

		for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
				if (fp != NULL) {
					// 一文字分の変数
					int c;
					// 文字列格納用
					char buf[10];

					int posX;
					int posY;

					// 文字列初期化
					memset(buf, 0, sizeof(buf));

					while (true) {
						c = fgetc(fp);

						// マップチップ用区切り
						if (c == ',') {
							posX = atoi(buf);
							memset(buf, 0, sizeof(buf));
						}
						// 当たり判定用区切り又は開業 -> 次のチップへ
						else if (c == ';' || c == '\n') {
							posY = atoi(buf);
							memset(buf, 0, sizeof(buf));
							break;
						}
						// 文字列連結
						else {
							strcat_s(buf, (const char*)&c);
						}
					}

					this->mapAnchor[z][x][y] = MakeFloat2((float)posX, (float)posY);
				}
				else {
					this->mapAnchor[z][x][y] = this->mapChip[x * MAP_ONE_SIZE_X][y * MAP_ONE_SIZE_Y]->transform.position;
				}
			}
		}

		if (fp != NULL) {
			fclose(fp);
		}

		fopen_s(&fp, enemyFilePath[this->worldPointZ][z], "r");

		for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
				for (int index = 0; index < ENEMY_MAX; index++) {
					if (fp != NULL) {
						// 一文字分の変数
						int c;
						// 文字列格納用
						char buf[10];

						// 文字列初期化
						memset(buf, 0, sizeof(buf));

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->enemyData[z][x][y].tag[index] = atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->enemyData[z][x][y].position[index].x = (float)atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->enemyData[z][x][y].position[index].y = (float)atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

					}
					else {
						this->enemyData[z][x][y].tag[index] = TAG_ENEMY_NULL;
						this->enemyData[z][x][y].position[index] = Float2zero;
					}
				}
			}
		}

		if (fp != NULL) {
			fclose(fp);
		}

		fopen_s(&fp, saveItemFilePath[this->worldPointZ][z], "r");

		if (fp == NULL) {
			fopen_s(&fp, itemFilePath[this->worldPointZ][z], "r");
		}

		for (int y = 0; y < MAP_SIZE_Y / MAP_ONE_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X / MAP_ONE_SIZE_X; x++) {
				for (int index = 0; index < ITEM_MAX; index++) {
					if (fp != NULL) {
						// 一文字分の変数
						int c;
						// 文字列格納用
						char buf[10];

						// 文字列初期化
						memset(buf, 0, sizeof(buf));

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->itemData[z][x][y].tag[index] = atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->itemData[z][x][y].position[index].x = (float)atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

						while (true) {
							c = fgetc(fp);

							if (c == ',' || c == '\n') {
								this->itemData[z][x][y].position[index].y = (float)atoi(buf);
								memset(buf, 0, sizeof(buf));
								break;
							}
							// 文字列連結
							else {
								strcat_s(buf, (const char*)&c);
							}
						}

					}
					else {
						this->itemData[z][x][y].tag[index] = TAG_ITEM_NULL;
						this->itemData[z][x][y].position[index] = Float2zero;
					}
				}
			}
		}

		if (fp != NULL) {
			fclose(fp);
		}

	}
}

void CPlay::SetMap(int mapIndex) {
	if (mapIndex >= 0 && mapIndex < MAP_NUM) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X; x++) {
				this->mapChip[x][y]->attack.dontHitFrame = 25;
				this->mapChip[x][y]->attack.power = 1.25;
				this->mapChip[x][y]->attack.stopFrame = 5;
				this->mapChip[x][y]->attack.knockBack = 0;
				this->mapChip[x][y]->animator.frameX = this->mapChipID[mapIndex][x][y] % MAPCHIP_NUM;
				this->mapChip[x][y]->animator.frameY = this->mapChipID[mapIndex][x][y] / MAPCHIP_NUM;
				if (this->mapChip[x][y]->animator.frameY == MAPCHIP_NUM - 2) {
					this->mapChip[x][y]->hp = { 4, 4 };
				}
				else if (this->mapChip[x][y]->animator.frameY == MAPCHIP_NUM - 3) {
					this->mapChip[x][y]->hp = { 1, 1 };
				}
				else if (this->mapChip[x][y]->animator.frameY == MAPCHIP_NUM - 5) {
					this->mapChip[x][y]->hp = { 9, 9 };
				}
				else if (this->mapChip[x][y]->animator.frameY == MAPCHIP_NUM - 6) {
					this->mapChip[x][y]->hp = { 1, 1 };
				}
				else {
					this->mapChip[x][y]->hp = { -1, -1 };
				}

				if (this->mapChipHitID[mapIndex][x][y] == 1) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_WALL;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 2) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_DOWN_1;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 3) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_DOWN_2;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 4) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_MAGMA;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 5) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_HOLE;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 6) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_UP_3;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 7) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_WATER;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 8) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_UP_1;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 9) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_UP_2;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 10) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_POISONOUS;
				}
				else if (this->mapChipHitID[mapIndex][x][y] == 11) {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR_DOWN_3;
				}
				else {
					this->mapChip[x][y]->collider.isActive = true;
					this->mapChip[x][y]->tag = TAG_FLOOR;
				}
			}
		}
	}
	else {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			for (int x = 0; x < MAP_SIZE_X; x++) {
				if (y % 10 == 9 && x % 15 == 14) {
					this->mapChip[x][y]->animator.frameX = 2;
					this->mapChip[x][y]->animator.frameY = 0;
					this->mapChip[x][y]->collider.isActive = true;
				}
				else if (y == MAP_SIZE_Y - 1 || y == 0) {
					this->mapChip[x][y]->animator.frameX = 1;
					this->mapChip[x][y]->animator.frameY = 0;
					this->mapChip[x][y]->collider.isActive = true;
				}
				else if (x == MAP_SIZE_X - 1 || x == 0) {
					this->mapChip[x][y]->animator.frameX = 1;
					this->mapChip[x][y]->animator.frameY = 0;
					this->mapChip[x][y]->collider.isActive = true;
				}
				else {
					this->mapChip[x][y]->animator.frameX = 0;
					this->mapChip[x][y]->animator.frameY = 0;
					this->mapChip[x][y]->collider.isActive = false;
				}
			}
		}
	}
}

void CPlay::SetAnchor(int mapIndex) {
	if (mapIndex >= 0 && mapIndex < MAP_NUM) {
		this->player->transform.position = this->mapAnchor[mapIndex][this->mapPointX][this->mapPointY] - this->player->collider.offset;
		this->player->controller.moveSpeed = Float2zero;
		this->player->controller.pMove->direction = SOUTH;
	}
}

void CPlay::SetEnemy() {
	for (int index = 0; index < ENEMY_MAX; index++) {
		CObjGene::SetEnemyParameter(this->enemy[this->nowEnemyGroup][index], this->enemyData[this->mapPointZ][this->mapPointX][this->mapPointY].tag[index], this->enemyData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].x, this->enemyData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].y);
	}
}

void CPlay::SetItem() {
	for (int index = 0; index < ITEM_MAX; index++) {
		CObjGene::SetItemParameter(this->item[this->nowItemGroup][index], this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].tag[index], this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].x, this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].y);
	}
}

void CPlay::UpdataItem() {
	for (int index = 0; index < ITEM_MAX; index++) {
		this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].tag[index] = this->item[this->nowItemGroup][index]->tag;
		this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].x = this->item[this->nowItemGroup][index]->transform.position.x;
		this->itemData[this->mapPointZ][this->mapPointX][this->mapPointY].position[index].y = this->item[this->nowItemGroup][index]->transform.position.y;
	}
}

void CPlay::NormalHitTest() {
	const Float2 setPos[4] = {
	{0, -(SCREEN_HEIGHT * 2)},
	{0, (SCREEN_HEIGHT - MAP_CHIP_SIZE) * 2},
	{-(SCREEN_WIDTH * 2), 0},
	{SCREEN_WIDTH * 2, 0},
	};

	for (int index = 0; index < 4; index++) {
		this->invisibleWall[index]->transform.position = CDraw::camera + setPos[index];
	}

	for (int index = 0; index < ITEM_MAX; index++) {
		COLLIDER_RESULT result = Collider_Test(&this->item[this->nowItemGroup][index]->collider, &this->player->collider);

		if (result.isHit) {
			this->player->collider.HitReaction_Play(this->item[this->nowItemGroup][index], &result);
		}
	}

	// プレイヤーと壁
	{
		Float2 float2 = this->player->transform.position;

		int chipPosX = (int)(float2.x / MAP_CHIP_SIZE);
		int chipPosY = (int)(float2.y / MAP_CHIP_SIZE);
		int startX = chipPosX - 6;
		int startY = chipPosY - 6;
		int endX = chipPosX + 7;
		int endY = chipPosY + 7;

		if (startX < 0) {
			startX = 0;
		}
		if (startY < 0) {
			startY = 0;
		}
		if (endX > MAP_SIZE_X) {
			endX = MAP_SIZE_X;
		}
		if (endY > MAP_SIZE_Y) {
			endY = MAP_SIZE_Y;
		}

		int px;
		int py;
		int addX;
		int addY;

		if (this->player->controller.moveSpeed.x > 0) {
			px = endX - 1;
			addX = -1;
		}
		else {
			px = startX;
			addX = 1;
		}

		if (this->player->controller.moveSpeed.y > 0) {
			py = startY;
			addY = 1;
		}
		else {
			py = endY - 1;
			addY = -1;
		}

		for (int x = px; (x >= startX) && (x < endX); x += addX) {
			for (int y = py; (y >= startY) && (y < endY); y += addY) {
				COLLIDER_RESULT result = Collider_Test(&this->player->collider, &this->mapChip[x][y]->collider);

				if (result.isHit) {
					this->mapChip[x][y]->collider.HitReaction_Play(this->player, &result);
				}
			}
		}
	}

	for (int index = 0; index < ENEMY_MAX; index++) {
		Float2 float2 = this->enemy[this->nowEnemyGroup][index]->transform.position;

		int chipPosX = (int)(float2.x / MAP_CHIP_SIZE);
		int chipPosY = (int)(float2.y / MAP_CHIP_SIZE);
		int startX = chipPosX - 6;
		int startY = chipPosY - 6;
		int endX = chipPosX + 7;
		int endY = chipPosY + 7;

		if (startX < 0) {
			startX = 0;
		}
		if (startY < 0) {
			startY = 0;
		}
		if (endX > MAP_SIZE_X) {
			endX = MAP_SIZE_X;
		}
		if (endY > MAP_SIZE_Y) {
			endY = MAP_SIZE_Y;
		}

		int px;
		int py;
		int addX;
		int addY;

		if (this->enemy[this->nowEnemyGroup][index]->controller.moveSpeed.x > 0) {
			px = endX - 1;
			addX = -1;
		}
		else {
			px = startX;
			addX = 1;
		}

		if (this->enemy[this->nowEnemyGroup][index]->controller.moveSpeed.y > 0) {
			py = startY;
			addY = 1;
		}
		else {
			py = endY - 1;
			addY = -1;
		}

		for (int indexItem = 0; indexItem < ITEM_MAX; indexItem++) {
			COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->item[this->nowItemGroup][indexItem]->collider);

			if (result.isHit) {
				this->item[this->nowItemGroup][indexItem]->collider.HitReaction_Play(this->enemy[this->nowEnemyGroup][index], &result);
			}
		}

		for (int x = px; (x >= startX) && (x < endX); x += addX) {
			for (int y = py; (y >= startY) && (y < endY); y += addY) {
				COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->mapChip[x][y]->collider);

				if (result.isHit) {
					this->mapChip[x][y]->collider.HitReaction_Play(this->enemy[this->nowEnemyGroup][index], &result);
				}
			}
		}

		for (int wall = 0; wall < 4; wall++) {
			COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->invisibleWall[wall]->collider);

			if (result.isHit) {
				this->invisibleWall[wall]->collider.HitReaction_Play(this->enemy[this->nowEnemyGroup][index], &result);
			}
		}

		COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->player->collider);

		if (result.isHit) {
			this->player->collider.HitReaction_Play(this->enemy[this->nowEnemyGroup][index], &result);
		}
	}

	if (this->plAttackEffect->isActive && this->plAttackEffect->collider.dontHitFrame < 1) {

		FRECT fRect = this->plAttackEffect->collider.Collider_GetXY();

		int startX = (int)(fRect.left / MAP_CHIP_SIZE);
		int startY = (int)(fRect.bottom / MAP_CHIP_SIZE);
		int endX = (int)(fRect.right / MAP_CHIP_SIZE);
		int endY = (int)(fRect.top / MAP_CHIP_SIZE);

		if (startX < 0) {
			startX = 0;
		}
		if (startY < 0) {
			startY = 0;
		}
		if (endX >= MAP_SIZE_X) {
			endX = MAP_SIZE_X - 1;
		}
		if (endY >= MAP_SIZE_Y) {
			endY = MAP_SIZE_Y - 1;
		}

		for (int x = startX; x <= endX; x++) {
			for (int y = startY; y <= endY; y++) {
				COLLIDER_RESULT result = Collider_Test(&this->plAttackEffect->collider, &this->mapChip[x][y]->collider);
				if (result.isHit) {
					this->mapChip[x][y]->collider.HitReaction_Play(this->plAttackEffect, &result);
				}
			}
		}

		for (int index = 0; index < ENEMY_MAX; index++) {
			COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->plAttackEffect->collider);

			if (result.isHit) {
				this->plAttackEffect->collider.HitReaction_Play(this->enemy[this->nowEnemyGroup][index], &result);
			}
		}
	}

	Float2 pPos = this->player->transform.position;
	Float2 cameraPos = CDraw::camera;

	if (pPos.x < cameraPos.x - SCREEN_WIDTH) {
		this->scene = PLAY_SLIDE;
		this->frame = 0;
		this->slideDire = SLIDE_LEFT;
	}
	else if (pPos.x > cameraPos.x + SCREEN_WIDTH) {
		this->scene = PLAY_SLIDE;
		this->frame = 0;
		this->slideDire = SLIDE_RIGHT;
	}
	else if (pPos.y < cameraPos.y - SCREEN_HEIGHT) {
		this->scene = PLAY_SLIDE;
		this->frame = 0;
		this->slideDire = SLIDE_BOTTOM;
	}
	else if (pPos.y > cameraPos.y + SCREEN_HEIGHT - (2 * MAP_CHIP_SIZE)) {
		this->scene = PLAY_SLIDE;
		this->frame = 0;
		this->slideDire = SLIDE_TOP;
	}
}

void CPlay::NormalDraw() {
	Float2 float2 = CDraw::camera;

	this->keyNumberObj[0]->animator.frameX = (this->haveKeyNum % 100) / 10;
	this->keyNumberObj[1]->animator.frameX = (this->haveKeyNum % 10) / 1;
	this->coinNumberObj[0]->animator.frameX = (this->haveGoldNum % 1000) / 100;
	this->coinNumberObj[1]->animator.frameX = (this->haveGoldNum % 100) / 10;
	this->coinNumberObj[2]->animator.frameX = (this->haveGoldNum % 10) / 1;

	int chipPosX = (int)(float2.x / MAP_CHIP_SIZE);
	int chipPosY = (int)(float2.y / MAP_CHIP_SIZE);
	int startX = chipPosX - 10;
	int startY = chipPosY - 6;
	int endX = chipPosX + 11;
	int endY = chipPosY + 7;

	if (startX < 0) {
		startX = 0;
	}
	if (startY < 0) {
		startY = 0;
	}
	if (endX > MAP_SIZE_X) {
		endX = MAP_SIZE_X;
	}
	if (endY > MAP_SIZE_Y) {
		endY = MAP_SIZE_Y;
	}

	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			this->mapChip_Draw->SetVertex(this->mapChip[x][y]);
		}
	}
	this->mapChip_Draw->DrawAct();

	for (int index = 0; index < ITEM_MAX; index++) {
		this->enemy_and_item_Draw->SetVertex(this->item[this->nowItemGroup][index]);
	}
	for (int index = 0; index < ENEMY_MAX; index++) {
		this->enemy_and_item_Draw->SetVertex(this->enemy[this->nowEnemyGroup][index]);
	}
	this->enemy_and_item_Draw->DrawAct();

	this->player_Draw->SetVertex(this->player);
	this->player_Draw->DrawAct();

	if (this->plAttackEffect->isActive) {
		this->plAttackEffect_Draw->SetVertex(this->plAttackEffect);
	}

	this->plAttackEffect_Draw->DrawAct();

	this->sysDraw->SetVertex(this->sysMainObj, Float2zero);
	this->sysDraw->SetVertex(this->sysMapPoint, Float2zero);
	for (int index = 0; index < 2; index++) {
		this->sysDraw->SetVertex(this->keyNumberObj[index], Float2zero);
	}
	for (int index = 0; index < 3; index++) {
		this->sysDraw->SetVertex(this->coinNumberObj[index], Float2zero);
	}
	this->sysDraw->DrawAct();

	this->staminaGage->transform.Scale.x = this->player->sp.now / this->player->sp.max;
	this->plAttackEffect_Draw->SetVertex(this->staminaGage, Float2zero);

	this->lifeGage->transform.Scale.x = this->player->hp.now / this->player->hp.max;
	this->plAttackEffect_Draw->SetVertex(this->lifeGage, Float2zero);

	this->plAttackEffect_Draw->SetVertex(this->lifeGageFrame, Float2zero);
	this->plAttackEffect_Draw->SetVertex(this->staminaGageFrame, Float2zero);

	this->plAttackEffect_Draw->DrawAct();

	this->fadeDraw->SetVertex(this->fadeObj, MakeFloat2(0.0f, SCREEN_HEIGHT));
	this->fadeDraw->DrawAct();

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

bool CPlay::NowContinue() {
	if (this->stopFrame < this->nextStopFrame) {
		this->stopFrame = this->nextStopFrame;
	}

	this->nextStopFrame = 0;

	if (this->stopFrame > 0) {
		this->stopFrame--;
		return false;
	}
	else {
		return true;
	}
}

void CPlay::SetStop(int frame) {
	if (this->nextStopFrame < frame) {
		this->nextStopFrame = frame;
	}
}

void CPlay::SlideMap() {

	this->sysMapPoint->transform.position.x = -SCREEN_WIDTH + 40.0f + (this->mapPointX * 20);
	this->sysMapPoint->transform.position.y = SCREEN_HEIGHT - 180.0f + (this->mapPointY * 20);

	switch (this->slideDire)
	{
	case SLIDE_LEFT:
		if (this->frame == 0) {
			if (this->mapPointZ == 0) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM005);
			}
			else if (this->mapPointZ == 1) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 2) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 3) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM001);
			}
			this->UpdataItem();
			this->mapPointX--;
			this->player->startPosition = this->player->transform.position;

			// ここで敵をどうこうする
			for (int index = 0; index < ENEMY_MAX; index++) {
				for (int wall = 0; wall < 4; wall++) {
					COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->invisibleWall[wall]->collider);

					if (result.isHit) {
						CObjGene::SetEnemyParameter(this->enemy[this->nowEnemyGroup][index], TAG_ENEMY_NULL, this->enemy[this->nowEnemyGroup][index]->transform.position.x, this->enemy[this->nowEnemyGroup][index]->transform.position.y);
					}
				}
			}
			this->nowEnemyGroup = (this->nowEnemyGroup + 1) % 2;
			this->SetEnemy();
			this->nowItemGroup = (this->nowItemGroup + 1) % 2;
			this->SetItem();
		}
		else if (this->frame <= (MAP_ONE_SIZE_X * SLIDENUM)) {
			CDraw::camera.x -= (MAP_CHIP_SIZE / SLIDENUM);
		}
		else {
			this->scene = PLAY_NORMAL;
			this->frame = 0;
			return;
		}
		break;
	case SLIDE_RIGHT:
		if (this->frame == 0) {
			if (this->mapPointZ == 0) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM005);
			}
			else if (this->mapPointZ == 1) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 2) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 3) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM001);
			}
			this->UpdataItem();
			this->mapPointX++;
			this->player->startPosition = this->player->transform.position;

			// ここで敵をどうこうする
			for (int index = 0; index < ENEMY_MAX; index++) {
				for (int wall = 0; wall < 4; wall++) {
					COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->invisibleWall[wall]->collider);

					if (result.isHit) {
						CObjGene::SetEnemyParameter(this->enemy[this->nowEnemyGroup][index], TAG_ENEMY_NULL, this->enemy[this->nowEnemyGroup][index]->transform.position.x, this->enemy[this->nowEnemyGroup][index]->transform.position.y);
					}
				}
			}
			this->nowEnemyGroup = (this->nowEnemyGroup + 1) % 2;
			this->SetEnemy();
			this->nowItemGroup = (this->nowItemGroup + 1) % 2;
			this->SetItem();
		}
		else if (this->frame <= (MAP_ONE_SIZE_X * SLIDENUM)) {
			CDraw::camera.x += (MAP_CHIP_SIZE / SLIDENUM);
		}
		else {
			this->scene = PLAY_NORMAL;
			this->frame = 0;
			return;
		}
		break;
	case SLIDE_BOTTOM:
		if (this->frame == 0) {
			if (this->mapPointZ == 0) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM005);
			}
			else if (this->mapPointZ == 1) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 2) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 3) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM001);
			}
			this->UpdataItem();
			this->mapPointY--;
			this->player->startPosition = this->player->transform.position;

			// ここで敵をどうこうする
			for (int index = 0; index < ENEMY_MAX; index++) {
				for (int wall = 0; wall < 4; wall++) {
					COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->invisibleWall[wall]->collider);

					if (result.isHit) {
						CObjGene::SetEnemyParameter(this->enemy[this->nowEnemyGroup][index], TAG_ENEMY_NULL, this->enemy[this->nowEnemyGroup][index]->transform.position.x, this->enemy[this->nowEnemyGroup][index]->transform.position.y);
					}
				}
			}
			this->nowEnemyGroup = (this->nowEnemyGroup + 1) % 2;
			this->SetEnemy();
			this->nowItemGroup = (this->nowItemGroup + 1) % 2;
			this->SetItem();
		}
		else if (this->frame <= (MAP_ONE_SIZE_Y * SLIDENUM)) {
			CDraw::camera.y -= (MAP_CHIP_SIZE / SLIDENUM);
		}
		else {
			this->scene = PLAY_NORMAL;
			this->frame = 0;
			return;
		}
		break;
	case SLIDE_TOP:
		if (this->frame == 0) {
			if (this->mapPointZ == 0) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM005);
			}
			else if (this->mapPointZ == 1) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 2) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM000);
			}
			else if (this->mapPointZ == 3) {
				this->bgm->XA_Play_BGM(SOUND_LABEL_BGM001);
			}
			this->UpdataItem();
			this->mapPointY++;
			this->player->startPosition = this->player->transform.position;

			// ここで敵をどうこうする
			for (int index = 0; index < ENEMY_MAX; index++) {
				for (int wall = 0; wall < 4; wall++) {
					COLLIDER_RESULT result = Collider_Test(&this->enemy[this->nowEnemyGroup][index]->collider, &this->invisibleWall[wall]->collider);

					if (result.isHit) {
						CObjGene::SetEnemyParameter(this->enemy[this->nowEnemyGroup][index], TAG_ENEMY_NULL, this->enemy[this->nowEnemyGroup][index]->transform.position.x, this->enemy[this->nowEnemyGroup][index]->transform.position.y);
					}
				}
			}
			this->nowEnemyGroup = (this->nowEnemyGroup + 1) % 2;
			this->SetEnemy();
			this->nowItemGroup = (this->nowItemGroup + 1) % 2;
			this->SetItem();
		}
		else if (this->frame <= (MAP_ONE_SIZE_Y * SLIDENUM)) {
			CDraw::camera.y += (MAP_CHIP_SIZE / SLIDENUM);
		}
		else {
			this->scene = PLAY_NORMAL;
			this->frame = 0;
			return;
		}
		break;
	default:
		break;
	}

	this->frame++;
}

void CPlay::SlideDraw() {
	Float2 float2 = CDraw::camera;

	int chipPosX = (int)(float2.x / MAP_CHIP_SIZE);
	int chipPosY = (int)(float2.y / MAP_CHIP_SIZE);
	int startX = chipPosX - 10;
	int startY = chipPosY - 6;
	int endX = chipPosX + 11;
	int endY = chipPosY + 7;

	if (startX < 0) {
		startX = 0;
	}
	if (startY < 0) {
		startY = 0;
	}
	if (endX > MAP_SIZE_X) {
		endX = MAP_SIZE_X;
	}
	if (endY > MAP_SIZE_Y) {
		endY = MAP_SIZE_Y;
	}

	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			this->mapChip_Draw->SetVertex(this->mapChip[x][y]);
		}
	}
	this->mapChip_Draw->DrawAct();

	for (int group = 0; group < 2; group++) {
		for (int index = 0; index < ITEM_MAX; index++) {
			this->enemy_and_item_Draw->SetVertex(this->item[group][index]);
		}
	}
	for (int group = 0; group < 2; group++) {
		for (int index = 0; index < ENEMY_MAX; index++) {
			this->enemy_and_item_Draw->SetVertex(this->enemy[group][index]);
		}
	}
	this->enemy_and_item_Draw->DrawAct();

	this->player_Draw->SetVertex(this->player);
	this->player_Draw->DrawAct();

	this->sysDraw->SetVertex(this->sysMainObj, Float2zero);
	this->sysDraw->SetVertex(this->sysMapPoint, Float2zero);
	for (int index = 0; index < 2; index++) {
		this->sysDraw->SetVertex(this->keyNumberObj[index], Float2zero);
	}
	for (int index = 0; index < 3; index++) {
		this->sysDraw->SetVertex(this->coinNumberObj[index], Float2zero);
	}
	this->sysDraw->DrawAct();

	this->staminaGage->transform.Scale.x = this->player->sp.now / this->player->sp.max;
	this->plAttackEffect_Draw->SetVertex(this->staminaGage, Float2zero);

	this->lifeGage->transform.Scale.x = this->player->hp.now / this->player->hp.max;
	this->plAttackEffect_Draw->SetVertex(this->lifeGage, Float2zero);

	this->plAttackEffect_Draw->SetVertex(this->lifeGageFrame, Float2zero);
	this->plAttackEffect_Draw->SetVertex(this->staminaGageFrame, Float2zero);

	this->plAttackEffect_Draw->DrawAct();

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

void CPlay::GameOver() {
	const float stepAnim[GAMEOVER_STEP] = { -5.0f, -5.0f, 5.0f, 5.0f, -10.0f, -10.0f, 10.0f, 10.0f, -50.0f, -50.0f, 50.0f, 50.0f, -10.0f, -10.0f, 10.0f, 10.0f, -5.0f, -5.0f, 5.0f, 5.0f, 0.0f };

	if (this->frame < GAMEOVER_STEP) {
		if (this->frame == 0) {
			this->bgm->XA_Stop_BGM();
		}
		this->player->animator.pos.x = stepAnim[this->frame];
		this->player->animator.frameX = 1;
		this->player->animator.frameY = 0;
		this->frame++;
	}
	else if (this->frame < GAMEOVER_DESTROY_WAIT) {
		if (this->frame == GAMEOVER_DESTROY_WAIT - 1) {
			CObjGene::SetEnemyParameter(this->enemy[0][0], TAG_ENEMY_DESTROY, this->player->transform.position.x + this->player->collider.offset.x, this->player->transform.position.y + this->player->collider.offset.y + 6);
		}
		this->frame++;
	}
	else if (this->enemy[0][0]->tag == TAG_ENEMY_NULL && this->frame == GAMEOVER_DESTROY_WAIT) {
		this->frame++;
	}
	if (this->frame > GAMEOVER_DESTROY_WAIT && this->frame < GAMEOVER_SE_WAIT) {
		if (this->frame == GAMEOVER_SE) {
			XA_Play(SOUND_LABEL_SE007);
		}
		if (this->frame > GAMEOVER_SE) {
			this->gameOverText->transform.Scale.y += 0.005f;
			if (this->gameOverText->transform.Scale.y > 1.0f) {
				this->gameOverText->transform.Scale.y = 1.0f;
			}
			this->gameOverText->transform.position.y = this->gameOverText->transform.Scale.y * (this->gameOverText->transform.size.y / 2);
		}
		this->frame++;
	}
	else if (this->frame == GAMEOVER_DESTROY_WAIT) {
		this->enemy[0][0]->controller.pMove->Update();
		this->enemy[0][0]->animator.anim->Update();
		//this->frame++;
	}
	else {
		if (Input_GetKeyTrigger('S')) {
			this->fade = FADEOUT;
		}
	}


}

void CPlay::GameOverDraw() {
	this->backGround->transform.position = CDraw::camera;
	this->plAttackEffect_Draw->SetVertex(this->backGround);
	this->plAttackEffect_Draw->DrawAct();

	if (this->frame < GAMEOVER_DESTROY_WAIT) {
		this->player_Draw->SetVertex(this->player);
		this->player_Draw->DrawAct();
	}
	else {
		this->enemy_and_item_Draw->SetVertex(this->enemy[0][0]);
		this->enemy_and_item_Draw->DrawAct();
	}

	this->sysDraw->SetVertex(this->sysMainObj, Float2zero);
	this->sysDraw->SetVertex(this->sysMapPoint, Float2zero);
	this->sysDraw->SetVertex(this->gameOverText, Float2zero);
	for (int index = 0; index < 2; index++) {
		this->sysDraw->SetVertex(this->keyNumberObj[index], Float2zero);
	}
	for (int index = 0; index < 3; index++) {
		this->sysDraw->SetVertex(this->coinNumberObj[index], Float2zero);
	}
	this->sysDraw->DrawAct();

	this->staminaGage->transform.Scale.x = this->player->sp.now / this->player->sp.max;
	this->plAttackEffect_Draw->SetVertex(this->staminaGage, Float2zero);

	this->lifeGage->transform.Scale.x = this->player->hp.now / this->player->hp.max;
	this->plAttackEffect_Draw->SetVertex(this->lifeGage, Float2zero);

	this->plAttackEffect_Draw->SetVertex(this->lifeGageFrame, Float2zero);
	this->plAttackEffect_Draw->SetVertex(this->staminaGageFrame, Float2zero);

	this->plAttackEffect_Draw->DrawAct();

	this->fadeDraw->SetVertex(this->fadeObj, MakeFloat2(0.0f, SCREEN_HEIGHT));
	this->fadeDraw->DrawAct();

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

void CPlay::GameClear() {
	const int rotateStepAnim[GAMECLEAR_STEP] = {
		NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST, NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH
	};

	if (this->frame == 0) {
		for (int index = 0; index < 12; index++) {
			this->gameClearText[index]->transform.position = this->gameClearText[index]->transform.position + CDraw::camera;
		}
	}

	if (this->frame < GAMEOVER_STEP) {
		int movePos = (SCREEN_WIDTH * 2) / (GAMEOVER_STEP - 1);
		this->gameClearBackGround[0]->transform.position.x += movePos;
		this->gameClearBackGround[1]->transform.position.x -= movePos;
	}
	else if (this->frame < GAMECLEAR_ROTATE + GAMEOVER_STEP) {
		int step = SOUTH;
		if (this->frame < GAMECLEAR_ROTATE_LOOP + GAMEOVER_STEP) {
			int count = (this->frame - GAMEOVER_STEP) % GAMECLEAR_DIRCOUNT;
			step = rotateStepAnim[count];
		}
		else {
			int count = (this->frame - (GAMECLEAR_ROTATE_LOOP + GAMEOVER_STEP)) % GAMECLEAR_STEP;
			step = rotateStepAnim[count];
		}
		if (this->frame == GAMECLEAR_ROTATE + GAMEOVER_STEP - 1) {
			this->getItem->transform.Scale.y = 1.0f;
		}
		switch (step)
		{
		case SOUTH:
			this->player->animator.frameX = 0;
			this->player->animator.frameY = 0;
			break;
		case WEST:
			this->player->animator.frameX = 0;
			this->player->animator.frameY = 1;
			break;
		case EAST:
			this->player->animator.frameX = 0;
			this->player->animator.frameY = 2;
			break;
		case NORTH:
			this->player->animator.frameX = 0;
			this->player->animator.frameY = 3;
			break;
		case SOUTH_WEST:
			this->player->animator.frameX = 3;
			this->player->animator.frameY = 0;
			break;
		case SOUTH_EAST:
			this->player->animator.frameX = 3;
			this->player->animator.frameY = 1;
			break;
		case NORTH_WEST:
			this->player->animator.frameX = 3;
			this->player->animator.frameY = 2;
			break;
		case NORTH_EAST:
		default:
			this->player->animator.frameX = 3;
			this->player->animator.frameY = 3;
			break;
		}
	}

	if (this->frame >= (GAMECLEAR_ROTATE + GAMEOVER_STEP) && this->frame < GAMECLEAR_TEXT_FADE) {
		int count = this->frame - (GAMECLEAR_ROTATE + GAMEOVER_STEP) + 1;
		this->gameClearText[8]->transform.Scale.y = (float)count * (0.05f);
		this->gameClearText[9]->transform.Scale.y = (float)count * (0.05f);
		if (this->frame == GAMECLEAR_TEXT_FADE) {
			this->gameClearText[8]->transform.Scale.y = 1.0f;
			this->gameClearText[9]->transform.Scale.y = 1.0f;
		}
	}

	if (this->frame >= GAMECLEAR_STOP_START && this->frame < GAMECLEAR_STOP_END) {
		float movePos = (-(SCREEN_HEIGHT * 2) - 1432.0f) / GAMECLEAR_CAMERA_MOVE;
		CDraw::camera.y += movePos;
		if (this->frame == GAMECLEAR_STOP_END - 1) {
			CDraw::camera = this->mapCamera;
		}
	}

	if (this->frame < GAMECLEAR_END) {
		this->frame++;
	}
	else {
		if (Input_GetKeyTrigger('S')) {
			this->fade = FADEOUT;
			XA_Stop(SOUND_LABEL_SE004);
		}
	}
}

void CPlay::GameClearDraw() {
	Float2 float2 = CDraw::camera;

	int chipPosX = (int)(float2.x / MAP_CHIP_SIZE);
	int chipPosY = (int)(float2.y / MAP_CHIP_SIZE);
	int startX = chipPosX - 10;
	int startY = chipPosY - 6;
	int endX = chipPosX + 11;
	int endY = chipPosY + 7;

	if (startX < 0) {
		startX = 0;
	}
	if (startY < 0) {
		startY = 0;
	}
	if (endX > MAP_SIZE_X) {
		endX = MAP_SIZE_X;
	}
	if (endY > MAP_SIZE_Y) {
		endY = MAP_SIZE_Y;
	}

	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			this->mapChip_Draw->SetVertex(this->mapChip[x][y]);
		}
	}
	this->mapChip_Draw->DrawAct();

	for (int index = 0; index < 2; index++) {
		this->gameClearDraw->SetVertex(this->gameClearBackGround[index], Float2zero);
	}
	for (int index = 0; index < 12; index++) {
		this->gameClearDraw->SetVertex(this->gameClearText[index]);
	}
	this->gameClearDraw->DrawAct();

	if (this->getItem != NULL) {
		this->enemy_and_item_Draw->SetVertex(this->getItem);
		this->enemy_and_item_Draw->DrawAct();
	}

	this->player_Draw->SetVertex(this->player);
	this->player_Draw->DrawAct();

	this->sysDraw->SetVertex(this->sysMainObj, Float2zero);
	this->sysDraw->SetVertex(this->sysMapPoint, Float2zero);
	for (int index = 0; index < 2; index++) {
		this->sysDraw->SetVertex(this->keyNumberObj[index], Float2zero);
	}
	for (int index = 0; index < 3; index++) {
		this->sysDraw->SetVertex(this->coinNumberObj[index], Float2zero);
	}
	this->sysDraw->DrawAct();

	this->staminaGage->transform.Scale.x = this->player->sp.now / this->player->sp.max;
	this->plAttackEffect_Draw->SetVertex(this->staminaGage, Float2zero);

	this->lifeGage->transform.Scale.x = this->player->hp.now / this->player->hp.max;
	this->plAttackEffect_Draw->SetVertex(this->lifeGage, Float2zero);

	this->plAttackEffect_Draw->SetVertex(this->lifeGageFrame, Float2zero);
	this->plAttackEffect_Draw->SetVertex(this->staminaGageFrame, Float2zero);

	this->plAttackEffect_Draw->DrawAct();

	this->fadeDraw->SetVertex(this->fadeObj, MakeFloat2(0.0f, SCREEN_HEIGHT));
	this->fadeDraw->DrawAct();

	// ダブル・バッファのディスプレイ領域へのコピー命令
	Direct3D_GetSwapChain()->Present(0, 0);
}

void CPlay::ItemGet() {
	const int stepAnim[ITEMGET_STEP] = {
	NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST, NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH
	};
	if (this->frame < ITEMGET_END) {
		if (this->getItem != NULL) {
			if (this->frame < ITEMGET_STEP) {
				switch (stepAnim[this->frame])
				{
				case SOUTH:
					this->player->animator.frameX = 0;
					this->player->animator.frameY = 0;
					break;
				case WEST:
					this->player->animator.frameX = 0;
					this->player->animator.frameY = 1;
					break;
				case EAST:
					this->player->animator.frameX = 0;
					this->player->animator.frameY = 2;
					break;
				case NORTH:
					this->player->animator.frameX = 0;
					this->player->animator.frameY = 3;
					break;
				case SOUTH_WEST:
					this->player->animator.frameX = 3;
					this->player->animator.frameY = 0;
					break;
				case SOUTH_EAST:
					this->player->animator.frameX = 3;
					this->player->animator.frameY = 1;
					break;
				case NORTH_WEST:
					this->player->animator.frameX = 3;
					this->player->animator.frameY = 2;
					break;
				case NORTH_EAST:
				default:
					this->player->animator.frameX = 3;
					this->player->animator.frameY = 3;
					break;
				}
				this->getItem->transform.Scale.y = 0.0f;
			}
			else {
				this->getItem->transform.Scale.y = 1.0f;
			}
		}
		this->frame++;
	}
	else {
		this->bgm->XA_Play_BGM_OneMore();
		this->frame = 0;
		this->scene = PLAY_NORMAL;
		this->getItem = NULL;
	}
}

void CPlay::CrystalGet() {
	this->ItemGet();
	if (this->frame >= ITEMGET_END) {
		this->bgm->XA_Stop_BGM();
		this->frame = 0;
		this->getItem = NULL;
		this->mapPointX = this->mapLastPointX;
		this->mapPointY = this->mapLastPointY;
		this->mapPointZ = this->mapLastPointZ;
		this->fade = FADEOUT;
		this->scene = PLAY_CHANGMAP;
	}
}

#pragma endregion

#pragma region // CGame (ゲーム全体)

CGame::CGame(const HWND *hWnd) {
	this->title = new CTitle;
	this->play = new CPlay;
	this->pGameMode[GAME_MODE_TITLE] = this->title;
	this->pGameMode[GAME_MODE_PLAY] = this->play;

	for (int index = 0; index < GAME_MODE_NUM; index++) {
		this->pGameMode[index]->SethWnd(hWnd);
		this->pGameMode[index]->SetCGame(this);
	}

	this->isInit = false;
	this->gameOrder = GAME_ORDER_NONE;

	this->gameMode = GAME_MODE_TITLE;
}

CGame::~CGame() {
	for (int index = 0; index < GAME_MODE_NUM; index++) {
		this->pGameMode[index] = NULL;
	}
	delete this->title;
	delete this->play;
}

bool CGame::Update() {
	bool isContinue = true;

	if (!this->isInit) {
		isContinue = this->pGameMode[this->gameMode]->Init();
		this->isInit = true;
	}
	else {
		this->gameOrder = this->pGameMode[this->gameMode]->Update();
		if (!(this->gameOrder & GAME_ORDER_CONTINUE)) {
			this->pGameMode[this->gameMode]->UnInit();
			this->isInit = false;

			if (this->gameOrder & GAME_ORDER_MODECHANGE_TITLE) {
				this->gameMode = GAME_MODE_TITLE;
			}
			else if (this->gameOrder & GAME_ORDER_MODECHANGE_PLAY) {
				this->gameMode = GAME_MODE_PLAY;
			}
			else {
				isContinue = false;
			}
		}
	}

	return isContinue;
}

#pragma endregion

