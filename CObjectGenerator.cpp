#include "CObjectGenerator.h"

void CObjGene::CreateInvisibleObject(CGameObject *obj, float posX, float posY, float sizeX, float sizeY) {
	FDATA_GAMEOBJECT data = {
		{
			{ posX, posY },
			{ sizeX, sizeY },
			{ 1.0f, 1.0f }
		},{
			{ 0.0f, 0.0f },
			{ 0.0f, 0.0f },
			FLIP_NONE
		},
		ANIM_NONE,
		MOVE_NONE,
		COLLIDER_TRANSFORM,
		{ 0.0f, 0.0f },
		NULL,
		TAG_INVISIBLE_WALL,
	{ -1, -1, 0, 0 },
	{ -1, -1 },
	{ -1, -1, 0, 0 }
	};

	obj->Init(data);
}

void CObjGene::CreateBackGround(CGameObject *backGround, int scrollType, int index) {
	FDATA_GAMEOBJECT data = {
		{
			{ -(SCREEN_WIDTH / 3), 0.0f },
			{ SCREEN_WIDTH * (4.0f / 3.0f), SCREEN_HEIGHT * 2 },
			{ 1.0f, 1.0f }
		},{
			{ 0.0f, 0.0f },
			{ 1.0f, 1.0f },
			FLIP_NONE
		},
		ANIM_NONE,
		MOVE_NONE,
		COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ -1, -1, 0, 0 },
		{ -1, -1 },
		{ -1, -1, 0, 0 }
	};

	switch (scrollType)
	{
	case MAPCHIP:
		data.transform.position.x = (index % MAP_SIZE_X) * MAP_CHIP_SIZE + (MAP_CHIP_SIZE / 2);
		data.transform.position.y = (index / MAP_SIZE_X) * MAP_CHIP_SIZE + (MAP_CHIP_SIZE / 2);
		data.transform.size = MakeFloat2(MAP_CHIP_SIZE, MAP_CHIP_SIZE);
		data.uv.size = MakeFloat2(MAPVHIP_UV_SIZE, MAPVHIP_UV_SIZE);
		data.colliderType = COLLIDER_TRANSFORM;
		break;
	case GAGE:
		data.transform.position = Float2zero;
		data.transform.size = MakeFloat2(160.0f * 2, 16.0f * 2);
		data.uv.offset = MakeFloat2(768.0f / 1024.0f, 512.0f / 768.0f);
		data.uv.size = MakeFloat2(16.0f / 1024.0f, 16.0f / 768.0f);
		break;
	case GAMEOVER_BACKGROUND:
		data.transform.position = Float2zero;
		data.transform.size = MakeFloat2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
		data.uv.offset = MakeFloat2(768.0f / 1024.0f, 512.0f / 768.0f);
		data.uv.size = MakeFloat2(16.0f / 1024.0f, 16.0f / 768.0f);
		break;
	case GAMEOVER_TEXT:
		data.transform.position = Float2zero;
		data.transform.size = MakeFloat2(1152.0f, 144.0f);
		data.uv.offset = MakeFloat2(485.0f / 1024.0f, 28.0f / 1024.0f);
		data.uv.size = MakeFloat2(288.0f / 1024.0f, 36.0f / 1024.0f);
		data.transform.Scale.y = 0.0f;
		break;
	case GAMECLEAR_BG:
		if (index == 0) {
			data.transform.position = MakeFloat2(-(SCREEN_WIDTH * 2), 0);
		}
		else {
			data.transform.position = MakeFloat2((SCREEN_WIDTH * 2), 0);
		}
		data.transform.size = MakeFloat2((SCREEN_WIDTH * 2), (SCREEN_HEIGHT * 2));
		data.uv.offset = MakeFloat2(492.0f / 1024.0f, 0.0f);
		data.uv.size = MakeFloat2(5.0f / 1024.0f, 5.0f / 1024.0f);
		break;
	case SYSTEM_TEXT:
		switch (index)
		{
		case 1:
			data.transform.size = MakeFloat2(800.0f, 152.0f);
			data.uv.offset = MakeFloat2(824.0f / 1024.0f, 0.0f);
			data.uv.size = MakeFloat2(200.0f / 1024.0f, 38.0f / 1024.0f);
			break;
		case 2:
			data.transform.size = MakeFloat2(1240.0f, 240.0f);
			data.uv.offset = MakeFloat2(480.0f / 1024.0f, 64.0f / 1024.0f);
			data.uv.size = MakeFloat2(310.0f / 1024.0f, 60.0f / 1024.0f);
			break;
		case 3:
			data.transform.size = MakeFloat2(1440.0f, 352.0f);
			data.uv.offset = MakeFloat2(480.0f / 1024.0f, 124.0f / 1024.0f);
			data.uv.size = MakeFloat2(360.0f / 1024.0f, 88.0f / 1024.0f);
			break;
		case 4:
			data.transform.size = MakeFloat2(392.0f, 128.0f);
			data.uv.offset = MakeFloat2(840.0f / 1024.0f, 124.0f / 1024.0f);
			data.uv.size = MakeFloat2(98.0f / 1024.0f, 32.0f / 1024.0f);
			break;
		case 5:
			data.transform.size = MakeFloat2(744.0f, 248.0f);
			data.uv.offset = MakeFloat2(790.0f / 1024.0f, 62.0f / 1024.0f);
			data.uv.size = MakeFloat2(186.0f / 1024.0f, 62.0f / 1024.0f);
			break;
		case 6:
			data.transform.size = MakeFloat2(248.0f, 128.0f);
			data.uv.offset = MakeFloat2(938.0f / 1024.0f, 124.0f / 1024.0f);
			data.uv.size = MakeFloat2(62.0f / 1024.0f, 32.0f / 1024.0f);
			break;
		case 7:
			data.transform.size = MakeFloat2(744.0f, 248.0f);
			data.uv.offset = MakeFloat2(790.0f / 1024.0f, 62.0f / 1024.0f);
			data.uv.size = MakeFloat2(186.0f / 1024.0f, 62.0f / 1024.0f);
			break;
		case 8:
			data.transform.size = MakeFloat2(1156.0f, 92.0f);
			data.uv.offset = MakeFloat2(446.0f / 1024.0f, 212.0f / 1024.0f);
			data.uv.size = MakeFloat2(578.0f / 1024.0f, 46.0f / 1024.0f);
			break;
		case 9:
			data.transform.size = MakeFloat2(1340.0f, 92.0f);
			data.uv.offset = MakeFloat2(354.0f / 1024.0f, 258.0f / 1024.0f);
			data.uv.size = MakeFloat2(670.0f / 1024.0f, 46.0f / 1024.0f);
			break;
		case 10:
			data.transform.size = MakeFloat2(1120.0f, 92.0f);
			data.uv.offset = MakeFloat2(464.0f / 1024.0f, 304.0f / 1024.0f);
			data.uv.size = MakeFloat2(560.0f / 1024.0f, 46.0f / 1024.0f);
			break;
		case 11:
			data.transform.size = MakeFloat2(1100.0f, 92.0f);
			data.uv.offset = MakeFloat2(474.0f / 1024.0f, 350.0f / 1024.0f);
			data.uv.size = MakeFloat2(550.0f / 1024.0f, 46.0f / 1024.0f);
			break;
		case 0:
		default:
			data.transform.size = MakeFloat2(1304.0f, 640.0f);
			data.uv.offset = MakeFloat2(698.0f / 1024.0f, 396.0f / 1024.0f);
			data.uv.size = MakeFloat2(326.0f / 1024.0f, 162.0f / 1024.0f);
			break;
		}
		data.transform.position = Float2zero;
		break;
	default:
		break;
	}

	backGround->Init(data);

	if (scrollType == GAGE) {
		if (index == 0) {
			backGround->animator.frameY = 1;
		}
		else if (index == 1) {
			backGround->animator.frameY = 2;
		}
		else {
			backGround->animator.frameY = 3;
		}
	}
	else if (scrollType == GAMEOVER_BACKGROUND) {
		backGround->animator.frameY = 1;
	}
}

