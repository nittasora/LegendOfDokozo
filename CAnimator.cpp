#include "CAnimator.h"
#include "CGameObject.h"

bool CAnimator::Init(int animType) {

	if (this->isInit) {
		this->UnInit();
	}

	this->frameX = 0;
	this->frameY = 0;
	this->pos = Float2zero;
	this->time = 0;

	switch (animType)
	{
	case ANIM_NONE:
		this->anim = new Anim_None;
		this->speed = 0;
		this->isActive = false;
		break;
	case ANIM_PLAYER:
		this->anim = new Anim_Player;
		this->speed = 1;
		this->isActive = true;
		break;
	case ANIM_PLATTACKEFFECT:
		this->anim = new Anim_PlAttackEffect;
		this->speed = 15;
		this->isActive = true;
		break;
	case ANIM_ENEMY:
		this->anim = new Anim_Enemy;
		this->speed = 15;
		this->isActive = true;
		break;
	case ANIM_ITEM:
		this->anim = new Anim_ITEM;
		this->speed = 15;
		this->isActive = true;
		break;
	default:
		return false;
		break;
	}

	this->anim->animator = this;

	this->isInit = true;

	return true;
}

void CAnimator::UnInit() {
	delete this->anim;
	this->isInit = false;
}

void CAnim::Update() {

}

void CAnim::Start() {

}

void CAnim::Init() {

}

void Anim_None::Update() {
	// “Á‚É‚È‚µ
}

void Anim_Player::Update() {
	int *flg = &this->animator->pOwner->flg;

	this->animator->time += GameTimer_GetDeltaTime() * this->animator->speed;
	this->framCount = (int)this->animator->time % PLAYER_STEP_MAX;

	if (this->stepAnim[this->framCount] == ANIMATION_LOOP) {
		this->framCount = 0;
		this->animator->time = 0;
	}

	switch (this->animator->pOwner->controller.pMove->direction)
	{
	case SOUTH:
		this->animator->frameX = 0;
		this->animator->frameY = 0;
		break;
	case WEST:
		this->animator->frameX = 0;
		this->animator->frameY = 1;
		break;
	case EAST:
		this->animator->frameX = 0;
		this->animator->frameY = 2;
		break;
	case NORTH:
		this->animator->frameX = 0;
		this->animator->frameY = 3;
		break;
	case SOUTH_WEST:
		this->animator->frameX = 3;
		this->animator->frameY = 0;
		break;
	case SOUTH_EAST:
		this->animator->frameX = 3;
		this->animator->frameY = 1;
		break;
	case NORTH_WEST:
		this->animator->frameX = 3;
		this->animator->frameY = 2;
		break;
	case NORTH_EAST:
	default:
		this->animator->frameX = 3;
		this->animator->frameY = 3;
		break;
	}

	if (*flg & OBJECT_FLG_AVOID) {

	}
	else if (*flg & OBJECT_FLG_STRONGATTACK) {

	}
	else if (*flg & OBJECT_FLG_ATTACK) {

	}
	else {
		this->animator->frameX += this->stepAnim[this->framCount];
	}

}

void Anim_Enemy::Update() {
	CGameObject *pOwner = this->animator->pOwner;

	this->subTime += GameTimer_GetDeltaTime() * this->animator->speed;
	// –¾‚ç‚©‚ÉŽžŠÔ‚ª—§‚¿‚·‚¬‚Ä‚¢‚é
	if (this->subTime > 10000.0f) {
		this->subTime = 0.0f;
	}

	switch (this->animator->pOwner->tag)
	{
	case TAG_ENEMY_DESTROY:
		if (this->subTime < ENEMY_ANIM_DESTROY_ROTATION_TIME) {
			pOwner->controller.pMove->direction = this->destroyAnim[(int)this->subTime % (ENEMY_DESTROY_MAX - 1)];
		}
		else if (pOwner->controller.pMove->direction != SOUTH) {
			pOwner->controller.pMove->direction = this->destroyAnim[(int)this->subTime % (ENEMY_DESTROY_MAX - 1)];
		}
		else {
			pOwner->transform.Scale.x -= 0.05f;
			pOwner->transform.Scale.y -= 0.05f;
			if (pOwner->transform.Scale.y < 0) {
				pOwner->transform.Scale.x = 0.0f;
				pOwner->transform.Scale.y = 0.0f;
			}
		}
		this->CommonAnim();
		break;
	case TAG_ENEMY_DRAGON_1:
	case TAG_ENEMY_DRAGON_2:
	case TAG_ENEMY_DRAGON_3:
	case TAG_ENEMY_EYE_1:
	case TAG_ENEMY_EYE_2:
	case TAG_ENEMY_GOLEM_1:
	case TAG_ENEMY_GOLEM_2:
	case TAG_ENEMY_LIZARD_1:
	case TAG_ENEMY_LIZARD_2:
		if (this->subFramCount < 11) {
			if (this->subFramCount < 3) {
				this->animator->pos.y = 20.0f;
			}
			else if (this->subFramCount < 7) {
				this->animator->pos.y = 80.0f;
			}
			else if (this->subFramCount < 10) {
				this->animator->pos.y = 20.0f;
			}
			else {
				this->animator->pos.y = 0.0f;
			}
			this->subFramCount++;
		}
		this->CommonAnim();
		break;
		//case TAG_ENEMY_SPIRIT_1:
		//case TAG_ENEMY_SPIRIT_2:
		//case TAG_ENEMY_SPIRIT_3:
		//case TAG_ENEMY_SPIRIT_4:
		//case TAG_ENEMY_SPIRIT_5:
		//case TAG_ENEMY_SPIRIT_6:
		//case TAG_ENEMY_ORC_1:
		//case TAG_ENEMY_ORC_2:
		//case TAG_ENEMY_PLANT_1:
		//case TAG_ENEMY_PLANT_2:
		//case TAG_ENEMY_PLANT_FLOWER:
		//case TAG_ENEMY_SKULL:
		//case TAG_ENEMY_GHOST_1:
		//case TAG_ENEMY_GHOST_2:
		//case TAG_ENEMY_EYE_2:
		//case TAG_ENEMY_DEATH:
		//case TAG_ENEMY_GOLEM_2:
		//case TAG_ENEMY_DRAGON_2:
		//case TAG_ENEMY_DRAGON_3:
		//case TAG_ENEMY_SHADOW:
		//case TAG_ENEMY_ME:
		//	this->CommonAnim();
		//	break;
	case TAG_ENEMY_NULL:
		break;
	default:
		this->CommonAnim();
		break;
	}
}

