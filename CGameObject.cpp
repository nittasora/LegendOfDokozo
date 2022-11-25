#include "CGameObject.h"

CGameObject::~CGameObject() {
	this->controller.UnInit();
	this->animator.UnInit();
}

void CGameObject::Init(FDATA_GAMEOBJECT initData) {
	this->transform = initData.transform;
	this->uv = initData.uv;
	this->animator.pOwner = this;
	this->controller.pOwner = this;
	this->collider.pOwner = this;
	if (!this->animator.Init(initData.animType)) {
		this->animator.isActive = false;
	}
	if (!this->controller.Init(initData.moveType)) {
		this->controller.isActive = false;
	}
	if (!this->collider.Init(initData.colliderType)) {
		this->collider.isActive = false;
	}
	this->localPosition = initData.localPosition;
	this->pOwner = initData.pOwner;
	this->tag = initData.tag;
	this->flg = 0;
	this->hp = initData.hp;
	this->sp = initData.sp;
	this->attack = initData.attack;
	this->startPosition = initData.transform.position;
	this->isActive = true;
}

FRECT CGameObject::GameObject_GetXY() {
	float sizeX = this->transform.size.x * this->transform.Scale.x;
	float sizeY = this->transform.size.y * this->transform.Scale.y;

	float leftX = this->transform.position.x + this->animator.pos.x - sizeX / 2;
	float topY = this->transform.position.y + this->animator.pos.y + sizeY / 2;
	float rightX = leftX + sizeX;
	float bottomY = topY - sizeY;

	FRECT rect = { leftX / (SCREEN_WIDTH), rightX / (SCREEN_WIDTH), topY / (SCREEN_HEIGHT), bottomY / (SCREEN_HEIGHT) };

	int stopFrame = this->collider.dontHitFrame % 2;

	if (stopFrame == 1 && this->tag != TAG_PLAYERATTACK) {
		rect = { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	return rect;
}

FRECT CGameObject::GameObject_GetUV() {
	float leftU = this->uv.offset.x + (float)this->animator.frameX * this->uv.size.x;
	float rightU = leftU + this->uv.size.x;
	float topV = this->uv.offset.y + (float)this->animator.frameY * this->uv.size.y;
	float bottomV = topV + this->uv.size.y;

	FRECT rect;

	switch (this->uv.flip) {
	case FLIP_NONE: // ”½“]‚È‚µ
		rect = { leftU, rightU, topV, bottomV };
		break;
	case FLIP_VERTICAL: // ‚’¼”½“]
		rect = { leftU, rightU, bottomV, topV };
		break;
	case FLIP_HORIZONTAL: // …•½”½“]
		rect = { rightU, leftU, topV, bottomV };
		break;
	}

	return rect;
}

void CGameObject::SetUV(FUVINFO *info) {
	this->uv = *info;
}