void CObjGene::CreatePlayer(CGameObject *mapChip, float posX, float posY) {
	FDATA_GAMEOBJECT data = {
		{
			{ posX, posY },
			{ 120.0f, 168.0f },
			{ 1.0f, 1.0f }
		},{
			{ 0.0f, 0.0f },
			{ (120.0f / 6.0f) / 120.0f, (112.0f / 4.0f) / 112.0f },
			FLIP_NONE
		},
		ANIM_PLAYER,
		MOVE_PLAYER,
		COLLIDER_PLAYER,
		{ 0.0f, 0.0f },
		NULL,
		TAG_PLAYER,
		{ 15, 15, 0, 0 },
		{ 15, 15 },
		{ 0, 0, 0, 0 }
	};

	mapChip->Init(data);
}

void CObjGene::CreatePLAtackEffect(CGameObject *effect, CGameObject *player) {
	FDATA_GAMEOBJECT data = {
		{
			{ 0, 0 },
			{ 512.0f, 512.0f },
			{ 1.0f, 1.0f }
		},{
			{ 0, 0 },
			{ ATTACK_0_UV_SIZE_X, ATTACK_0_UV_SIZE_Y },
			FLIP_NONE
		},
		ANIM_PLATTACKEFFECT,
		MOVE_NONE,
		COLLIDER_TRANSFORM,
		{ -SCREEN_WIDTH, -SCREEN_HEIGHT },
		player,
		TAG_PLAYERATTACK,
		{ -1, -1, 0, 0 },
		{ -1, -1 },
		{ -1, -1, 0, 0 }
	};

	effect->Init(data);

	effect->collider.isActive = false;
	effect->isActive = false;
}

