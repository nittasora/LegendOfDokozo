#include "CCharaController.h"
#include "CGameObject.h"

#define DASH_CON_SP 1.0f
#define ATTACK_CON_SP 1.0f
#define STRONGATTACK_CON_SP 3.0f
#define AVOIDANCE_CON_SP 5.0f
#define AVOIDANCE_SPEED 200.0f
#define AVOIDANCE_FRAME 25

#define ACCELE_INC_SP 0.1f
#define ACCELE_MAX_INC_SP 50.0f

#define DIAGONAL_MOVE 0.75f

bool CCharaController::Init(int moveType) {

	if (this->isInit) {
		this->UnInit();
	}

	switch (moveType)
	{
	case MOVE_NONE:
		this->pMove = new CharaMove_None;
		this->isActive = false;
		this->moveSpeed = Float2zero;
		this->dirSpeed = Float2zero;
		this->maxMoveSpeed = 0.0f;
		this->accelForce = 0.0f;
		this->stopForce = 0.0f;
		break;
	case MOVE_BG_SCROLL_TOPDOWN:
		this->pMove = new CharaMove_BackGround_TopDown;
		this->isActive = false;
		this->moveSpeed = Float2zero;
		this->dirSpeed = Float2zero;
		this->maxMoveSpeed = 0.0f;
		this->accelForce = 10.0f;
		this->stopForce = 0.0f;
		break;
	case MOVE_PLAYER:
		this->pMove = new CharaMove_Player;
		this->isActive = true;
		this->moveSpeed = Float2zero;
		this->dirSpeed = Float2zero;
		this->maxMoveSpeed = 10.0f;
		this->accelForce = 3.0f;
		this->stopForce = 1.0f;
		break;
	case MOVE_ENEMY:
		this->pMove = new CharaMove_Enemy;
		this->isActive = true;
		this->moveSpeed = Float2zero;
		this->dirSpeed = Float2zero;
		this->maxMoveSpeed = 8.0f;
		this->accelForce = 3.0f;
		this->stopForce = 1.0f;
		break;
	case MOVE_ITEM:
		this->pMove = new CharaMove_Item;
		this->isActive = true;
		this->moveSpeed = Float2zero;
		this->dirSpeed = Float2zero;
		this->maxMoveSpeed = 8.0f;
		this->accelForce = 3.0f;
		this->stopForce = 1.0f;
		break;
	default:
		return false;
		break;
	}

	this->pMove->controller = this;

	this->isInit = true;

	return true;
}

void CCharaController::UnInit() {
	delete this->pMove;
	this->isInit = false;
}

void CCharaMove::Init() {

}

void CCharaMove::Update() {

}

void CCharaMove::DeathAct() {

}

void CCharaMove::SetKnockBack(CGameObject *pOther) {
	CGameObject *pThis = this->controller->pOwner;

	Float2 pThisCenter = pThis->transform.position + pThis->collider.offset;
	Float2 pOtherCenter = pOther->transform.position + pOther->collider.offset;

	pThis->hp.now -= pOther->attack.power;
	if (pThis->hp.now < 0) {
		pThis->hp.now = 0;
	}
	if (pOther->attack.power > 0.0f) {
		XA_Play(SOUND_LABEL_SE000);
	}

	float knockBackSpeed = pOther->attack.knockBack - pThis->hp.protectKnockBack;
	if (knockBackSpeed < 0) {
		knockBackSpeed = 0;
	}

	pThis->controller.moveSpeed = MYMath::UnitVector2D(&pOtherCenter, &pThisCenter);
	pThis->controller.moveSpeed = pThis->controller.moveSpeed * knockBackSpeed;

	int stopFrame = pOther->attack.stopFrame - pThis->hp.protectStop;
	if (stopFrame < 0) {
		stopFrame = 0;
	}

	this->frameCount[0] = stopFrame;
	pThis->collider.dontHitFrame = pOther->attack.dontHitFrame;
	pThis->game->play->SetStop(pOther->attack.stopFrame);

	pThis->flg |= OBJECT_FLG_KNOCKBACK;
	pThis->controller.isActive = false;
}

void CCharaMove::CheckHP() {
	if (this->controller->pOwner->hp.now <= 0) {
		this->DeathAct();
	}
}

void CCharaMove::AddForce() {
	this->controller->pOwner->transform.position = this->controller->pOwner->transform.position + this->controller->moveSpeed;

	if (this->controller->moveSpeed.x > 0) {
		this->controller->moveSpeed.x -= this->controller->stopForce;
		if (this->controller->moveSpeed.x < 0) {
			this->controller->moveSpeed.x = 0;
		}
	}
	else if (this->controller->moveSpeed.x < 0) {
		this->controller->moveSpeed.x += this->controller->stopForce;
		if (this->controller->moveSpeed.x > 0) {
			this->controller->moveSpeed.x = 0;
		}
	}

	if (this->controller->moveSpeed.y > 0) {
		this->controller->moveSpeed.y -= this->controller->stopForce;
		if (this->controller->moveSpeed.y < 0) {
			this->controller->moveSpeed.y = 0;
		}
	}
	else if (this->controller->moveSpeed.y < 0) {
		this->controller->moveSpeed.y += this->controller->stopForce;
		if (this->controller->moveSpeed.y > 0) {
			this->controller->moveSpeed.y = 0;
		}
	}
}

void CCharaMove::AddForce(const Float2 *addForce) {
	this->controller->pOwner->transform.position = this->controller->pOwner->transform.position + *addForce;
}

void CCharaMove::SlowDown(const float *downForce) {
	if (this->controller->moveSpeed.x > 0) {
		this->controller->moveSpeed.x -= *downForce;
		if (this->controller->moveSpeed.x < 0) {
			this->controller->moveSpeed.x = 0;
		}
	}
	else if (this->controller->moveSpeed.x < 0) {
		this->controller->moveSpeed.x += *downForce;
		if (this->controller->moveSpeed.x > 0) {
			this->controller->moveSpeed.x = 0;
		}
	}

	if (this->controller->moveSpeed.y > 0) {
		this->controller->moveSpeed.y -= *downForce;
		if (this->controller->moveSpeed.y < 0) {
			this->controller->moveSpeed.y = 0;
		}
	}
	else if (this->controller->moveSpeed.y < 0) {
		this->controller->moveSpeed.y += *downForce;
		if (this->controller->moveSpeed.y > 0) {
			this->controller->moveSpeed.y = 0;
		}
	}
}

void CharaMove_None::Update() {
	//“Á‚É‚È‚µ
}

void CharaMove_BackGround_TopDown::Update() {
	this->controller->pOwner->transform.position.y -= this->controller->accelForce;

	if (this->controller->pOwner->transform.position.y <= -this->controller->pOwner->transform.size.y) {
		this->controller->pOwner->transform.position.y += this->controller->pOwner->transform.size.y * 2;
	}
}