void Anim_Enemy::CommonAnim() {
	int *flg = &this->animator->pOwner->flg;

	this->animator->time += GameTimer_GetDeltaTime() * this->animator->speed;
	this->framCount = (int)this->animator->time % ENEMY_STEP_MAX;

	if (this->stepAnim[this->framCount] == ANIMATION_LOOP) {
		this->framCount = 0;
		this->animator->time = 0;
	}

	switch (this->animator->pOwner->controller.pMove->direction)
	{
	case SOUTH:
		this->animator->frameX = 0;
		this->animator->frameY = 0;
		break;
	case WEST:
		this->animator->frameX = 0;
		this->animator->frameY = 1;
		break;
	case EAST:
		this->animator->frameX = 0;
		this->animator->frameY = 2;
		break;
	case NORTH:
		this->animator->frameX = 0;
		this->animator->frameY = 3;
		break;
	case SOUTH_WEST:
		this->animator->frameX = 3;
		this->animator->frameY = 0;
		break;
	case SOUTH_EAST:
		this->animator->frameX = 3;
		this->animator->frameY = 1;
		break;
	case NORTH_WEST:
		this->animator->frameX = 3;
		this->animator->frameY = 2;
		break;
	case NORTH_EAST:
	default:
		this->animator->frameX = 3;
		this->animator->frameY = 3;
		break;
	}

	this->animator->frameX += this->stepAnim[this->framCount];

}

void Anim_Enemy::Start() {
	this->subFramCount = 0;
}

void Anim_Enemy::Init() {
	CGameObject *pOwner = this->animator->pOwner;

	this->framCount = 0;
	this->subFramCount = 0;
	this->animator->time = 0;
	this->subTime = 0;
	this->animator->frameY = 0;
	this->animator->frameX = 0;
	pOwner->controller.pMove->direction = SOUTH;

	switch (this->animator->pOwner->tag)
	{
	case TAG_ENEMY_SPIRIT_1:
		pOwner->controller.pMove->direction = SOUTH;
		this->animator->frameY = 0;
		this->animator->frameX = 0;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_SPIRIT_2:
		pOwner->controller.pMove->direction = WEST;
		this->animator->frameY = 1;
		this->animator->frameX = 0;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_SPIRIT_3:
		pOwner->controller.pMove->direction = EAST;
		this->animator->frameY = 2;
		this->animator->frameX = 0;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_SPIRIT_4:
		pOwner->controller.pMove->direction = NORTH;
		this->animator->frameY = 3;
		this->animator->frameX = 0;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_SPIRIT_5:
		pOwner->controller.pMove->direction = SOUTH_WEST;
		this->animator->frameY = 0;
		this->animator->frameX = 3;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_SPIRIT_6:
		pOwner->controller.pMove->direction = SOUTH_EAST;
		this->animator->frameY = 1;
		this->animator->frameX = 3;
		this->animator->speed = ENEMY_ANIM_RUN_SPEED;
		break;
	case TAG_ENEMY_DRAGON_1:
	case TAG_ENEMY_DRAGON_2:
	case TAG_ENEMY_DRAGON_3:
	case TAG_ENEMY_LIZARD_1:
	case TAG_ENEMY_LIZARD_2:
	case TAG_ENEMY_EYE_1:
	case TAG_ENEMY_EYE_2:
	case TAG_ENEMY_GOLEM_1:
		this->subFramCount = 11;
		break;
	default:
		break;
	}
}