void CObjGene::CreateNullEnemy(CGameObject *mapChip) {
	FDATA_GAMEOBJECT data = {
		{
			Float2zero,
			Float2zero,
			Float2zero
		},{
			Float2zero,
			Float2zero,
			FLIP_NONE
		},
		ANIM_ENEMY,
		MOVE_ENEMY,
		COLLIDER_NONE,
		Float2zero,
		NULL,
		TAG_ENEMY_NULL,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	mapChip->Init(data);
}

void CObjGene::CreateNullItem(CGameObject *mapChip) {
	FDATA_GAMEOBJECT data = {
		{
			Float2zero,
			Float2zero,
			Float2zero
		},{
			Float2zero,
			Float2zero,
			FLIP_NONE
		},
		ANIM_ITEM,
		MOVE_ITEM,
		COLLIDER_NONE,
		Float2zero,
		NULL,
		TAG_ITEM_NULL,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	mapChip->Init(data);
}

void CObjGene::CreateSysObj(CGameObject *pThis) {
	FDATA_GAMEOBJECT data = {
	{
		{ 0, MAP_CHIP_SIZE * 5 },
		{ SCREEN_WIDTH * 2, MAP_CHIP_SIZE * 2 },
		{ 1, 1 }
	},{
		{ 0.0f, 0.0f },
		{ 480.0f / 1024.0f, 64.0f / 1024.0f },
		FLIP_NONE
	},
	ANIM_NONE,
	MOVE_NONE,
	COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);

}

void CObjGene::CreateSysMapPoint(CGameObject *pThis) {
	FDATA_GAMEOBJECT data = {
	{
		{ 0, 0 },
		{ 16.0f, 16.0f },
		{ 1, 1 }
	},{
		{ 485.0f / 1024.0f, 0.0f },
		{ 5.0f / 1024.0f, 5.0f / 1024.0f },
		FLIP_NONE
	},
	ANIM_NONE,
	MOVE_NONE,
	COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);

}

void CObjGene::CreateFade(CGameObject *pThis) {
	FDATA_GAMEOBJECT data = {
	{
		{ -SCREEN_WIDTH, SCREEN_HEIGHT },
		{ SCREEN_WIDTH * 4, SCREEN_HEIGHT * 2 },
		{ 1, 1 }
	},{
		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },
		FLIP_NONE
	},
	ANIM_NONE,
	MOVE_NONE,
	COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);

}

void CObjGene::CreateNumber(CGameObject *pThis) {
	FDATA_GAMEOBJECT data = {
		{
			{ 0.0f, 0.0f },
			{ 64.0f, 64.0f },
			{ 1, 1 }
		},{
			{ 0.0f, 80.0f / 1024.0f },
			{ 16.0f / 1024.0f, 16.0f / 1024.0f },
			FLIP_NONE
		},
		ANIM_NONE,
		MOVE_NONE,
		COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);
}

void CObjGene::CreateTitleBack(CGameObject *pThis, int index) {
	FDATA_GAMEOBJECT data = {
		{
			{ SCREEN_WIDTH, SCREEN_HEIGHT * 5 },
			{ SCREEN_WIDTH * 2.0f, SCREEN_HEIGHT * 10.0f },
			{ 1, 1 }
		},{
			{ 0.0f, 0.0f },
			{ 0.25f, 1.0f },
			FLIP_NONE
		},
		ANIM_NONE,
		MOVE_NONE,
		COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);

	pThis->animator.frameX = index % 3;
}

void CObjGene::CreateTitleText(CGameObject *pThis, int index) {
	FDATA_GAMEOBJECT data = {
		{
			{ 0.0f, 0.0f },
			{ SCREEN_WIDTH * 2.0f, SCREEN_HEIGHT * 2.0f },
			{ 1, 1 }
		},{
			{ 0.75f, 0.0f },
			{ 0.25f, 0.2f },
			FLIP_NONE
		},
		ANIM_NONE,
		MOVE_NONE,
		COLLIDER_NONE,
		{ 0.0f, 0.0f },
		NULL,
		TAG_BACKGROUND,
		{ 0.0f, 0.0f, 0, 0 },
		{ 0.0f, 0.0f },
		{ 0.0f, 0, 0.0f, 0 }
	};

	pThis->Init(data);

	pThis->animator.frameY = index % 5;
}

void CObjGene::SetEnemyParameter(CGameObject *pThis, int kind, float posX, float posY) {
	const Float2 enemySize[5] = {
		{120.0f, 108.0f},
		{120.0f, 168.0f},
		{144.0f, 180.0f},
		{264.0f, 312.0f},
		{228.0f, 180.0f},
	};

	pThis->collider.dontHitFrame = 0;
	pThis->transform.Scale = MakeFloat2(1.0f, 1.0f);
	pThis->collider.isActive = true;
	pThis->tag = kind;
	pThis->hp.now = 0.0f;
	pThis->hp.max = 0.0f;
	pThis->hp.protectKnockBack = 0.0f;
	pThis->hp.protectStop = 0;
	pThis->sp.now = 0.0f;
	pThis->sp.max = 0.0f;
	pThis->attack.dontHitFrame = 40;
	pThis->attack.knockBack = 0.0f;

	switch (kind)
	{
	case TAG_ENEMY_DESTROY:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -6.0f);
		pThis->collider.size = MakeFloat2(60.0f, 72.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 2)) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 0.0f;
		pThis->attack.stopFrame = 0;
		break;
	case TAG_ENEMY_SKULL:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -6.0f);
		pThis->collider.size = MakeFloat2(60.0f, 72.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 2)) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 1.25f;
		pThis->attack.stopFrame = 1;
		break;
	case TAG_ENEMY_SPIRIT_1:
	case TAG_ENEMY_SPIRIT_2:
	case TAG_ENEMY_SPIRIT_3:
	case TAG_ENEMY_SPIRIT_4:
	case TAG_ENEMY_SPIRIT_5:
	case TAG_ENEMY_SPIRIT_6:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, 12.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 1.25f;
		pThis->attack.stopFrame = 5;
		if (kind == TAG_ENEMY_SPIRIT_2) {
			pThis->attack.knockBack = 25.0f;
		}
		if (kind == TAG_ENEMY_SPIRIT_6) {
			pThis->attack.knockBack = 25.0f;
			pThis->attack.power = 5.0f;
		}
		break;
	case TAG_ENEMY_ORC_1:
	case TAG_ENEMY_ORC_2:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		if (kind == TAG_ENEMY_ORC_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + ENEMY_PIC_TYPE_2_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 1.25f;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 3.0f;
			pThis->hp.max = 3.0f;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + ENEMY_PIC_TYPE_2_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = 5;
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 10;
			pThis->hp.now = 6.0f;
			pThis->hp.max = 6.0f;
		}
		break;
	case TAG_ENEMY_PLANT_1:
	case TAG_ENEMY_PLANT_2:
	case TAG_ENEMY_PLANT_FLOWER:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		if (kind == TAG_ENEMY_PLANT_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 1.25f;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 2.0f;
			pThis->hp.max = 2.0f;
			pThis->hp.protectStop = -5;
		}
		else if (kind == TAG_ENEMY_PLANT_2) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 1.25f;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 4.0f;
			pThis->hp.max = 4.0f;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + ENEMY_PIC_TYPE_2_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 1.25f;;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 2.0f;
			pThis->hp.max = 2.0f;
		}
		break;
	case TAG_ENEMY_MOMMY:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 3) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 2.5f;
		pThis->attack.stopFrame = 5;
		pThis->hp.protectKnockBack = 15.0f;
		pThis->hp.now = 100.0f;
		pThis->hp.max = 100.0f;
		break;
	case TAG_ENEMY_SLIME:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 1.25f;
		pThis->attack.stopFrame = 5;
		pThis->hp.now = 2.0f;
		pThis->hp.max = 2.0f;
		break;
	case TAG_ENEMY_GHOST_1:
	case TAG_ENEMY_GHOST_2:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		if (kind == TAG_ENEMY_GHOST_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 2)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->hp.protectStop = 5;
			pThis->attack.power = 1.25f;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 8.0f;
			pThis->hp.max = 8.0f;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 2)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 10;
			pThis->hp.protectKnockBack = 25.0f;
			pThis->hp.protectStop = 5;
			pThis->hp.now = 10.0f;
			pThis->hp.max = 10.0f;
		}
		break;
	case TAG_ENEMY_EYE_1:
	case TAG_ENEMY_EYE_2:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, 12.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		if (kind == TAG_ENEMY_EYE_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 3)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 1.25f;
			pThis->attack.stopFrame = 5;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.now = 2.0f;
			pThis->hp.max = 2.0f;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 3)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 10;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.now = 5.0f;
			pThis->hp.max = 5.0f;
		}
		break;
	case TAG_ENEMY_SHADOW:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 4)) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 2.5f;
		pThis->attack.stopFrame = 10;
		pThis->hp.now = 6.0f;
		pThis->hp.max = 6.0f;
		break;
	case TAG_ENEMY_LIZARD_1:
	case TAG_ENEMY_LIZARD_2:
		pThis->transform.size = enemySize[0];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = MakeFloat2(72.0f, 108.0f);
		if (kind == TAG_ENEMY_LIZARD_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 4)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y - 1) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_1_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_1_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 2.5f;
			pThis->attack.stopFrame = 5;
			pThis->hp.now = 10.0f;
			pThis->hp.max = 10.0f;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 4)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y + ENEMY_PIC_TYPE_1_SIZE_Y - 1) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_1_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_1_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 10;
			pThis->hp.now = 15.0f;
			pThis->hp.max = 15.0f;
		}
		break;
	case TAG_ENEMY_DEATH:
		pThis->transform.size = enemySize[2];
		pThis->collider.offset = MakeFloat2(0.0f, -24.0f);
		pThis->collider.size = MakeFloat2(60.0f, 108.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 5) + ENEMY_PIC_TYPE_4_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_3_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_3_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 2.5f;
		pThis->attack.stopFrame = 5;
		break;
	case TAG_ENEMY_GOLEM_1:
	case TAG_ENEMY_GOLEM_2:
		pThis->transform.size = enemySize[3];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = MakeFloat2(264.0f, 288.0f);
		if (kind == TAG_ENEMY_GOLEM_1) {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 5)) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_4_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_4_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 5;
			pThis->attack.knockBack = 30.0f;
			pThis->hp.now = 15.0f;
			pThis->hp.max = 15.0f;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = 200;
		}
		else {
			pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 5)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_4_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_4_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_4_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 10.0f;
			pThis->attack.stopFrame = 15;
			pThis->attack.knockBack = 30.0f;
			pThis->hp.now = 25.0f;
			pThis->hp.max = 25.0f;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = -25;
		}
		break;
	case TAG_ENEMY_DRAGON_1:
	case TAG_ENEMY_DRAGON_2:
	case TAG_ENEMY_DRAGON_3:
		pThis->transform.size = enemySize[4];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = MakeFloat2(108.0f, 132.0f);
		if (kind == TAG_ENEMY_DRAGON_1) {
			pThis->uv.offset = MakeFloat2(0.0f, 0.0f);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_5_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 5.0f;
			pThis->attack.stopFrame = 10;
			pThis->hp.now = 15.0f;
			pThis->hp.max = 15.0f;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = 100;
		}
		else if (kind == TAG_ENEMY_DRAGON_2) {
			pThis->transform.size = pThis->transform.size * 1.5;
			pThis->collider.offset = pThis->collider.offset * 1.5;
			pThis->collider.size = pThis->collider.size * 1.5;
			pThis->uv.offset = MakeFloat2(0.0f, (ENEMY_PIC_TYPE_5_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_5_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 10.0f;
			pThis->attack.stopFrame = 15;
			pThis->hp.now = 20.0f;
			pThis->hp.max = 20.0f;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = 100;
		}
		else {
			pThis->transform.size = pThis->transform.size * 2;
			pThis->collider.offset = pThis->collider.offset * 2;
			pThis->collider.size = pThis->collider.size * 2;
			pThis->uv.offset = MakeFloat2(0.0f, (ENEMY_PIC_TYPE_5_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_5_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
			pThis->attack.power = 15.0f;
			pThis->attack.stopFrame = 20;
			pThis->hp.now = 25.0f;
			pThis->hp.max = 25.0f;
			pThis->hp.protectKnockBack = 200.0f;
			pThis->hp.protectStop = 100;
		}
		break;
	case TAG_ENEMY_ME:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, -30.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 3)) / ENEMY_PIC_ALL_SIZE_X, 0.0f);
		pThis->uv.size = MakeFloat2((ENEMY_PIC_TYPE_2_SIZE_X / 6) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y / 4) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->attack.power = 5.0f;
		pThis->attack.stopFrame = 0;
		pThis->attack.dontHitFrame = 60;
		pThis->attack.knockBack = 50;
		pThis->hp.max = pThis->game->play->player->hp.max;
		pThis->hp.now = pThis->hp.max;
		break;
	case TAG_ENEMY_NULL:
	default:
		pThis->uv.offset = MakeFloat2(0.0f, 0.0f);
		pThis->uv.size = MakeFloat2(0.0f, 0.0f);
		pThis->transform.Scale = Float2zero;
		pThis->collider.isActive = false;
		pThis->tag = TAG_ENEMY_NULL;
		break;
	}

	pThis->startPosition = MakeFloat2(posX, posY);
	pThis->transform.position = MakeFloat2(posX, posY);
	pThis->animator.anim->Init();
	pThis->controller.pMove->Init();
}