void CharaMove_Player::Update() {
	Float2 speed = Float2zero;
	CCharaController *con = this->controller;
	float maxSpeed = con->maxMoveSpeed;
	CGameObject *pOwner = this->controller->pOwner;
	int *flg = &pOwner->flg;
	float deltaTime = GameTimer_GetDeltaTime();

	if (this->controller->isActive) {

		pOwner->sp.now += this->incSp;
		if (pOwner->sp.now > pOwner->sp.max) {
			pOwner->sp.now = pOwner->sp.max;
		}
		this->incSp += deltaTime * ACCELE_INC_SP;
		if (this->incSp > ACCELE_MAX_INC_SP) {
			this->incSp = ACCELE_MAX_INC_SP;
		}

		int chipPosX = (int)(pOwner->transform.position.x / MAP_CHIP_SIZE);
		int chipPosY = (int)(pOwner->transform.position.y / MAP_CHIP_SIZE);
		if (chipPosX < 0) {
			chipPosX = 0;
		}
		if (chipPosY < 0) {
			chipPosY = 0;
		}
		if (chipPosX > MAP_SIZE_X - 1) {
			chipPosX = MAP_SIZE_X - 1;
		}
		if (chipPosY > MAP_SIZE_Y - 1) {
			chipPosY = MAP_SIZE_Y - 1;
		}
		if (pOwner->game->play->mapChip[chipPosX][chipPosY]->tag == TAG_WATER) {
			maxSpeed *= 0.5f;
		}

		this->controller->pOwner->animator.speed = PLAYER_ANIM_STAND_SPEED;

		if (Input_GetKeyPress(VK_LEFT)) {
			speed.x -= this->controller->accelForce;
			this->controller->pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
		}
		if (Input_GetKeyPress(VK_RIGHT)) {
			speed.x += this->controller->accelForce;
			this->controller->pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
		}
		if (Input_GetKeyPress(VK_DOWN)) {
			speed.y -= this->controller->accelForce;
			this->controller->pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
		}
		if (Input_GetKeyPress(VK_UP)) {
			speed.y += this->controller->accelForce;
			this->controller->pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
		}

		if (speed.x != 0.0f && speed.y != 0.0f) {
			speed = speed * DIAGONAL_MOVE;
		}

		if (speed.x != 0.0f || speed.y != 0.0f) {
			this->avoidSpeed = speed;
		}

		if (Input_GetKeyTrigger('X') && pOwner->sp.now >= AVOIDANCE_CON_SP) {
			pOwner->collider.dontHitFrame = AVOIDANCE_FRAME;
			maxSpeed *= 4.0f;
			speed.x = -this->avoidSpeed.x * AVOIDANCE_SPEED;
			speed.y = -this->avoidSpeed.y * AVOIDANCE_SPEED;
			this->controller->isActive = false;
			*flg |= OBJECT_FLG_AVOID;
			pOwner->sp.now -= AVOIDANCE_CON_SP;
			this->incSp = 0;
			this->frameCount[1] = 0;
		}
		else if (Input_GetKeyTrigger('S') && pOwner->sp.now >= STRONGATTACK_CON_SP) {
			speed = Float2zero;
			this->controller->isActive = false;
			*flg |= OBJECT_FLG_STRONGATTACK;
			pOwner->sp.now -= STRONGATTACK_CON_SP;
			this->incSp = 0;
			this->frameCount[2] = 0;
		}
		else if (Input_GetKeyTrigger('A') && pOwner->sp.now >= ATTACK_CON_SP) {
			speed = Float2zero;
			this->controller->isActive = false;
			*flg |= OBJECT_FLG_ATTACK;
			pOwner->sp.now -= ATTACK_CON_SP;
			this->incSp = 0;
			this->frameCount[3] = 0;
		}
		else if (Input_GetKeyPress(VK_SHIFT) && pOwner->sp.now >= deltaTime * DASH_CON_SP) {
			maxSpeed *= 1.5f;
			this->incSp = 0;
			pOwner->sp.now -= deltaTime * DASH_CON_SP;
			if (this->controller->pOwner->animator.speed == PLAYER_ANIM_WALK_SPEED) {
				this->controller->pOwner->animator.speed = PLAYER_ANIM_RUN_SPEED;
			}
		}

		con->moveSpeed = con->moveSpeed + speed;

		if (con->moveSpeed.x < -maxSpeed) {
			con->moveSpeed.x = -maxSpeed;
		}
		else if (con->moveSpeed.x > maxSpeed) {
			con->moveSpeed.x = maxSpeed;
		}

		if (con->moveSpeed.y < -maxSpeed) {
			con->moveSpeed.y = -maxSpeed;
		}
		else if (con->moveSpeed.y > maxSpeed) {
			con->moveSpeed.y = maxSpeed;
		}

		if (con->moveSpeed.y < 0) {
			if (con->moveSpeed.x < 0) {
				this->direction = SOUTH_WEST;
			}
			else if (con->moveSpeed.x > 0) {
				this->direction = SOUTH_EAST;
			}
			else if (fabsf(con->moveSpeed.x - con->moveSpeed.y) > this->controller->stopForce * 3) {
				this->direction = SOUTH;
			}
		}
		else if (con->moveSpeed.y > 0) {
			if (con->moveSpeed.x < 0) {
				this->direction = NORTH_WEST;
			}
			else if (con->moveSpeed.x > 0) {
				this->direction = NORTH_EAST;
			}
			else if (fabsf(con->moveSpeed.x - con->moveSpeed.y) > this->controller->stopForce * 3) {
				this->direction = NORTH;
			}
		}
		else {
			if (fabsf(con->moveSpeed.x - con->moveSpeed.y) > this->controller->stopForce * 3) {
				if (con->moveSpeed.x < 0) {
					this->direction = WEST;
				}
				else if (con->moveSpeed.x > 0) {
					this->direction = EAST;
				}
			}
		}

	}

	if (*flg & OBJECT_FLG_FALL) {
		pOwner->transform.Scale = pOwner->transform.Scale - MakeFloat2(0.05f, 0.05f);
		if (pOwner->transform.Scale.x < 0.0f && pOwner->transform.Scale.y < 0.0f) {
			if (pOwner->game->play->mapPointZ < (MAP_NUM - 1)) {
				pOwner->game->play->UpdataItem();
				pOwner->game->play->mapPointZ++;
				pOwner->game->play->fade = FADEOUT;
				pOwner->game->play->scene = PLAY_CHANGMAP;
				pOwner->game->play->bgm->XA_Stop_BGM();
			}
			else {
				pOwner->transform.Scale = MakeFloat2(1.0f, 1.0f);
				pOwner->game->play->SetAnchor(pOwner->game->play->mapPointZ);
				pOwner->hp.now -= 1.25f;
				XA_Play(SOUND_LABEL_SE000);
				pOwner->collider.dontHitFrame = 25;
				pOwner->collider.isActive = true;
				*flg &= ~OBJECT_FLG_FALL;
				this->controller->isActive = true;
			}
		}
	}
	else if (*flg & OBJECT_FLG_KNOCKBACK) {
		if (con->moveSpeed.x == 0.0f && con->moveSpeed.y == 0.0f) {
			if (this->frameCount[0] <= 0) {
				*flg &= ~OBJECT_FLG_KNOCKBACK;
				this->controller->isActive = true;
			}
			else {
				this->frameCount[0]--;
			}
		}
	}
	else if (*flg & OBJECT_FLG_AVOID) {
		if (this->frameCount[1] < 5) {
			this->frameCount[1]++;
		}
		else {
			this->SlowDown(&this->controller->stopForce);
			if (con->moveSpeed.y < 0) {
				if (con->moveSpeed.x < 0) {
					this->direction = NORTH_EAST;
				}
				else if (con->moveSpeed.x > 0) {
					this->direction = NORTH_WEST;
				}
				else {
					this->direction = NORTH;
				}
			}
			else if (con->moveSpeed.y > 0) {
				if (con->moveSpeed.x < 0) {
					this->direction = SOUTH_EAST;
				}
				else if (con->moveSpeed.x > 0) {
					this->direction = SOUTH_WEST;
				}
				else {
					this->direction = SOUTH;
				}
			}
			else {
				if (con->moveSpeed.x < 0) {
					this->direction = EAST;
				}
				else if (con->moveSpeed.x > 0) {
					this->direction = WEST;
				}
			}
			if (con->moveSpeed.x == 0.0f && con->moveSpeed.y == 0.0f) {
				*flg &= ~OBJECT_FLG_AVOID;
				this->controller->isActive = true;
			}
		}
	}
	else if (*flg & OBJECT_FLG_STRONGATTACK) {
		if (this->frameCount[2] == 0) {
			Float2 pos = Float2zero;
			pOwner->game->play->plAttackEffect->transform.size = { 256.0f, 256.0f };
			pOwner->game->play->plAttackEffect->collider.size = pOwner->game->play->plAttackEffect->transform.size / 2;
			Float2 pSize = pOwner->collider.size / 2;
			Float2 eSize = pOwner->game->play->plAttackEffect->collider.size / 2;
			pOwner->animator.pos = Float2zero;
			pOwner->game->play->plAttackEffect->attack.stopFrame = 10;
			pOwner->game->play->plAttackEffect->attack.power = 5;
			pOwner->game->play->plAttackEffect->attack.dontHitFrame = 45;
			pOwner->game->play->plAttackEffect->attack.knockBack = 50;
			con->moveSpeed = Float2zero;

			switch (this->direction)
			{
			case SOUTH:
				pos.y = -(pSize.y + eSize.y);
				pOwner->animator.pos.y = -10.0f;
				break;
			case WEST:
				pos.x = -(pSize.x + eSize.x);
				pOwner->animator.pos.x = -10.0f;
				break;
			case EAST:
				pos.x = (pSize.x + eSize.x);
				pOwner->animator.pos.x = 10.0f;
				break;
			case NORTH:
				pos.y = (pSize.y + eSize.y);
				pOwner->animator.pos.y = 10.0f;
				break;
			case SOUTH_WEST:
				pos.y = -(pSize.y + eSize.y);
				pos.x = -(pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = -7.0f;
				pOwner->animator.pos.x = -7.0f;
				break;
			case SOUTH_EAST:
				pos.y = -(pSize.y + eSize.y);
				pos.x = (pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = -7.0f;
				pOwner->animator.pos.x = 7.0f;
				break;
			case NORTH_WEST:
				pos.y = (pSize.y + eSize.y);
				pos.x = -(pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = 7.0f;
				pOwner->animator.pos.x = -7.0f;
				break;
			case NORTH_EAST:
			default:
				pos.y = (pSize.y + eSize.y);
				pos.x = (pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = 7.0f;
				pOwner->animator.pos.x = 7.0f;
				break;
			}

			pOwner->game->play->plAttackEffect->transform.position = pOwner->transform.position + pOwner->collider.offset + pos;
			pOwner->game->play->plAttackEffect->animator.anim->Start();
		}
		else if (this->frameCount[2] < 4) {
			pOwner->animator.pos.y *= 2.0f;
			pOwner->animator.pos.x *= 2.0f;
		}
		else if (this->frameCount[2] < 9) {

		}
		else if (this->frameCount[2] == 9) {
			pOwner->animator.pos.y = 0.0f;
			pOwner->animator.pos.x = 0.0f;
		}
		else if (this->frameCount[2] < 20) {
		}
		else if (this->frameCount[2] >= 30) {
			*flg &= ~OBJECT_FLG_STRONGATTACK;
			if (*flg == OBJECT_FLG_NONE) {
				this->controller->isActive = true;
			}
		}

		this->frameCount[2]++;
	}
	else if (*flg & OBJECT_FLG_ATTACK) {
		if (this->frameCount[3] == 0) {
			Float2 pos = Float2zero;
			pOwner->game->play->plAttackEffect->transform.size = { 512.0f, 512.0f };
			pOwner->game->play->plAttackEffect->collider.size = pOwner->game->play->plAttackEffect->transform.size / 2;
			Float2 pSize = pOwner->collider.size / 2;
			Float2 eSize = pOwner->game->play->plAttackEffect->collider.size / 2;
			pOwner->animator.pos = Float2zero;
			pOwner->game->play->plAttackEffect->attack.stopFrame = 2;
			pOwner->game->play->plAttackEffect->attack.power = 1;
			pOwner->game->play->plAttackEffect->attack.dontHitFrame = 45;
			pOwner->game->play->plAttackEffect->attack.knockBack = 25;
			con->moveSpeed = Float2zero;

			switch (this->direction)
			{
			case SOUTH:
				pos.y = -(pSize.y + eSize.y);
				pOwner->animator.pos.y = -10.0f;
				break;
			case WEST:
				pos.x = -(pSize.x + eSize.x);
				pOwner->animator.pos.x = -10.0f;
				break;
			case EAST:
				pos.x = (pSize.x + eSize.x);
				pOwner->animator.pos.x = 10.0f;
				break;
			case NORTH:
				pos.y = (pSize.y + eSize.y);
				pOwner->animator.pos.y = 10.0f;
				break;
			case SOUTH_WEST:
				pos.y = -(pSize.y + eSize.y);
				pos.x = -(pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = -7.0f;
				pOwner->animator.pos.x = -7.0f;
				break;
			case SOUTH_EAST:
				pos.y = -(pSize.y + eSize.y);
				pos.x = (pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = -7.0f;
				pOwner->animator.pos.x = 7.0f;
				break;
			case NORTH_WEST:
				pos.y = (pSize.y + eSize.y);
				pos.x = -(pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = 7.0f;
				pOwner->animator.pos.x = -7.0f;
				break;
			case NORTH_EAST:
			default:
				pos.y = (pSize.y + eSize.y);
				pos.x = (pSize.x + eSize.x);
				pos = pos * 0.75f;
				pOwner->animator.pos.y = 7.0f;
				pOwner->animator.pos.x = 7.0f;
				break;
			}

			pOwner->game->play->plAttackEffect->transform.position = pOwner->transform.position + pOwner->collider.offset + pos;
			pOwner->game->play->plAttackEffect->animator.anim->Start();
		}
		else if (this->frameCount[3] < 4) {
			pOwner->animator.pos.y *= 2.0f;
			pOwner->animator.pos.x *= 2.0f;
		}
		else if (this->frameCount[3] < 6) {

		}
		else if (this->frameCount[3] == 6) {
			pOwner->animator.pos.y = 0.0f;
			pOwner->animator.pos.x = 0.0f;
		}
		else if (this->frameCount[3] < 10) {
		}
		else {
			*flg &= ~OBJECT_FLG_ATTACK;
			if (*flg == OBJECT_FLG_NONE) {
				this->controller->isActive = true;
			}
		}

		this->frameCount[3]++;
	}
}

void CharaMove_Player::DeathAct() {
	if (this->controller->pOwner->game->play->scene == PLAY_NORMAL) {
		this->controller->pOwner->collider.dontHitFrame = 0;
		this->controller->pOwner->game->play->scene = PLAY_GAMEOVER;
	}
}

void CharaMove_Enemy::Update() {
	CGameObject *pOwner = this->controller->pOwner;
	CGameObject *player = pOwner->game->play->player;
	CPlay *pCPlay = pOwner->game->play;

	if (this->controller->isActive) {
		switch (this->controller->pOwner->tag)
		{
		case TAG_ENEMY_DESTROY:
			if (pOwner->transform.Scale.y <= 0) {
				int itemTag = TAG_ITEM_NULL;
				int checkHeart = (int)((player->hp.now / player->hp.max) * 100.0f);
				if (rand() % 100 < 100 - checkHeart) {
					itemTag = TAG_ITEM_HEART;
				}
				else if (rand() % 100 < 10) {
					itemTag = TAG_ITEM_COIN_1;
				}
				else if (rand() % 100 < 40) {
					itemTag = TAG_ITEM_COIN_2;
				}
				CObjGene::SetEnemyParameter(pOwner, TAG_ENEMY_NULL, pOwner->transform.position.x, pOwner->transform.position.y);
				for (int index = 0; index < ITEM_MAX; index++) {
					if (pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_NULL) {
						CObjGene::SetItemParameter(pCPlay->item[pCPlay->nowItemGroup][index], itemTag, pOwner->transform.position.x, pOwner->transform.position.y);
						break;
					}
				}
			}
			break;
		case TAG_ENEMY_SPIRIT_1:
			break;
		case TAG_ENEMY_SPIRIT_3:
			if (this->controller->moveSpeed.x == 0 && this->controller->moveSpeed.y == 0) {
				Float2 playerCenter = player->transform.position + player->collider.offset;
				Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
				this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
				this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;
			}
			this->AddForce();
			break;
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_6:
			this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;
			this->AddForce();
			break;
		case TAG_ENEMY_SPIRIT_4:
		{
			float rotation = this->controller->accelForce;
			float alpha = this->controller->stopForce;
			float radius = this->controller->maxMoveSpeed;
			this->controller->dirSpeed = MYMath::CalcuPosByDegree2D(radius, rotation);
			this->controller->accelForce += alpha;
			if (this->controller->accelForce >= 360.0f) {
				this->controller->accelForce -= 360.0f;
			}
			pOwner->transform.position = pOwner->startPosition + this->controller->dirSpeed;
		}
		break;
		case TAG_ENEMY_SPIRIT_5:
		{
			float rotation = this->controller->accelForce;
			float alpha = this->controller->stopForce;
			this->controller->maxMoveSpeed += 2.5f;
			float radius = this->controller->maxMoveSpeed;
			this->controller->dirSpeed = MYMath::CalcuPosByDegree2D(radius, rotation);
			this->controller->accelForce += alpha;
			if (this->controller->accelForce >= 360.0f) {
				this->controller->accelForce -= 360.0f;
			}
			pOwner->transform.position = pOwner->startPosition + this->controller->dirSpeed;
			if (radius >= SCREEN_WIDTH * 2) {
				CObjGene::SetEnemyParameter(pOwner, TAG_ENEMY_NULL, pOwner->transform.position.x, pOwner->transform.position.y);
			}
		}
		break;
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_SLIME:
		case TAG_ENEMY_ORC_1:
		case TAG_ENEMY_MOMMY:
			if (pOwner->tag == TAG_ENEMY_MOMMY) {
				pOwner->hp.now = pOwner->hp.max;
			}

			if (pOwner->flg & OBJECT_FLG_IDOL) {
				if (this->frame <= 0) {
					this->frame = (rand() % 20) + 20;
					int speedX = (rand() % (3)) - 1;
					int speedY = (rand() % (3)) - 1;
					this->controller->dirSpeed = MakeFloat2((float)speedX, (float)speedY);
					this->moveFrame = (rand() % 20) + 40;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_WALK;
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_WALK) {
				if (this->moveFrame <= 0) {
					if (this->controller->moveSpeed.x == 0.0f && this->controller->moveSpeed.y == 0.0f) {
						this->moveFrame = 0;
						pOwner->flg &= ~OBJECT_FLG_WALK;
						pOwner->flg |= OBJECT_FLG_IDOL;
					}
				}
				else {
					if (this->controller->dirSpeed.x == 0 || this->controller->dirSpeed.y == 0) {
						this->controller->moveSpeed = this->controller->moveSpeed + (this->controller->dirSpeed * this->controller->accelForce);
					}
					else {
						this->controller->moveSpeed = this->controller->moveSpeed + ((this->controller->dirSpeed * this->controller->accelForce) * DIAGONAL_MOVE);
					}
					this->SpeedCheck();
					this->moveFrame--;
				}
			}
			if (this->controller->dirSpeed.x != 0 || this->controller->dirSpeed.y != 0) {
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			else {
				pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			}
			this->DirCheck();
			this->AddForce();
			break;
		case TAG_ENEMY_GHOST_2:
		case TAG_ENEMY_ORC_2:
			if (pOwner->flg & OBJECT_FLG_IDOL) {
				if (this->frame <= 0) {
					this->frame = (rand() % 50) + 50;
					if (MYMath::CalcuDistance2D(&player->transform.position, &pOwner->transform.position) <= MAP_CHIP_SIZE * 3) {
						Float2 playerCenter = player->transform.position + player->collider.offset;
						Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
						this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
					}
					else {
						float rotation = (float)(rand() % 360);
						this->controller->dirSpeed = MYMath::CalcuPosByDegree2D(1, rotation);
					}
					this->moveFrame = (rand() % 20) + 40;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_WALK;
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_WALK) {
				if (this->moveFrame <= 0) {
					if (this->controller->moveSpeed.x == 0.0f && this->controller->moveSpeed.y == 0.0f) {
						this->moveFrame = 0;
						pOwner->flg &= ~OBJECT_FLG_WALK;
						pOwner->flg |= OBJECT_FLG_IDOL;
					}
				}
				else {
					this->controller->moveSpeed = this->controller->moveSpeed + (this->controller->dirSpeed * this->controller->accelForce);
					this->SpeedCheck();
					this->moveFrame--;
				}
			}
			if (this->controller->dirSpeed.x != 0 || this->controller->dirSpeed.y != 0) {
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			else {
				pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			}
			this->DirCheck();
			this->AddForce();
			break;
		case TAG_ENEMY_PLANT_1:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_PLANT_FLOWER:
			if (pOwner->flg & OBJECT_FLG_IDOL) {
				if (this->frame <= 0) {
					this->frame = 0;
					int speedX = (rand() % (3)) - 1;
					int speedY = (rand() % (3)) - 1;
					this->controller->dirSpeed = MakeFloat2((float)speedX, (float)speedY);
					this->moveFrame = (rand() % 20) + 40;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_WALK;
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_WALK) {
				if (this->moveFrame <= 0) {
					if (this->controller->moveSpeed.x == 0.0f && this->controller->moveSpeed.y == 0.0f) {
						this->moveFrame = 0;
						this->frame = (rand() % 20) + 20;
						pOwner->flg &= ~OBJECT_FLG_WALK;
						pOwner->flg |= OBJECT_FLG_IDOL;
					}
				}
				else {
					if (this->controller->dirSpeed.x == 0 || this->controller->dirSpeed.y == 0) {
						this->controller->moveSpeed = this->controller->moveSpeed + (this->controller->dirSpeed * this->controller->accelForce);
					}
					else {
						this->controller->moveSpeed = this->controller->moveSpeed + ((this->controller->dirSpeed * this->controller->accelForce) * DIAGONAL_MOVE);
					}
					this->SpeedCheck();
					this->moveFrame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_RUN) {
				if (this->controller->dirSpeed.x == 0 || this->controller->dirSpeed.y == 0) {
					this->controller->moveSpeed = this->controller->moveSpeed + (this->controller->dirSpeed * (this->controller->accelForce * 2));
				}
				else {
					this->controller->moveSpeed = this->controller->moveSpeed + ((this->controller->dirSpeed * (this->controller->accelForce * 2)) * DIAGONAL_MOVE);
				}
				this->SpeedCheck(this->controller->maxMoveSpeed * 2);
			}
			if (!(pOwner->flg & OBJECT_FLG_RUN)) {
				Float2 nextDir = Float2zero;
				bool isRun = false;
				if (player->transform.position.x - pOwner->transform.position.x > -(MAP_CHIP_SIZE / 2) && player->transform.position.x - pOwner->transform.position.x < (MAP_CHIP_SIZE / 2)) {
					isRun = true;
					if (player->transform.position.y - pOwner->transform.position.y < 0) {
						nextDir = MakeFloat2(0.0f, -1.0f);
					}
					else {
						nextDir = MakeFloat2(0.0f, 1.0f);
					}
				}
				else if (player->transform.position.y - pOwner->transform.position.y > -(MAP_CHIP_SIZE / 2) && player->transform.position.y - pOwner->transform.position.y < (MAP_CHIP_SIZE / 2)) {
					isRun = true;
					if (player->transform.position.x - pOwner->transform.position.x < 0) {
						nextDir = MakeFloat2(-1.0f, 0.0f);
					}
					else {
						nextDir = MakeFloat2(1.0f, 0.0f);
					}
				}
				if (isRun) {
					this->controller->moveSpeed = Float2zero;
					pOwner->flg &= ~OBJECT_FLG_WALK;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_RUN;
					this->controller->dirSpeed = nextDir;
				}
			}
			if (this->controller->dirSpeed.x != 0 || this->controller->dirSpeed.y != 0) {
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			else {
				pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			}
			this->DirCheck();
			this->AddForce();
			break;
		case TAG_ENEMY_SKULL:
			this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;
			this->AddForce();
			this->DirCheck();
			break;
		case TAG_ENEMY_DRAGON_1:
		case TAG_ENEMY_DRAGON_2:
		case TAG_ENEMY_DRAGON_3:
			if (pOwner->flg & OBJECT_FLG_IDOL) {
				if (this->moveFrame <= 0) {
					this->moveFrame = 0;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_WALK;
					this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;
				}
				else {
					this->moveFrame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_WALK) {
				if (this->controller->moveSpeed.x == 0 && this->controller->moveSpeed.y == 0) {
					this->moveFrame = rand() % 100;
					pOwner->flg &= ~OBJECT_FLG_WALK;
					pOwner->flg |= OBJECT_FLG_IDOL;
					this->controller->dirSpeed.y *= -1;
				}
			}

			if (!(pOwner->flg & OBJECT_FLG_FIRE)) {
				if (this->frame <= 0) {
					this->frame = 7;
					pOwner->flg |= OBJECT_FLG_FIRE;
					pOwner->animator.anim->Start();
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_FIRE) {
				if (this->frame <= 0) {
					this->frame = rand() % 200 + 100;
					pOwner->flg &= ~OBJECT_FLG_FIRE;

					int indexNum = 0;
					int index[3] = { 0, 0, 0 };
					int hitNum = 0;
					Float2 playerCenter = player->transform.position + player->collider.offset;
					Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
					float angle = MYMath::CalcuDegree2D(&ownerCenter, &playerCenter);

					for (int searchNum = 0; searchNum < 3; searchNum++) {
						for (indexNum; indexNum < ENEMY_MAX; indexNum++) {
							if (pCPlay->enemy[pCPlay->nowEnemyGroup][indexNum]->tag == TAG_ENEMY_NULL) {
								index[searchNum] = indexNum;
								hitNum++;
								indexNum++;
								break;
							}
						}
					}

					if (pOwner->tag == TAG_ENEMY_DRAGON_1 || rand() % 50 < 25) {
						for (int createNum = 0; createNum < hitNum; createNum++) {
							CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index[createNum]], TAG_ENEMY_SPIRIT_2, ownerCenter.x, ownerCenter.y);
						}
					}
					else if (pOwner->tag == TAG_ENEMY_DRAGON_2 || rand() % 50 < 25) {
						for (int createNum = 0; createNum < hitNum; createNum++) {
							CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index[createNum]], TAG_ENEMY_SPIRIT_6, ownerCenter.x, ownerCenter.y);
						}
					}
					else {
						for (int createNum = 0; createNum < hitNum; createNum++) {
							CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index[createNum]], TAG_ENEMY_SPIRIT_5, ownerCenter.x, ownerCenter.y);
						}
					}

					switch (hitNum)
					{
					case 2:
						pCPlay->enemy[pCPlay->nowEnemyGroup][index[0]]->controller.dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle - 15.0f);
						pCPlay->enemy[pCPlay->nowEnemyGroup][index[1]]->controller.dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle + 15.0f);
						break;
					case 3:
						pCPlay->enemy[pCPlay->nowEnemyGroup][index[2]]->controller.dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle - 15.0f);
						pCPlay->enemy[pCPlay->nowEnemyGroup][index[1]]->controller.dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle + 15.0f);
					case 1:
						pCPlay->enemy[pCPlay->nowEnemyGroup][index[0]]->controller.dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle);
						break;
					default:
						break;
					}
				}
				else {
					this->frame--;
				}
			}

			this->AddForce();
			this->DirCheck_Player();
			break;
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
			if (!(pOwner->flg & OBJECT_FLG_FIRE)) {
				if (this->frame <= 0) {
					this->frame = 7;
					pOwner->flg |= OBJECT_FLG_FIRE;
					pOwner->animator.anim->Start();
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_FIRE) {
				if (this->frame <= 0) {
					this->frame = rand() % 200 + 100;
					pOwner->flg &= ~OBJECT_FLG_FIRE;

					Float2 playerCenter = player->transform.position + player->collider.offset;
					Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;

					for (int index = 0; index < ENEMY_MAX; index++) {
						if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag == TAG_ENEMY_NULL) {
							if (pOwner->tag == TAG_ENEMY_EYE_1) {
								CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index], TAG_ENEMY_SPIRIT_2, ownerCenter.x, ownerCenter.y);
							}
							else {
								CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index], TAG_ENEMY_SPIRIT_5, ownerCenter.x, ownerCenter.y);
							}
							break;
						}
					}
				}
				else {
					this->frame--;
				}
			}
			this->DirCheck_Player();
			break;
		case TAG_ENEMY_LIZARD_1:
		case TAG_ENEMY_LIZARD_2:
			if (pOwner->flg & OBJECT_FLG_IDOL) {
				if (this->frame <= 0) {
					int speedX = (rand() % (3)) - 1;
					int speedY = (rand() % (3)) - 1;
					this->controller->dirSpeed = MakeFloat2((float)speedX, (float)speedY);
					this->frame = (rand() % 20) + 40;
					pOwner->flg &= ~OBJECT_FLG_IDOL;
					pOwner->flg |= OBJECT_FLG_WALK;
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_WALK) {
				if (this->frame <= 0) {
					if (this->controller->moveSpeed.x == 0.0f && this->controller->moveSpeed.y == 0.0f) {
						this->frame = (rand() % 20) + 20;
						pOwner->flg &= ~OBJECT_FLG_WALK;
						pOwner->flg |= OBJECT_FLG_IDOL;
					}
				}
				else {
					if (this->controller->dirSpeed.x == 0 || this->controller->dirSpeed.y == 0) {
						this->controller->moveSpeed = this->controller->moveSpeed + (this->controller->dirSpeed * this->controller->accelForce);
					}
					else {
						this->controller->moveSpeed = this->controller->moveSpeed + ((this->controller->dirSpeed * this->controller->accelForce) * DIAGONAL_MOVE);
					}
					this->SpeedCheck();
					this->frame--;
				}
			}
			if (this->controller->dirSpeed.x != 0 || this->controller->dirSpeed.y != 0) {
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			else {
				pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			}

			if (!(pOwner->flg & OBJECT_FLG_FIRE)) {
				if (this->moveFrame <= 0) {
					this->moveFrame = 7;
					pOwner->flg |= OBJECT_FLG_FIRE;
					pOwner->animator.anim->Start();
				}
				else {
					this->moveFrame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_FIRE) {
				if (this->moveFrame <= 0) {
					this->moveFrame = rand() % 200 + 100;
					pOwner->flg &= ~OBJECT_FLG_FIRE;

					int indexNum = 0;
					int index[3] = { 0, 0, 0 };
					int hitNum = 0;
					Float2 playerCenter = player->transform.position + player->collider.offset;
					Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
					float angle = MYMath::CalcuDegree2D(&ownerCenter, &playerCenter);

					for (int searchNum = 0; searchNum < 1; searchNum++) {
						for (indexNum; indexNum < ENEMY_MAX; indexNum++) {
							if (pCPlay->enemy[pCPlay->nowEnemyGroup][indexNum]->tag == TAG_ENEMY_NULL) {
								index[searchNum] = indexNum;
								hitNum++;
								indexNum++;
								break;
							}
						}
					}

					if (pOwner->tag == TAG_ENEMY_LIZARD_1) {
						for (int createNum = 0; createNum < hitNum; createNum++) {
							CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index[createNum]], TAG_ENEMY_SPIRIT_2, ownerCenter.x, ownerCenter.y);
						}
					}
					else {
						for (int createNum = 0; createNum < hitNum; createNum++) {
							CObjGene::SetEnemyParameter(pCPlay->enemy[pCPlay->nowEnemyGroup][index[createNum]], TAG_ENEMY_SPIRIT_6, ownerCenter.x, ownerCenter.y);
						}
					}
					if (this->controller->dirSpeed.x == 0.0f && this->controller->dirSpeed.y == 0.0f) {
						this->controller->dirSpeed = MYMath::CalcuPosByDegree2D(1.0, angle);
					}
					pCPlay->enemy[pCPlay->nowEnemyGroup][index[0]]->controller.dirSpeed = this->controller->dirSpeed;
				}
				else {
					this->moveFrame--;
				}
			}

			this->DirCheck();
			this->AddForce();	break;
		case TAG_ENEMY_DEATH:
			if (this->controller->moveSpeed.x == 0 && this->controller->moveSpeed.y == 0) {
				Float2 playerCenter = player->transform.position + player->collider.offset;
				Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
				this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
				this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;
			}
			this->DirCheck();
			this->AddForce();
			break;
		case TAG_ENEMY_GOLEM_1:
		case TAG_ENEMY_GOLEM_2:
			Float2 playerCenter = player->transform.position + player->collider.offset;
			Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
			this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
			this->controller->moveSpeed = this->controller->dirSpeed * this->controller->accelForce;

			if (!(pOwner->flg & OBJECT_FLG_FIRE)) {
				if (this->frame <= 0) {
					this->frame = 7;
					pOwner->flg |= OBJECT_FLG_FIRE;
					pOwner->animator.anim->Start();
				}
				else {
					this->frame--;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_FIRE) {
				if (this->frame <= 0) {
					this->frame = rand() % 200 + 100;
					pOwner->flg &= ~OBJECT_FLG_FIRE;

					player->controller.pMove->frameCount[0] = 5;

					player->flg |= OBJECT_FLG_KNOCKBACK;
					player->controller.isActive = false;
				}
				else {
					this->frame--;
				}
			}

			this->AddForce();
			this->DirCheck_Player();
			break;
		case TAG_ENEMY_SHADOW:
		{
			Float2 speed = Float2zero;
			pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			if (player->transform.position.x < pOwner->transform.position.x) {
				speed.x -= this->controller->accelForce;
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			if (player->transform.position.x > pOwner->transform.position.x) {
				speed.x += this->controller->accelForce;
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			if (player->transform.position.y < pOwner->transform.position.y) {
				speed.y -= this->controller->accelForce;
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}
			if (player->transform.position.y > pOwner->transform.position.y) {
				speed.y += this->controller->accelForce;
				pOwner->animator.speed = ENEMY_ANIM_RUN_SPEED;
			}

			if (speed.x != 0.0f && speed.y != 0.0f) {
				speed = speed * DIAGONAL_MOVE;
			}

			this->controller->moveSpeed = this->controller->moveSpeed + speed;

			if (this->controller->moveSpeed.x < -this->controller->maxMoveSpeed) {
				this->controller->moveSpeed.x = -this->controller->maxMoveSpeed;
			}
			else if (this->controller->moveSpeed.x > this->controller->maxMoveSpeed) {
				this->controller->moveSpeed.x = this->controller->maxMoveSpeed;
			}

			if (this->controller->moveSpeed.y < -this->controller->maxMoveSpeed) {
				this->controller->moveSpeed.y = -this->controller->maxMoveSpeed;
			}
			else if (this->controller->moveSpeed.y > this->controller->maxMoveSpeed) {
				this->controller->moveSpeed.y = this->controller->maxMoveSpeed;
			}
			this->AddForce();
			this->DirCheck_Player();
			if (player->controller.pMove->direction == this->direction) {
				pOwner->transform.Scale.y = 1.0f;
			}
			else {
				pOwner->transform.Scale.y = 0.0f;
			}
		}
		break;
		case TAG_ENEMY_ME:
		{
			float maxSpeed = this->controller->maxMoveSpeed;
			Float2 speed = Float2zero;
			pOwner->flg &= ~OBJECT_FLG_AVOID;
			pOwner->animator.speed = ENEMY_ANIM_STAND_SPEED;
			if (player->transform.position.x < pOwner->transform.position.x) {
				speed.x -= this->controller->accelForce;
				pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
			}
			if (player->transform.position.x > pOwner->transform.position.x) {
				speed.x += this->controller->accelForce;
				pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
			}
			if (player->transform.position.y < pOwner->transform.position.y) {
				speed.y -= this->controller->accelForce;
				pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
			}
			if (player->transform.position.y > pOwner->transform.position.y) {
				speed.y += this->controller->accelForce;
				pOwner->animator.speed = PLAYER_ANIM_WALK_SPEED;
			}

			if (speed.x != 0.0f && speed.y != 0.0f) {
				speed = speed * DIAGONAL_MOVE;
			}

			if (speed.x != 0.0f || speed.y != 0.0f) {
				this->controller->dirSpeed = speed;
			}

			if (Input_GetKeyTrigger('X') || Input_GetKeyTrigger('A')) {
				pOwner->collider.dontHitFrame = AVOIDANCE_FRAME;
				maxSpeed *= 4.0f;
				if (Input_GetKeyTrigger('A')) {
					speed.x = this->controller->dirSpeed.x * AVOIDANCE_SPEED;
					speed.y = this->controller->dirSpeed.y * AVOIDANCE_SPEED;
				}
				else {
					speed.x = -this->controller->dirSpeed.x * AVOIDANCE_SPEED;
					speed.y = -this->controller->dirSpeed.y * AVOIDANCE_SPEED;
				}
				this->controller->isActive = false;
				pOwner->flg |= OBJECT_FLG_AVOID;
				this->frameCount[1] = 0;
			}
			else if (Input_GetKeyPress(VK_SHIFT)) {
				maxSpeed *= 1.5f;
				if (this->controller->pOwner->animator.speed == PLAYER_ANIM_WALK_SPEED) {
					this->controller->pOwner->animator.speed = PLAYER_ANIM_RUN_SPEED;
				}
			}

			this->controller->moveSpeed = this->controller->moveSpeed + speed;

			if (this->controller->moveSpeed.x < -maxSpeed) {
				this->controller->moveSpeed.x = -maxSpeed;
			}
			else if (this->controller->moveSpeed.x > maxSpeed) {
				this->controller->moveSpeed.x = maxSpeed;
			}

			if (this->controller->moveSpeed.y < -maxSpeed) {
				this->controller->moveSpeed.y = -maxSpeed;
			}
			else if (this->controller->moveSpeed.y > maxSpeed) {
				this->controller->moveSpeed.y = maxSpeed;
			}

			this->AddForce();
			this->DirCheck_Player();

		}
		break;
		case TAG_ENEMY_NULL:
		default:
			break;
		}
	}

	if (pOwner->flg & OBJECT_FLG_FALL) {
		pOwner->transform.Scale = pOwner->transform.Scale - MakeFloat2(0.05f, 0.05f);
		if (pOwner->transform.Scale.x < 0.0f && pOwner->transform.Scale.y < 0.0f) {
			CObjGene::SetEnemyParameter(pOwner, TAG_ENEMY_NULL, 0, 0);
		}
	}
	else if (pOwner->flg & OBJECT_FLG_KNOCKBACK) {
		if (pOwner->controller.moveSpeed.x == 0.0f && pOwner->controller.moveSpeed.y == 0.0f) {
			if (this->frameCount[0] <= 0) {
				pOwner->flg &= ~OBJECT_FLG_KNOCKBACK;
				this->controller->isActive = true;
			}
			else {
				this->frameCount[0]--;
			}
		}
		else {
			this->AddForce();
		}
	}
	else if (pOwner->flg & OBJECT_FLG_AVOID) {
		if (this->frameCount[1] < 5) {
			this->frameCount[1]++;
		}
		else {
			this->SlowDown(&this->controller->stopForce);
			if (this->controller->moveSpeed.y < 0) {
				if (this->controller->moveSpeed.x < 0) {
					this->direction = NORTH_EAST;
				}
				else if (this->controller->moveSpeed.x > 0) {
					this->direction = NORTH_WEST;
				}
				else {
					this->direction = NORTH;
				}
			}
			else if (this->controller->moveSpeed.y > 0) {
				if (this->controller->moveSpeed.x < 0) {
					this->direction = SOUTH_EAST;
				}
				else if (this->controller->moveSpeed.x > 0) {
					this->direction = SOUTH_WEST;
				}
				else {
					this->direction = SOUTH;
				}
			}
			else {
				if (this->controller->moveSpeed.x < 0) {
					this->direction = EAST;
				}
				else if (this->controller->moveSpeed.x > 0) {
					this->direction = WEST;
				}
			}
			if (this->controller->moveSpeed.x == 0.0f && this->controller->moveSpeed.y == 0.0f) {
				pOwner->flg &= ~OBJECT_FLG_AVOID;
				this->controller->isActive = true;
			}
		}
		this->AddForce();
	}


}

void CharaMove_Enemy::Init() {
	CGameObject *pOwner = this->controller->pOwner;
	CGameObject *player = pOwner->game->play->player;

	Float2 playerCenter = player->transform.position + player->collider.offset;
	Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;

	this->controller->isActive = true;
	this->controller->dirSpeed = Float2zero;
	this->controller->moveSpeed = { 0.0f, 0.0f };
	pOwner->flg = OBJECT_FLG_NONE;
	for (int index = 0; index < OBJECT_FLG_NUM; index++) {
		this->controller->pMove->frameCount[index] = 0;
	}

	this->frame = 0;
	this->moveFrame = 0;

	switch (this->controller->pOwner->tag)
	{
	case TAG_ENEMY_SPIRIT_1:
		break;
	case TAG_ENEMY_SPIRIT_2:
	case TAG_ENEMY_SPIRIT_6:
		this->frame = 1000;
		this->controller->maxMoveSpeed = 15.0f;
		this->controller->accelForce = 15.0f;
		this->controller->stopForce = 1.0f;
		this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
		break;
	case TAG_ENEMY_SPIRIT_3:
		this->controller->maxMoveSpeed = 25.0f;
		this->controller->accelForce = 25.0f;
		this->controller->stopForce = 0.5f;
		break;
	case TAG_ENEMY_SPIRIT_4:
		this->controller->maxMoveSpeed = 200.0f;
		this->controller->accelForce = 0.0f;
		this->controller->stopForce = 5.0f;
		this->controller->dirSpeed = MakeFloat2(0.0f, 1.0f);
		pOwner->transform.position = pOwner->startPosition + MakeFloat2(this->controller->maxMoveSpeed, 0.0f);
		break;
	case TAG_ENEMY_SPIRIT_5:
		this->controller->maxMoveSpeed = 0.0f;
		this->controller->accelForce = 0.0f;
		this->controller->stopForce = 5.0f;
		this->controller->dirSpeed = MakeFloat2(0.0f, 1.0f);
		//pOwner->transform.position = pOwner->startPosition + MakeFloat2(this->controller->maxMoveSpeed, 0.0f);
		break;
	case TAG_ENEMY_SLIME:
	case TAG_ENEMY_MOMMY:
	case TAG_ENEMY_GHOST_1:
	case TAG_ENEMY_GHOST_2:
	case TAG_ENEMY_ORC_1:
	case TAG_ENEMY_ORC_2:
	case TAG_ENEMY_PLANT_1:
	case TAG_ENEMY_PLANT_2:
	case TAG_ENEMY_PLANT_FLOWER:
		this->controller->maxMoveSpeed = 10.0f;
		this->controller->accelForce = 3.0f;
		this->controller->stopForce = 1.0f;
		pOwner->flg = OBJECT_FLG_IDOL;
		break;
	case TAG_ENEMY_LIZARD_1:
	case TAG_ENEMY_LIZARD_2:
		this->frame = rand() % 30 + 30;
		this->moveFrame = rand() % 30 + 30;
		this->controller->maxMoveSpeed = 10.0f;
		this->controller->accelForce = 3.0f;
		this->controller->stopForce = 1.0f;
		pOwner->flg = OBJECT_FLG_IDOL;
		break;
	case TAG_ENEMY_SKULL:
		this->controller->maxMoveSpeed = 10.0f;
		this->controller->accelForce = 10.0f;
		this->controller->stopForce = 1.0f;
		this->controller->dirSpeed = MakeFloat2(1.0f, 1.0f);
		break;
	case TAG_ENEMY_DRAGON_1:
		this->controller->maxMoveSpeed = 10.0f;
		this->controller->accelForce = 10.0f;
		this->controller->stopForce = 0.5f;
		this->frame = rand() % 30 + 30;
		this->moveFrame = rand() % 30 + 30;
		this->controller->dirSpeed = MakeFloat2(0.0f, -1.0f);
		pOwner->flg = OBJECT_FLG_IDOL;
		break;
	case TAG_ENEMY_DEATH:
		this->controller->maxMoveSpeed = 20.0f;
		this->controller->accelForce = 20.0f;
		this->controller->stopForce = 0.25f;
		break;
	case TAG_ENEMY_GOLEM_1:
	case TAG_ENEMY_GOLEM_2:
		this->controller->maxMoveSpeed = 5.0f;
		this->controller->accelForce = 5.0f;
		this->controller->stopForce = 0.25f;
		break;
	case TAG_ENEMY_DRAGON_2:
	case TAG_ENEMY_DRAGON_3:
		this->frame = rand() % 30 + 30;
		this->moveFrame = rand() % 30 + 30;
		this->controller->dirSpeed = MakeFloat2(0.0f, -1.0f);
		pOwner->flg = OBJECT_FLG_IDOL;
		break;
	case TAG_ENEMY_ME:
		this->controller->maxMoveSpeed = 7.0f;
		this->controller->accelForce = 3.0f;
		this->controller->stopForce = 0.5f;
		this->controller->dirSpeed = MYMath::UnitVector2D(&ownerCenter, &playerCenter);
		break;
	case TAG_ENEMY_SHADOW:
		this->controller->maxMoveSpeed = 5.0f;
		this->controller->accelForce = 1.0f;
		this->controller->stopForce = 0.5f;
		break;
	case TAG_ENEMY_EYE_1:
	case TAG_ENEMY_EYE_2:
	case TAG_ENEMY_DESTROY:
	case TAG_ENEMY_NULL:
	default:
		this->controller->maxMoveSpeed = 0.0f;
		this->controller->accelForce = 0.0f;
		this->controller->stopForce = 0.0f;
		break;
	}
}

void CharaMove_Enemy::DeathAct() {
	CGameObject *pOwner = this->controller->pOwner;

	switch (pOwner->tag)
	{
	case TAG_ENEMY_NULL:
	case TAG_ENEMY_DESTROY:
	case TAG_ENEMY_SPIRIT_1:
	case TAG_ENEMY_SPIRIT_2:
	case TAG_ENEMY_SPIRIT_3:
	case TAG_ENEMY_SPIRIT_4:
	case TAG_ENEMY_SPIRIT_5:
	case TAG_ENEMY_SPIRIT_6:
	case TAG_ENEMY_SKULL:
	case TAG_ENEMY_DEATH:
		break;
	case TAG_ENEMY_PLANT_FLOWER:
		CObjGene::SetEnemyParameter(pOwner, TAG_ENEMY_PLANT_1, pOwner->transform.position.x, pOwner->transform.position.y);
		for (int i = 0; i < 2; i++) {
			for (int index = 0; index < ENEMY_MAX; index++) {
				if (pOwner->game->play->enemy[pOwner->game->play->nowEnemyGroup][index]->tag == TAG_ENEMY_NULL) {
					CObjGene::SetEnemyParameter(pOwner->game->play->enemy[pOwner->game->play->nowEnemyGroup][index], TAG_ENEMY_PLANT_1, pOwner->transform.position.x, pOwner->transform.position.y);
					break;
				}
			}
		}
		break;
	default:
		CObjGene::SetEnemyParameter(pOwner, TAG_ENEMY_DESTROY, pOwner->transform.position.x, pOwner->transform.position.y);
		break;
	}
}

void CharaMove_Enemy::DirCheck() {
	CCharaController *con = this->controller;

	if (con->moveSpeed.y < 0) {
		if (con->moveSpeed.x < 0) {
			this->direction = SOUTH_WEST;
		}
		else if (con->moveSpeed.x > 0) {
			this->direction = SOUTH_EAST;
		}
		else {
			this->direction = SOUTH;
		}
	}
	else if (con->moveSpeed.y > 0) {
		if (con->moveSpeed.x < 0) {
			this->direction = NORTH_WEST;
		}
		else if (con->moveSpeed.x > 0) {
			this->direction = NORTH_EAST;
		}
		else {
			this->direction = NORTH;
		}
	}
	else {
		if (con->moveSpeed.x < 0) {
			this->direction = WEST;
		}
		else if (con->moveSpeed.x > 0) {
			this->direction = EAST;
		}
	}
}

void CharaMove_Enemy::DirCheck_Player() {
	CCharaController *con = this->controller;
	float angle = MYMath::CalcuDegree2D(&con->pOwner->game->play->player->transform.position, &con->pOwner->transform.position);

	if (angle < -157.5f) {
		this->direction = EAST;
	}
	else if (angle < -112.5f) {
		this->direction = NORTH_EAST;
	}
	else if (angle < -67.5f) {
		this->direction = NORTH;
	}
	else if (angle < -22.5f) {
		this->direction = NORTH_WEST;
	}
	else if (angle < 22.5f) {
		this->direction = WEST;
	}
	else if (angle < 67.5f) {
		this->direction = SOUTH_WEST;
	}
	else if (angle < 112.5f) {
		this->direction = SOUTH;
	}
	else if (angle < 157.5f) {
		this->direction = SOUTH_EAST;
	}
	else {
		this->direction = EAST;
	}
}

void CharaMove_Enemy::DirCheck_Roll() {
	CCharaController *con = this->controller;

	switch (this->direction)
	{
	case SOUTH:
		this->direction = SOUTH_WEST;
		break;
	case SOUTH_WEST:
		this->direction = WEST;
		break;
	case WEST:
		this->direction = NORTH_WEST;
		break;
	case NORTH_WEST:
		this->direction = NORTH;
		break;
	case NORTH:
		this->direction = NORTH_EAST;
		break;
	case NORTH_EAST:
		this->direction = EAST;
		break;
	case EAST:
		this->direction = SOUTH_EAST;
		break;
	case SOUTH_EAST:
	default:
		this->direction = SOUTH;
		break;
		break;
	}
}

void CharaMove_Enemy::SpeedCheck() {
	if (this->controller->moveSpeed.x > this->controller->maxMoveSpeed) {
		this->controller->moveSpeed.x = this->controller->maxMoveSpeed;
	}
	if (this->controller->moveSpeed.y > this->controller->maxMoveSpeed) {
		this->controller->moveSpeed.y = this->controller->maxMoveSpeed;
	}
	if (this->controller->moveSpeed.x < -this->controller->maxMoveSpeed) {
		this->controller->moveSpeed.x = -this->controller->maxMoveSpeed;
	}
	if (this->controller->moveSpeed.y < -this->controller->maxMoveSpeed) {
		this->controller->moveSpeed.y = -this->controller->maxMoveSpeed;
	}
}

void CharaMove_Enemy::SpeedCheck(float maxMoveSpeed) {
	if (this->controller->moveSpeed.x > maxMoveSpeed) {
		this->controller->moveSpeed.x = maxMoveSpeed;
	}
	if (this->controller->moveSpeed.y > maxMoveSpeed) {
		this->controller->moveSpeed.y = maxMoveSpeed;
	}
	if (this->controller->moveSpeed.x < -maxMoveSpeed) {
		this->controller->moveSpeed.x = -maxMoveSpeed;
	}
	if (this->controller->moveSpeed.y < -maxMoveSpeed) {
		this->controller->moveSpeed.y = -maxMoveSpeed;
	}
}

void CharaMove_Item::Update() {
	CGameObject *pOwner = this->controller->pOwner;
	CGameObject *player = pOwner->game->play->player;
	CPlay *pCPlay = pOwner->game->play;

	if (this->controller->isActive) {
		switch (this->controller->pOwner->tag)
		{
		case TAG_ITEM_FLOOR_KEY:
		{
			bool enemyAlive = false;
			for (int index = 0; index < ENEMY_MAX; index++) {
				if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
					enemyAlive = true;
				}
			}
			if (!enemyAlive) {
				CObjGene::SetItemParameter(pOwner, TAG_ITEM_KEY, pOwner->transform.position.x, pOwner->transform.position.y);
			}
		}
		break;
		case TAG_ITEM_FLOOR_STATES_UP:
		{
			bool enemyAlive = false;
			for (int index = 0; index < ENEMY_MAX; index++) {
				if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
					enemyAlive = true;
				}
			}
			if (!enemyAlive) {
				CObjGene::SetItemParameter(pOwner, TAG_ITEM_STATES_UP, pOwner->transform.position.x, pOwner->transform.position.y);
			}
		}
		break;
		case TAG_ITEM_DOOR_KEY:
		case TAG_ITEM_DOOR_CRYSTAL_1:
		case TAG_ITEM_DOOR_CRYSTAL_2:
		case TAG_ITEM_DOOR_CRYSTAL_3:
		case TAG_ITEM_DOOR_CRYSTAL_4:
		case TAG_ITEM_DOOR_CRYSTAL_5:
		case TAG_ITEM_DOOR_CRYSTAL_6:
		case TAG_ITEM_DOOR_CRYSTAL_7:
		case TAG_ITEM_DOOR_CRYSTAL_8:
			if (pOwner->flg == OBJECT_FLG_NONE) {
				pOwner->flg &= ~OBJECT_FLG_NONE;
				pOwner->flg |= OBJECT_FLG_CLOSE_START;
			}
			else if (pOwner->flg & OBJECT_FLG_CLOSE_START) {
				pOwner->flg &= ~OBJECT_FLG_CLOSE_START;
				pOwner->flg |= OBJECT_FLG_CLOSE_NOW;
			}
			else if (pOwner->flg & OBJECT_FLG_OPEN) {
				CObjGene::SetItemParameter(pOwner, TAG_ITEM_NULL, pOwner->transform.position.x, pOwner->transform.position.y);
			}
			break;
		case TAG_ITEM_DOOR_LOCK:
			if (pOwner->flg == OBJECT_FLG_NONE) {
				pOwner->flg &= ~OBJECT_FLG_NONE;
				pOwner->flg |= OBJECT_FLG_CLOSE_START;
			}
			else if (pOwner->flg & OBJECT_FLG_CLOSE_START) {
				bool enemyAlive = false;
				for (int index = 0; index < ENEMY_MAX; index++) {
					if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
						enemyAlive = true;
					}
				}

				bool blockAlive = false;
				for (int index = 0; index < ITEM_MAX; index++) {
					if (pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_1 || pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_2) {
						if (!(pCPlay->item[pCPlay->nowItemGroup][index]->flg & OBJECT_FLG_MOVE_END)) {
							blockAlive = true;
						}
					}
				}

				pOwner->flg &= ~OBJECT_FLG_CLOSE_START;

				if (enemyAlive || blockAlive) {
					pOwner->flg |= OBJECT_FLG_CLOSE_NOW;
				}
				else {
					pOwner->flg |= OBJECT_FLG_OPEN;
				}
			}
			else if (pOwner->flg & OBJECT_FLG_CLOSE) {
				bool enemyAlive = false;
				for (int index = 0; index < ENEMY_MAX; index++) {
					if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
						enemyAlive = true;
					}
				}

				bool blockAlive = false;
				for (int index = 0; index < ITEM_MAX; index++) {
					if (pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_1 || pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_2) {
						if (!(pCPlay->item[pCPlay->nowItemGroup][index]->flg & OBJECT_FLG_MOVE_END)) {
							blockAlive = true;
						}
					}
				}

				if (!enemyAlive && !blockAlive) {
					pOwner->flg &= ~OBJECT_FLG_CLOSE;
					pOwner->flg |= OBJECT_FLG_OPEN_NOW;
				}
			}
			break;
		case TAG_ITEM_MOVE_BLOCK_1:
		case TAG_ITEM_MOVE_BLOCK_2:
			if (pOwner->flg & OBJECT_FLG_MOVE_END) {
				break;
			}
			if (!(pOwner->flg & OBJECT_FLG_MOVE_START)) {
				int chipPosX = (int)(pOwner->transform.position.x / MAP_CHIP_SIZE);
				int chipPosY = (int)(pOwner->transform.position.y / MAP_CHIP_SIZE);
				if (chipPosX < 0) {
					chipPosX = 0;
				}
				if (chipPosY < 0) {
					chipPosY = 0;
				}
				if (chipPosX > MAP_SIZE_X - 1) {
					chipPosX = MAP_SIZE_X - 1;
				}
				if (chipPosY > MAP_SIZE_Y - 1) {
					chipPosY = MAP_SIZE_Y - 1;
				}
				int upPos = chipPosY + 1;
				int downPos = chipPosY - 1;
				int rightPos = chipPosX + 1;
				int leftPos = chipPosX - 1;
				if (upPos < 0) {
					upPos = 0;
				}
				if (downPos < 0) {
					downPos = 0;
				}
				if (rightPos < 0) {
					rightPos = 0;
				}
				if (leftPos < 0) {
					leftPos = 0;
				}
				if (upPos > MAP_SIZE_Y - 1) {
					upPos = MAP_SIZE_Y - 1;
				}
				if (downPos > MAP_SIZE_Y - 1) {
					downPos = MAP_SIZE_Y - 1;
				}
				if (rightPos > MAP_SIZE_X - 1) {
					rightPos = MAP_SIZE_X - 1;
				}
				if (leftPos > MAP_SIZE_X - 1) {
					leftPos = MAP_SIZE_X - 1;
				}

				if (pCPlay->mapChip[chipPosX][upPos]->tag == TAG_FLOOR && pCPlay->mapChip[chipPosX][downPos]->tag == TAG_FLOOR) {
					pOwner->flg |= OBJECT_FLG_HEIGHT_MOVE;
				}
				else {
					pOwner->flg &= ~OBJECT_FLG_HEIGHT_MOVE;
				}
				if (pCPlay->mapChip[rightPos][chipPosY]->tag == TAG_FLOOR && pCPlay->mapChip[leftPos][chipPosY]->tag == TAG_FLOOR) {
					pOwner->flg |= OBJECT_FLG_WIDTH_MOVE;
				}
				else {
					pOwner->flg &= ~OBJECT_FLG_WIDTH_MOVE;
				}

				bool enemyAlive = false;
				for (int index = 0; index < ENEMY_MAX; index++) {
					if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
						enemyAlive = true;
					}
				}
				if (!enemyAlive) {
					pOwner->flg |= OBJECT_FLG_MOVE_START;
				}
			}
			break;
		case TAG_ITEM_FLOOR_UP:
		case TAG_ITEM_FLOOR_DOWN:
			if (!(pOwner->flg & OBJECT_FLG_OPEN)) {
				bool enemyAlive = false;
				for (int index = 0; index < ENEMY_MAX; index++) {
					if (pCPlay->enemy[pCPlay->nowEnemyGroup][index]->tag != TAG_ENEMY_NULL) {
						enemyAlive = true;
					}
				}

				bool blockAlive = false;
				for (int index = 0; index < ITEM_MAX; index++) {
					if (pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_1 || pCPlay->item[pCPlay->nowItemGroup][index]->tag == TAG_ITEM_MOVE_BLOCK_2) {
						if (!(pCPlay->item[pCPlay->nowItemGroup][index]->flg & OBJECT_FLG_MOVE_END)) {
							blockAlive = true;
						}
					}
				}

				if (!enemyAlive && !blockAlive) {
					pOwner->flg |= OBJECT_FLG_OPEN;
					pOwner->animator.frameX = 0;
					XA_Play(SOUND_LABEL_SE013);
				}
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
		case TAG_ITEM_STATES_UP:
		case TAG_ITEM_NECKLACE:
		case TAG_ITEM_STATES_UP_SHOP:
			if (pOwner->flg & OBJECT_FLG_GET) {
				CObjGene::SetItemParameter(pOwner, TAG_ITEM_NULL, pOwner->transform.position.x, pOwner->transform.position.y);
			}
			break;
		case TAG_ITEM_HEART_SHOP:
		case TAG_ITEM_KEY_SHOP:
			if (pOwner->flg & OBJECT_FLG_GET) {
				pOwner->collider.isActive = false;
				pOwner->animator.frameX = 2;
				pOwner->flg &= ~OBJECT_FLG_GET;
			}
			break;
		case TAG_ITEM_LASTANCHOR:
			if (pOwner->flg == OBJECT_FLG_NONE) {
				pCPlay->mapLastPointX = pCPlay->mapPointX;
				pCPlay->mapLastPointY = pCPlay->mapPointY;
				pCPlay->mapLastPointZ = pCPlay->mapPointZ;
				pOwner->flg = OBJECT_FLG_CLOSE;
			}
			break;
		case TAG_ITEM_MUSICBOX_1:
			if (pOwner->flg == OBJECT_FLG_NONE) {
				pOwner->flg = OBJECT_FLG_CLOSE;
			}
			break;
		default:
			break;
		}
	}

	if (pOwner->flg & OBJECT_FLG_KNOCKBACK) {
		if (pOwner->controller.moveSpeed.x == 0.0f && pOwner->controller.moveSpeed.y == 0.0f) {
			if (this->frameCount[0] <= 0) {
				pOwner->flg &= ~OBJECT_FLG_KNOCKBACK;
				this->controller->isActive = true;
			}
			else {
				this->frameCount[0]--;
			}
		}
		else {
			this->AddForce();
		}
	}

}

void CharaMove_Item::Init() {
	CGameObject *pOwner = this->controller->pOwner;
	CGameObject *player = pOwner->game->play->player;

	this->controller->isActive = true;
	this->controller->dirSpeed = Float2zero;
	this->controller->moveSpeed = Float2zero;
	pOwner->flg = OBJECT_FLG_NONE;
	for (int index = 0; index < OBJECT_FLG_NUM; index++) {
		this->controller->pMove->frameCount[index] = 0;
	}
	this->controller->maxMoveSpeed = 0.0f;
	this->controller->accelForce = 0.0f;
	this->controller->stopForce = 10000.0f;
	this->controller->dirSpeed = Float2zero;
}

void CharaMove_Item::DeathAct() {
	CGameObject *pOwner = this->controller->pOwner;
}

