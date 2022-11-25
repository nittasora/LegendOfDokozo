#pragma once

#include "CGameObject.h"

enum BACKGROUND_TYPE
{
	MAPCHIP,
	GAGE,
	GAMEOVER_BACKGROUND,
	GAMEOVER_TEXT,
	GAMECLEAR_BG,
	SYSTEM_TEXT
};

namespace CObjGene
{
	// èâä˙âªä÷åW
	void CreateInvisibleObject(CGameObject*, float, float, float, float);
	void CreateBackGround(CGameObject*, int, int);
	void CreatePlayer(CGameObject*, float, float);
	void CreatePLAtackEffect(CGameObject*, CGameObject*);
	void CreateNullEnemy(CGameObject*);
	void CreateNullItem(CGameObject*);
	void CreateSysObj(CGameObject*);
	void CreateSysMapPoint(CGameObject*);
	void CreateFade(CGameObject*);
	void SetEnemyParameter(CGameObject*, int, float, float);
	void SetItemParameter(CGameObject*, int, float, float);
	void CreateNumber(CGameObject*);
	void CreateTitleBack(CGameObject*, int);
	void CreateTitleText(CGameObject*, int);
}