void CObjGene::SetItemParameter(CGameObject *pThis, int kind, float posX, float posY) {
	const Float2 enemySize[4] = {
		{120.0f, 168.0f},
		{MAP_CHIP_SIZE, MAP_CHIP_SIZE},
		{128.0f, 144.0f},
		{288.0f, 300.0f},
	};

	pThis->collider.dontHitFrame = 0;
	pThis->transform.Scale = MakeFloat2(1.0f, 1.0f);
	pThis->collider.isActive = true;
	pThis->tag = kind;
	pThis->hp.now = 0.0f;
	pThis->hp.max = 0.0f;
	pThis->hp.protectKnockBack = 0.0f;
	pThis->hp.protectStop = 0;
	pThis->sp.now = 0.0f;
	pThis->sp.max = 0.0f;
	pThis->attack.dontHitFrame = 40;
	pThis->attack.knockBack = 0.0f;
	pThis->attack.power = 0.0f;
	pThis->attack.stopFrame = 0;

	switch (kind)
	{
	case TAG_ITEM_HEART:
	case TAG_ITEM_COIN_1:
	case TAG_ITEM_COIN_2:
	case TAG_ITEM_KEY:
	case TAG_ITEM_STATES_UP:
	case TAG_ITEM_NECKLACE:
	case TAG_ITEM_HEART_SHOP:
	case TAG_ITEM_KEY_SHOP:
	case TAG_ITEM_STATES_UP_SHOP:
		pThis->transform.size = enemySize[0];
		pThis->collider.offset = MakeFloat2(0.0f, 12.0f);
		pThis->collider.size = MakeFloat2(60.0f, 96.0f);
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 5)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_4_SIZE_Y * 2) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ITEM_PIC_TYPE_1_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ITEM_PIC_TYPE_1_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
		if (kind == TAG_ITEM_COIN_1) {
			pThis->uv.offset.y += pThis->uv.size.y;
		}
		else if (kind == TAG_ITEM_COIN_2) {
			pThis->uv.offset.y += (pThis->uv.size.y * 2);
		}
		else if (kind == TAG_ITEM_KEY) {
			pThis->uv.offset.y += (pThis->uv.size.y * 3);
		}
		else if (kind == TAG_ITEM_HEART_SHOP) {
			pThis->uv.offset.y += (pThis->uv.size.y * 4);
		}
		else if (kind == TAG_ITEM_KEY_SHOP) {
			pThis->uv.offset.y += (pThis->uv.size.y * 5);
		}
		else if (kind == TAG_ITEM_STATES_UP_SHOP) {
			pThis->uv.offset.y += (pThis->uv.size.y * 6);
		}
		else if (kind == TAG_ITEM_STATES_UP) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
		}
		else if (kind == TAG_ITEM_NECKLACE) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
			pThis->uv.offset.y += pThis->uv.size.y;
		}
		break;
	case TAG_ITEM_DOOR_KEY:
	case TAG_ITEM_DOOR_LOCK:
	case TAG_ITEM_FLOOR_KEY:
	case TAG_ITEM_FLOOR_STATES_UP:
	case TAG_ITEM_DOOR_CRYSTAL_1:
	case TAG_ITEM_DOOR_CRYSTAL_2:
	case TAG_ITEM_DOOR_CRYSTAL_3:
	case TAG_ITEM_DOOR_CRYSTAL_4:
	case TAG_ITEM_DOOR_CRYSTAL_5:
	case TAG_ITEM_DOOR_CRYSTAL_6:
	case TAG_ITEM_DOOR_CRYSTAL_7:
	case TAG_ITEM_DOOR_CRYSTAL_8:
	case TAG_ITEM_MOVE_BLOCK_1:
	case TAG_ITEM_MOVE_BLOCK_2:
	case TAG_ITEM_FLOOR_UP:
	case TAG_ITEM_FLOOR_DOWN:
		pThis->transform.size = enemySize[1];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = enemySize[1];
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 5) + ENEMY_PIC_TYPE_4_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_3_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ITEM_PIC_TYPE_2_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ITEM_PIC_TYPE_2_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
		if (kind == TAG_ITEM_DOOR_LOCK) {
			pThis->uv.offset.y += pThis->uv.size.y;
		}
		else if (kind == TAG_ITEM_FLOOR_KEY) {
			pThis->uv.offset.y += (pThis->uv.size.y * 2);
		}
		else if (kind == TAG_ITEM_FLOOR_STATES_UP) {
			pThis->uv.offset.y += (pThis->uv.size.y * 3);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_1) {
			pThis->uv.offset.y += (pThis->uv.size.y * 4);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_2) {
			pThis->uv.offset.y += (pThis->uv.size.y * 5);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_3) {
			pThis->uv.offset.y += (pThis->uv.size.y * 6);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_4) {
			pThis->uv.offset.y += (pThis->uv.size.y * 7);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_5) {
			pThis->uv.offset.y += (pThis->uv.size.y * 8);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_6) {
			pThis->uv.offset.y += (pThis->uv.size.y * 9);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_7) {
			pThis->uv.offset.y += (pThis->uv.size.y * 10);
		}
		else if (kind == TAG_ITEM_DOOR_CRYSTAL_8) {
			pThis->uv.offset.y += (pThis->uv.size.y * 11);
		}
		else if (kind == TAG_ITEM_MOVE_BLOCK_1) {
			pThis->uv.offset.y += (pThis->uv.size.y * 12);
		}
		else if (kind == TAG_ITEM_MOVE_BLOCK_2) {
			pThis->uv.offset.y += (pThis->uv.size.y * 12);
			pThis->uv.offset.x += (pThis->uv.size.x * 1);
		}
		else if (kind == TAG_ITEM_FLOOR_UP) {
			pThis->uv.offset.y += (pThis->uv.size.y * 12);
			pThis->uv.offset.x += (pThis->uv.size.x * 2);
		}
		else if (kind == TAG_ITEM_FLOOR_DOWN) {
			pThis->uv.offset.y += (pThis->uv.size.y * 13);
			pThis->uv.offset.x += (pThis->uv.size.x * 2);
		}
		break;
	case TAG_ITEM_CRYSTAL_1:
	case TAG_ITEM_CRYSTAL_2:
	case TAG_ITEM_CRYSTAL_3:
	case TAG_ITEM_CRYSTAL_4:
	case TAG_ITEM_CRYSTAL_5:
	case TAG_ITEM_CRYSTAL_6:
	case TAG_ITEM_CRYSTAL_7:
	case TAG_ITEM_CRYSTAL_8:
		pThis->transform.size = enemySize[2];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = enemySize[2];
		pThis->uv.offset = MakeFloat2((ENEMY_PIC_TYPE_5_SIZE_X + (ENEMY_PIC_TYPE_2_SIZE_X * 4)) / ENEMY_PIC_ALL_SIZE_X, (ENEMY_PIC_TYPE_2_SIZE_Y + (ENEMY_PIC_TYPE_1_SIZE_Y * 2)) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ITEM_PIC_TYPE_3_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ITEM_PIC_TYPE_3_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
		if (kind == TAG_ITEM_CRYSTAL_2) {
			pThis->uv.offset.y += pThis->uv.size.y;
		}
		else if (kind == TAG_ITEM_CRYSTAL_3) {
			pThis->uv.offset.y += (pThis->uv.size.y * 2);
		}
		else if (kind == TAG_ITEM_CRYSTAL_4) {
			pThis->uv.offset.y += (pThis->uv.size.y * 3);
		}
		else if (kind == TAG_ITEM_CRYSTAL_5) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
		}
		else if (kind == TAG_ITEM_CRYSTAL_6) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
			pThis->uv.offset.y += pThis->uv.size.y;
		}
		else if (kind == TAG_ITEM_CRYSTAL_7) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
			pThis->uv.offset.y += (pThis->uv.size.y * 2);
		}
		else if (kind == TAG_ITEM_CRYSTAL_8) {
			pThis->uv.offset.x += (pThis->uv.size.x * 3);
			pThis->uv.offset.y += (pThis->uv.size.y * 3);
		}
		break;
	case TAG_ITEM_CRYSTAL_BIG:
		pThis->transform.size = enemySize[3];
		pThis->collider.offset = MakeFloat2(0.0f, 0.0f);
		pThis->collider.size = enemySize[3];
		pThis->uv.offset = MakeFloat2(0.0f, ((ENEMY_PIC_TYPE_5_SIZE_Y * 3)) / ENEMY_PIC_ALL_SIZE_Y);
		pThis->uv.size = MakeFloat2((ITEM_PIC_TYPE_4_SIZE_X) / ENEMY_PIC_ALL_SIZE_X, (ITEM_PIC_TYPE_4_SIZE_Y) / ENEMY_PIC_ALL_SIZE_Y);
		break;
	case TAG_ITEM_LASTANCHOR:
	case TAG_ITEM_MUSICBOX_1:
		pThis->uv.offset = MakeFloat2(0.0f, 0.0f);
		pThis->uv.size = MakeFloat2(0.0f, 0.0f);
		pThis->collider.offset = Float2zero;
		pThis->collider.size = Float2zero;
		pThis->transform.Scale = Float2zero;
		pThis->collider.isActive = false;
		break;
	case TAG_ITEM_NULL:
	default:
		pThis->uv.offset = MakeFloat2(0.0f, 0.0f);
		pThis->uv.size = MakeFloat2(0.0f, 0.0f);
		pThis->transform.Scale = Float2zero;
		pThis->collider.isActive = false;
		pThis->tag = TAG_ITEM_NULL;
		break;
	}

	pThis->startPosition = MakeFloat2(posX, posY);
	pThis->transform.position = MakeFloat2(posX, posY);
	pThis->animator.anim->Init();
	pThis->controller.pMove->Init();
}