void Anim_ITEM::Update() {
	CGameObject *pOwner = this->animator->pOwner;
	CPlay *pCPlay = pOwner->game->play;

	this->subTime += GameTimer_GetDeltaTime() * this->animator->speed;
	// –¾‚ç‚©‚ÉŽžŠÔ‚ª—§‚¿‚·‚¬‚Ä‚¢‚é
	if (this->subTime > 10000.0f) {
		this->subTime = 0.0f;
	}

	switch (pOwner->tag)
	{
	case TAG_ITEM_DOOR_KEY:
	case TAG_ITEM_DOOR_LOCK:
	case TAG_ITEM_DOOR_CRYSTAL_1:
	case TAG_ITEM_DOOR_CRYSTAL_2:
	case TAG_ITEM_DOOR_CRYSTAL_3:
	case TAG_ITEM_DOOR_CRYSTAL_4:
	case TAG_ITEM_DOOR_CRYSTAL_5:
	case TAG_ITEM_DOOR_CRYSTAL_6:
	case TAG_ITEM_DOOR_CRYSTAL_7:
	case TAG_ITEM_DOOR_CRYSTAL_8:
		if (pOwner->flg & OBJECT_FLG_CLOSE_NOW) {
			this->framCount++;

			if (this->animator->frameX == 3) {
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE002);
			}
			if (this->framCount >= 4) {
				this->framCount = 0;
				this->animator->frameX -= 1;
				if (this->animator->frameX <= 0) {
					this->animator->frameX = 0;
					pOwner->flg &= ~OBJECT_FLG_CLOSE_NOW;
					pOwner->flg |= OBJECT_FLG_CLOSE;
				}
			}
		}
		else if (pOwner->flg & OBJECT_FLG_OPEN_NOW) {
			this->framCount++;

			if (this->animator->frameX == 0) {
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE003);
			}
			if (this->framCount >= 4) {
				this->framCount = 0;
				this->animator->frameX += 1;
				if (this->animator->frameX >= 3) {
					this->animator->frameX = 3;
					pOwner->flg &= ~OBJECT_FLG_OPEN_NOW;
					pOwner->flg |= OBJECT_FLG_OPEN;
				}
			}
		}
		break;
	case TAG_ITEM_HEART_SHOP:
	case TAG_ITEM_KEY_SHOP:
	case TAG_ITEM_STATES_UP_SHOP:
	case TAG_ITEM_FLOOR_KEY:
	case TAG_ITEM_FLOOR_STATES_UP:
	case TAG_ITEM_MOVE_BLOCK_1:
	case TAG_ITEM_MOVE_BLOCK_2:
	case TAG_ITEM_FLOOR_DOWN:
	case TAG_ITEM_FLOOR_UP:
	case TAG_ITEM_LASTANCHOR:
	case TAG_ITEM_MUSICBOX_1:
		break;
	default:
		this->CommonAnim();
		break;
	}
}

void Anim_ITEM::CommonAnim() {
	int *flg = &this->animator->pOwner->flg;

	this->animator->time += GameTimer_GetDeltaTime() * this->animator->speed;
	this->framCount = (int)this->animator->time % ENEMY_STEP_MAX;

	if (this->stepAnim[this->framCount] == ANIMATION_LOOP) {
		this->framCount = 0;
		this->animator->time = 0;
	}

	this->animator->frameX = this->stepAnim[this->framCount];

}

void Anim_ITEM::Init() {
	CGameObject *pOwner = this->animator->pOwner;

	this->framCount = 0;
	this->animator->time = 0;
	this->subTime = 0;
	this->animator->frameY = 0;
	this->animator->frameX = 0;
	this->animator->pos = Float2zero;
	pOwner->controller.pMove->direction = SOUTH;
	this->animator->speed = ITEM_ANIM_STAND_SPEED;

	switch (this->animator->pOwner->tag)
	{
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
		this->animator->frameX = 3;
		break;
	case TAG_ITEM_FLOOR_DOWN:
	case TAG_ITEM_FLOOR_UP:
		this->animator->frameX = 1;
		break;
	default:
		break;
	}
}

void Anim_PlAttackEffect::Start() {
	this->framCount = 0;
	this->animator->time = 0;
	this->animator->isActive = true;
	this->animator->pOwner->isActive = true;
	XA_Play(SOUND_LABEL_SE001);
}

void Anim_PlAttackEffect::Update() {
	if (!this->animator->isActive) {
		return;
	}

	this->animator->time += GameTimer_GetDeltaTime() * this->animator->speed;
	this->framCount = (int)this->animator->time;

	if (this->framCount >= PLAYER_ATTACK_EFFECT_STEP_MAX - 1) {
		this->framCount = 0;
		this->animator->time = 0;
		this->animator->frameX = 0;
		this->animator->frameY = 0;
		this->animator->isActive = false;
		this->animator->pOwner->isActive = false;
		this->animator->pOwner->collider.isActive = false;
	}
	else if (this->framCount == 2) {
		this->animator->pOwner->collider.isActive = true;
	}
	else {
		this->animator->frameX = this->animFrame[this->framCount] % 4;
		this->animator->frameY = this->animFrame[this->framCount] / 4;
	}

}
