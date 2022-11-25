#include "CCollider.h"
#include "CGameObject.h"

bool CCollider::Init(int colliderType) {
	this->dontHitFrame = 0;

	switch (colliderType)
	{
	case COLLIDER_NONE:
		this->isActive = false;
		this->offset = Float2zero;
		this->size = Float2zero;
		break;
	case COLLIDER_TRANSFORM:
		this->isActive = true;
		this->offset = Float2zero;
		this->size = this->pOwner->transform.size;
		break;
	case COLLIDER_PLAYER:
		this->isActive = true;
		this->offset = MakeFloat2(0.0f, -30.0f);
		this->size = MakeFloat2(60.0f, 96.0f);
		break;
	default:
		return false;
		break;
	}

	return true;
}

void CCollider::Update() {
	if (this->dontHitFrame > 0) {
		this->dontHitFrame--;
	}
}

FRECT CCollider::Collider_GetXY() {
	float leftX = this->pOwner->transform.position.x + this->offset.x - this->size.x / 2;
	float topY = this->pOwner->transform.position.y + this->offset.y + this->size.y / 2;
	float rightX = leftX + this->size.x;
	float bottomY = topY - this->size.y;

	FRECT rect = { leftX, rightX, topY, bottomY };

	return rect;
}

void CCollider::HitReaction_Play(CGameObject *hitObject, COLLIDER_RESULT *result) {
	CGameObject *myPOwner = this->pOwner;
	CPlay *pCPlay = myPOwner->game->play;

	switch (myPOwner->tag)
	{
	case TAG_PLAYER:
		//case TAG_PLAYER_CAN_WALK_WATER:
		switch (hitObject->tag)
		{
		case TAG_ENEMY_NULL:
		case TAG_ENEMY_DESTROY:
		case TAG_ITEM_NULL:
		case TAG_ITEM_FLOOR_KEY:
		case TAG_ITEM_FLOOR_STATES_UP:
			break;
		case TAG_ENEMY_DEATH:
			if (myPOwner->collider.dontHitFrame > 0) {
				break;
			}
			pCPlay->UpdataItem();
			pCPlay->mapPointX = pCPlay->mapLastPointX;
			pCPlay->mapPointY = pCPlay->mapLastPointY;
			pCPlay->mapPointZ = pCPlay->mapLastPointZ;
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			pCPlay->bgm->XA_Stop_BGM();
			myPOwner->controller.pMove->SetKnockBack(hitObject);
			break;
		case TAG_ITEM_HEART:
			pOwner->hp.now += 5.0f;
			if (pOwner->hp.now > pOwner->hp.max) {
				pOwner->hp.now = pOwner->hp.max;
			}
			CObjGene::SetItemParameter(hitObject, TAG_ITEM_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE008);
			break;
		case TAG_ITEM_COIN_1:
			pCPlay->haveGoldNum += 4;
		case TAG_ITEM_COIN_2:
			pCPlay->haveGoldNum += 1;
			if (pCPlay->haveGoldNum > GOLD_MAX) {
				pCPlay->haveGoldNum = GOLD_MAX;
			}
			CObjGene::SetItemParameter(hitObject, TAG_ITEM_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE009);
			break;
		case TAG_ITEM_KEY:
			pCPlay->haveKeyNum += 1;
			if (pCPlay->haveKeyNum > KEY_MAX) {
				pCPlay->haveKeyNum = KEY_MAX;
			}
			CObjGene::SetItemParameter(hitObject, TAG_ITEM_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE010);
			break;
		case TAG_ITEM_STATES_UP:
		case TAG_ITEM_NECKLACE:
			if (hitObject->flg == OBJECT_FLG_NONE) {
				hitObject->flg = OBJECT_FLG_GET;
				hitObject->transform.position.y = pOwner->transform.position.y + (pOwner->collider.size.y / 2) + (hitObject->collider.size.y / 2);
				hitObject->transform.position.x = pOwner->transform.position.x;
				pOwner->animator.frameX = 1;
				pOwner->animator.frameY = 3;
				pOwner->controller.pMove->direction = SOUTH;
				if (hitObject->tag == TAG_ITEM_STATES_UP) {
					pOwner->hp.max += 5;
					if (pOwner->hp.max > 100) {
						pOwner->hp.max = 100;
					}
					pOwner->hp.now = pOwner->hp.max;
				}
				else {
					pCPlay->canWalkWater = true;
				}
				pOwner->controller.moveSpeed = Float2zero;
				hitObject->transform.Scale.y = 0.0f;
				pCPlay->getItem = hitObject;
				pCPlay->scene = PLAY_ITEM_GET;
				pCPlay->bgm->XA_Stop_BGM_Once();
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE005);
			}
			break;
		case TAG_ITEM_HEART_SHOP:
		case TAG_ITEM_KEY_SHOP:
		case TAG_ITEM_STATES_UP_SHOP:
			if (hitObject->flg == OBJECT_FLG_NONE) {
				if (hitObject->tag == TAG_ITEM_STATES_UP_SHOP) {
					if (pCPlay->haveGoldNum < 100) {
						break;
					}
					else {
						pCPlay->haveGoldNum -= 100;
						pOwner->hp.max += 5;
						if (pOwner->hp.max > 100) {
							pOwner->hp.max = 100;
						}
						pOwner->hp.now = pOwner->hp.max;
					}
				}
				else if (hitObject->tag == TAG_ITEM_KEY_SHOP) {
					if (pCPlay->haveGoldNum < 40) {
						break;
					}
					else {
						pCPlay->haveGoldNum -= 40;
						pCPlay->haveKeyNum += 1;
						if (pCPlay->haveKeyNum > KEY_MAX) {
							pCPlay->haveKeyNum = KEY_MAX;
						}
					}
				}
				else {
					if (pCPlay->haveGoldNum < 10) {
						break;
					}
					else {
						pCPlay->haveGoldNum -= 10;
						pOwner->hp.now += 5;
						if (pOwner->hp.now > pOwner->hp.max) {
							pOwner->hp.now = pOwner->hp.max;
						}
					}
				}

				hitObject->flg = OBJECT_FLG_GET;
				Float2 position = MakeFloat2(pOwner->transform.position.x - hitObject->transform.position.x, pOwner->transform.position.y + (pOwner->collider.size.y / 2) + (hitObject->collider.size.y / 2) - hitObject->transform.position.y);
				hitObject->animator.pos = position;
				hitObject->animator.frameX = 1;
				pOwner->animator.frameX = 1;
				pOwner->animator.frameY = 3;
				pOwner->controller.pMove->direction = SOUTH;
				pOwner->controller.moveSpeed = Float2zero;
				hitObject->transform.Scale.y = 0.0f;
				pCPlay->getItem = hitObject;
				pCPlay->scene = PLAY_ITEM_GET;
				pCPlay->bgm->XA_Stop_BGM_Once();
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE005);
			}
			break;
		case TAG_ITEM_MOVE_BLOCK_1:
		case TAG_ITEM_MOVE_BLOCK_2:
			if (hitObject->flg & OBJECT_FLG_MOVE_START) {
				if (hitObject->flg & OBJECT_FLG_MOVE_START) {
					if (hitObject->flg & OBJECT_FLG_WIDTH_MOVE) {
						if (result->d.y == 0.0f) {
							hitObject->flg &= ~OBJECT_FLG_HEIGHT_MOVE;
							myPOwner->controller.moveSpeed = Float2zero;
							hitObject->animator.pos.x += result->d.x;
							hitObject->collider.offset.x += result->d.x;
							if (hitObject->animator.pos.x < -hitObject->collider.size.x) {
								hitObject->animator.pos.x = -hitObject->collider.size.x;
								hitObject->flg &= ~OBJECT_FLG_MOVE_START;
								hitObject->flg |= OBJECT_FLG_MOVE_END;
							}
							if (hitObject->animator.pos.x > hitObject->collider.size.x) {
								hitObject->animator.pos.x = hitObject->collider.size.x;
								hitObject->flg &= ~OBJECT_FLG_MOVE_START;
								hitObject->flg |= OBJECT_FLG_MOVE_END;
							}
						}
						else {
							myPOwner->transform.position = myPOwner->transform.position - result->d;
						}
					}
					if (hitObject->flg & OBJECT_FLG_HEIGHT_MOVE) {
						if (result->d.x == 0.0f) {
							hitObject->flg &= ~OBJECT_FLG_WIDTH_MOVE;
							myPOwner->controller.moveSpeed = Float2zero;
							hitObject->animator.pos.y += result->d.y;
							hitObject->collider.offset.y += result->d.y;
							if (hitObject->animator.pos.y < -hitObject->collider.size.y) {
								hitObject->animator.pos.y = -hitObject->collider.size.y;
								hitObject->flg &= ~OBJECT_FLG_MOVE_START;
								hitObject->flg |= OBJECT_FLG_MOVE_END;
							}
							if (hitObject->animator.pos.y > hitObject->collider.size.y) {
								hitObject->animator.pos.y = hitObject->collider.size.y;
								hitObject->flg &= ~OBJECT_FLG_MOVE_START;
								hitObject->flg |= OBJECT_FLG_MOVE_END;
							}
						}
						else {
							myPOwner->transform.position = myPOwner->transform.position - result->d;
						}
					}
					else {
						myPOwner->transform.position = myPOwner->transform.position - result->d;
					}
				}

			}
			else {
				myPOwner->transform.position = myPOwner->transform.position - result->d;
			}
			break;
		case TAG_ITEM_DOOR_KEY:
			if (hitObject->flg & OBJECT_FLG_CLOSE_START) {
				hitObject->flg = OBJECT_FLG_NONE;
			}
			else if (hitObject->flg & OBJECT_FLG_CLOSE) {
				if (pCPlay->haveKeyNum > 0) {
					pCPlay->haveKeyNum--;
					hitObject->flg &= ~OBJECT_FLG_CLOSE;
					hitObject->flg |= OBJECT_FLG_OPEN_NOW;
				}
				else {
					myPOwner->transform.position = myPOwner->transform.position - result->d;
				}
			}
			else if (!(hitObject->flg & OBJECT_FLG_OPEN)) {
				myPOwner->transform.position = myPOwner->transform.position - result->d;
			}
			break;
		case TAG_ITEM_DOOR_CRYSTAL_1:
		case TAG_ITEM_DOOR_CRYSTAL_2:
		case TAG_ITEM_DOOR_CRYSTAL_3:
		case TAG_ITEM_DOOR_CRYSTAL_4:
		case TAG_ITEM_DOOR_CRYSTAL_5:
		case TAG_ITEM_DOOR_CRYSTAL_6:
		case TAG_ITEM_DOOR_CRYSTAL_7:
		case TAG_ITEM_DOOR_CRYSTAL_8:
			if (hitObject->flg & OBJECT_FLG_CLOSE_START) {
				hitObject->flg = OBJECT_FLG_NONE;
			}
			else if (hitObject->flg & OBJECT_FLG_CLOSE) {
				if (pCPlay->haveCrystal[hitObject->tag - TAG_ITEM_DOOR_CRYSTAL_1] > 0) {
					hitObject->flg &= ~OBJECT_FLG_CLOSE;
					hitObject->flg |= OBJECT_FLG_OPEN_NOW;
				}
				else {
					myPOwner->transform.position = myPOwner->transform.position - result->d;
				}
			}
			else if (!(hitObject->flg & OBJECT_FLG_OPEN)) {
				myPOwner->transform.position = myPOwner->transform.position - result->d;
			}
			break;
		case TAG_ITEM_DOOR_LOCK:
			if (hitObject->flg & OBJECT_FLG_CLOSE_START) {
				hitObject->flg = OBJECT_FLG_NONE;
			}
			else if (!(hitObject->flg & OBJECT_FLG_OPEN)) {
				myPOwner->transform.position = myPOwner->transform.position - result->d;
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
			if (hitObject->flg == OBJECT_FLG_NONE) {
				hitObject->flg = OBJECT_FLG_GET;
				hitObject->transform.position.y = pOwner->transform.position.y + (pOwner->collider.size.y / 2) + (hitObject->collider.size.y / 2);
				hitObject->transform.position.x = pOwner->transform.position.x;
				pOwner->animator.frameX = 1;
				pOwner->animator.frameY = 0;
				pOwner->controller.pMove->direction = SOUTH;
				pCPlay->haveCrystal[hitObject->tag - TAG_ITEM_CRYSTAL_1] = 1;
				hitObject->transform.Scale.y = 0.0f;
				pCPlay->getItem = hitObject;
				pCPlay->scene = PLAY_CRYSTAL_GET;
				pCPlay->bgm->XA_Stop_BGM_Once();
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE005);
			}
			break;
		case TAG_ITEM_CRYSTAL_BIG:
			if (hitObject->flg == OBJECT_FLG_NONE) {
				hitObject->flg = OBJECT_FLG_GET;
				pOwner->animator.frameX = 1;
				pOwner->animator.frameY = 0;
				pOwner->controller.pMove->direction = SOUTH;
				pOwner->transform.position.x = CDraw::camera.x;
				pOwner->transform.position.y = CDraw::camera.y;
				hitObject->transform.position.y = pOwner->transform.position.y + (pOwner->collider.size.y / 2) + (hitObject->collider.size.y / 2);
				hitObject->transform.position.x = pOwner->transform.position.x;
				hitObject->transform.Scale.y = 0.0f;
				pCPlay->mapCamera = CDraw::camera;
				pCPlay->mapCamera.y = pCPlay->mapCamera.y - (SCREEN_HEIGHT * 2) - 1432.0f;
				pCPlay->getItem = hitObject;
				pCPlay->scene = PLAY_GAMECLEAR;
				pCPlay->bgm->XA_Stop_BGM_Once();
				pCPlay->bgm->XA_Play_SE(SOUND_LABEL_SE004);
			}
			break;
		case TAG_ITEM_FLOOR_UP:
		case TAG_ITEM_FLOOR_DOWN:
			if (hitObject->flg & OBJECT_FLG_OPEN) {
				pCPlay->UpdataItem();
				if (hitObject->tag == TAG_ITEM_FLOOR_UP) {
					pCPlay->mapPointZ--;
					if (pCPlay->mapPointZ < 0) {
						pCPlay->mapPointZ = 0;
					}
				}
				else {
					pCPlay->mapPointZ++;
					if (pCPlay->mapPointZ > MAP_NUM - 1) {
						pCPlay->mapPointZ = MAP_NUM - 1;
					}
				}
				pCPlay->fade = FADEOUT;
				pCPlay->scene = PLAY_CHANGMAP;
				XA_Play(SOUND_LABEL_SE006);
				pCPlay->bgm->XA_Stop_BGM();
			}
			break;
		default:
			if (myPOwner->collider.dontHitFrame > 0) {
				break;
			}
			myPOwner->controller.pMove->SetKnockBack(hitObject);
			break;
		}
		break;
	case TAG_PLAYERATTACK:
		if (hitObject->collider.dontHitFrame > 0) {
			break;
		}
		switch (hitObject->tag)
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
		default:
			hitObject->controller.pMove->SetKnockBack(myPOwner);
			break;
		}
		break;
	case TAG_WALL:
		switch (hitObject->tag)
		{
		case TAG_PLAYERATTACK:
			if (myPOwner->hp.max > -1) {
				myPOwner->hp.now -= hitObject->attack.power;
				if (myPOwner->hp.now <= 0) {
					if (myPOwner->animator.frameY == MAPCHIP_NUM - 2 || myPOwner->animator.frameY == MAPCHIP_NUM - 5) {
						myPOwner->hp = { -1, -1 };
						myPOwner->animator.frameY++;
						if (myPOwner->animator.frameX >= MAPCHIP_NUM - 4) {
							myPOwner->tag = TAG_FLOOR_DOWN_1;
						}
						else if (myPOwner->animator.frameX >= MAPCHIP_NUM - 8) {
							myPOwner->tag = TAG_FLOOR_DOWN_2;
						}
						else if (myPOwner->animator.frameX >= MAPCHIP_NUM - 12) {
							myPOwner->tag = TAG_FLOOR_UP_1;
						}
						else if (myPOwner->animator.frameX >= MAPCHIP_NUM - 16) {
							myPOwner->tag = TAG_FLOOR_UP_2;
						}
						else {
							myPOwner->tag = TAG_FLOOR;
						}
					}
					else if (myPOwner->animator.frameY == MAPCHIP_NUM - 3) {
						myPOwner->hp = { 4, 4 };
						myPOwner->animator.frameY++;
					}
					else if (myPOwner->animator.frameY == MAPCHIP_NUM - 6) {
						myPOwner->hp = { 9, 9 };
						myPOwner->animator.frameY++;
					}
				}
			}
			pCPlay->SetStop(hitObject->attack.stopFrame);
			pCPlay->plAttackEffect->collider.dontHitFrame = pCPlay->plAttackEffect->attack.dontHitFrame;
			break;
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_DESTROY:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
		case TAG_ENEMY_EYE_2:
			break;
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_6:
			CObjGene::SetEnemyParameter(hitObject, TAG_ENEMY_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			break;
		case TAG_ENEMY_SKULL:
			if (result->d.x == 0.0f) {
				hitObject->controller.dirSpeed.y *= -1;
			}
			else {
				hitObject->controller.dirSpeed.x *= -1;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_ORC_1:
		case TAG_ENEMY_ORC_2:
		case TAG_ENEMY_LIZARD_1:
		case TAG_ENEMY_LIZARD_2:
			if (hitObject->flg & OBJECT_FLG_WALK) {
				hitObject->flg &= ~OBJECT_FLG_WALK;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_PLANT_1:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_PLANT_FLOWER:
			if (hitObject->flg & OBJECT_FLG_RUN) {
				hitObject->flg &= ~OBJECT_FLG_RUN;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_INVISIBLE_WALL:
		switch (hitObject->tag)
		{
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_PLAYER:
			break;
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_6:
			CObjGene::SetEnemyParameter(hitObject, TAG_ENEMY_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			break;
		case TAG_ENEMY_SKULL:
			if (result->d.x == 0.0f) {
				hitObject->controller.dirSpeed.y *= -1;
			}
			else {
				hitObject->controller.dirSpeed.x *= -1;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_SLIME:
		case TAG_ENEMY_ORC_1:
		case TAG_ENEMY_ORC_2:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
		case TAG_ENEMY_LIZARD_1:
		case TAG_ENEMY_LIZARD_2:
			if (hitObject->flg & OBJECT_FLG_WALK) {
				hitObject->flg &= ~OBJECT_FLG_WALK;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_PLANT_1:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_PLANT_FLOWER:
			if (hitObject->flg & OBJECT_FLG_RUN) {
				hitObject->flg &= ~OBJECT_FLG_RUN;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_HOLE:
		switch (hitObject->tag)
		{
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		case TAG_PLAYER:
		{
			Float2 hitCenter = hitObject->transform.position + hitObject->collider.offset;
			Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
			if (hitCenter.x >= ownerCenter.x - (pOwner->collider.size.x / 2) && hitCenter.x <= ownerCenter.x + (pOwner->collider.size.x / 2)) {
				if (hitCenter.y >= ownerCenter.y - (pOwner->collider.size.y / 2) && hitCenter.y <= ownerCenter.y + (pOwner->collider.size.y / 2)) {
					XA_Play(SOUND_LABEL_SE012);
					hitObject->flg = OBJECT_FLG_FALL;
					hitObject->collider.dontHitFrame = 0;
					hitObject->controller.moveSpeed = Float2zero;
					hitObject->collider.isActive = false;
					hitObject->controller.isActive = false;
				}
			}
		}
		break;
		default:
			if (hitObject->flg & OBJECT_FLG_KNOCKBACK) {
				Float2 hitCenter = hitObject->transform.position + hitObject->collider.offset;
				Float2 ownerCenter = pOwner->transform.position + pOwner->collider.offset;
				if (hitCenter.x >= ownerCenter.x - (pOwner->collider.size.x / 2) && hitCenter.x <= ownerCenter.x + (pOwner->collider.size.x / 2)) {
					if (hitCenter.y >= ownerCenter.y - (pOwner->collider.size.y / 2) && hitCenter.y <= ownerCenter.y + (pOwner->collider.size.y / 2)) {
						XA_Play(SOUND_LABEL_SE012);
						hitObject->flg = OBJECT_FLG_FALL;
						hitObject->collider.dontHitFrame = 0;
						hitObject->controller.moveSpeed = Float2zero;
						hitObject->collider.isActive = false;
						hitObject->controller.isActive = false;
					}
				}
			}
			else {
				hitObject->transform.position = hitObject->transform.position + result->d;
			}
			break;
		}
		break;
	case TAG_MAGMA:
		switch (hitObject->tag)
		{
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
		case TAG_ENEMY_DRAGON_2:
		case TAG_ENEMY_DRAGON_3:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_POISONOUS:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			if (hitObject->collider.dontHitFrame > 0) {
				break;
			}
			hitObject->controller.pMove->SetKnockBack(myPOwner);
			break;
		}
		break;
	case TAG_WATER:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			if (!pCPlay->canWalkWater) {
				hitObject->transform.position = hitObject->transform.position + result->d;
			}
			break;
		case TAG_ENEMY_ORC_1:
		case TAG_ENEMY_ORC_2:
			if (hitObject->flg & OBJECT_FLG_WALK) {
				hitObject->flg &= ~OBJECT_FLG_WALK;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_PLANT_1:
		case TAG_ENEMY_PLANT_FLOWER:
			if (hitObject->flg & OBJECT_FLG_RUN) {
				hitObject->flg &= ~OBJECT_FLG_RUN;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
		case TAG_ENEMY_DRAGON_2:
		case TAG_ENEMY_DRAGON_3:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_UP_1:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			//case TAG_PLAYER_CAN_WALK_WATER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ--;
			if (pCPlay->mapPointZ < 0) {
				pCPlay->mapPointZ = 0;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_UP_2:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			//case TAG_PLAYER_CAN_WALK_WATER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ -= 2;
			if (pCPlay->mapPointZ < 0) {
				pCPlay->mapPointZ = 0;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_UP_3:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			//case TAG_PLAYER_CAN_WALK_WATER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ -= 3;
			if (pCPlay->mapPointZ < 0) {
				pCPlay->mapPointZ = 0;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_DOWN_1:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			//case TAG_PLAYER_CAN_WALK_WATER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ++;
			if (pCPlay->mapPointZ >= MAP_NUM) {
				pCPlay->mapPointZ = MAP_NUM - 1;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_DOWN_2:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ += 2;
			if (pCPlay->mapPointZ >= MAP_NUM) {
				pCPlay->mapPointZ = MAP_NUM - 1;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_FLOOR_DOWN_3:
		switch (hitObject->tag)
		{
		case TAG_PLAYER:
			pCPlay->UpdataItem();
			pCPlay->mapPointZ += 3;
			if (pCPlay->mapPointZ >= MAP_NUM) {
				pCPlay->mapPointZ = MAP_NUM - 1;
			}
			pCPlay->fade = FADEOUT;
			pCPlay->scene = PLAY_CHANGMAP;
			XA_Play(SOUND_LABEL_SE006);
			pCPlay->bgm->XA_Stop_BGM();
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_1:
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_SPIRIT_5:
		case TAG_ENEMY_SPIRIT_6:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_EYE_1:
		case TAG_ENEMY_EYE_2:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
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
		if (!(myPOwner->flg & OBJECT_FLG_OPEN)) {
			switch (hitObject->tag)
			{
			case TAG_ENEMY_SPIRIT_2:
			case TAG_ENEMY_SPIRIT_3:
			case TAG_ENEMY_SPIRIT_6:
				CObjGene::SetEnemyParameter(hitObject, TAG_ENEMY_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
				break;
			case TAG_ENEMY_DESTROY:
			case TAG_PLAYERATTACK:
			case TAG_ENEMY_SPIRIT_4:
			case TAG_ENEMY_DEATH:
			case TAG_ENEMY_GHOST_1:
			case TAG_ENEMY_GHOST_2:
				break;
			case TAG_ENEMY_PLANT_1:
			case TAG_ENEMY_PLANT_2:
			case TAG_ENEMY_PLANT_FLOWER:
				if (hitObject->flg & OBJECT_FLG_RUN) {
					hitObject->flg &= ~OBJECT_FLG_RUN;
					hitObject->flg |= OBJECT_FLG_IDOL;
				}
				hitObject->transform.position = hitObject->transform.position + result->d;
				break;
			default:
				hitObject->transform.position = hitObject->transform.position + result->d;
				break;
			}
		}
		break;
	case TAG_ITEM_MOVE_BLOCK_1:
	case TAG_ITEM_MOVE_BLOCK_2:
		switch (hitObject->tag)
		{
		case TAG_ENEMY_SPIRIT_2:
		case TAG_ENEMY_SPIRIT_3:
		case TAG_ENEMY_SPIRIT_6:
			CObjGene::SetEnemyParameter(hitObject, TAG_ENEMY_NULL, hitObject->transform.position.x, hitObject->transform.position.y);
			break;
		case TAG_ENEMY_DESTROY:
		case TAG_PLAYERATTACK:
		case TAG_ENEMY_SPIRIT_4:
		case TAG_ENEMY_DEATH:
		case TAG_ENEMY_GHOST_1:
		case TAG_ENEMY_GHOST_2:
			break;
		case TAG_ENEMY_PLANT_1:
		case TAG_ENEMY_PLANT_2:
		case TAG_ENEMY_PLANT_FLOWER:
			if (hitObject->flg & OBJECT_FLG_RUN) {
				hitObject->flg &= ~OBJECT_FLG_RUN;
				hitObject->flg |= OBJECT_FLG_IDOL;
			}
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		default:
			hitObject->transform.position = hitObject->transform.position + result->d;
			break;
		}
		break;
	case TAG_BACKGROUND:
	case TAG_FLOOR:
	default:
		break;
	}
}

COLLIDER_RESULT Collider_Test(CCollider * pThis, CCollider * pOther)
{
	COLLIDER_RESULT res = { false };

	if (pThis->isActive == false || pOther->isActive == false) {
		return res;
	}

	// Colliderの親GameObjectの座標を足して中心点を計算
	float this_centerX = pThis->pOwner->transform.position.x + pThis->offset.x;
	float this_centerY = pThis->pOwner->transform.position.y + pThis->offset.y;
	float other_centerX = pOther->pOwner->transform.position.x + pOther->offset.x;
	float other_centerY = pOther->pOwner->transform.position.y + pOther->offset.y;

	// 中心点同士の距離を計算（絶対値）
	float distX = fabsf(other_centerX - this_centerX);
	float distY = fabsf(other_centerY - this_centerY);
	// ２つの四角形の縦横の長さを足して÷２した値を計算
	float halfTotalLenX = (pThis->size.x + pOther->size.x) / 2.0f;
	float halfTotalLenY = (pThis->size.y + pOther->size.y) / 2.0f;

	// 下の２つの条件が両方trueなら衝突している
	res.isHit = distX < halfTotalLenX && distY < halfTotalLenY;

	// dy1のみ改変。なぜか下から上でもバグが出ない。
	float dx1 = (other_centerX - pOther->size.x / 2) - (this_centerX + pThis->size.x / 2.0f);
	float dx2 = (other_centerX + pOther->size.x / 2) - (this_centerX - pThis->size.x / 2.0f);
	float dy1 = (other_centerY - pOther->size.y / 2.0f) - (this_centerY + pThis->size.y / 2);
	float dy2 = (other_centerY + pOther->size.y / 2) - (this_centerY - pThis->size.y / 2.0f);

	float dx = fabsf(dx1) < fabsf(dx2) ? dx1 : dx2;
	float dy = fabsf(dy1) < fabsf(dy2) ? dy1 : dy2;

	if (fabsf(dx) < fabsf(dy)) {
		res.d.x = dx;
		res.d.y = 0.0f;
	}
	else {
		res.d.x = 0.0f;
		res.d.y = dy;
	}

	return res;
}

bool Collider_Test_Bool(CCollider * pThis, CCollider * pOther)
{
	if (pThis->isActive == false || pOther->isActive == false) {
		return false;
	}

	// Colliderの親GameObjectの座標を足して中心点を計算
	float this_centerX = pThis->pOwner->transform.position.x + pThis->offset.x;
	float this_centerY = pThis->pOwner->transform.position.y + pThis->offset.y;
	float other_centerX = pOther->pOwner->transform.position.x + pOther->offset.x;
	float other_centerY = pOther->pOwner->transform.position.y + pOther->offset.y;

	// 中心点同士の距離を計算（絶対値）
	float distX = fabsf(other_centerX - this_centerX);
	float distY = fabsf(other_centerY - this_centerY);
	// ２つの四角形の縦横の長さを足して÷２した値を計算
	float halfTotalLenX = (pThis->size.x + pOther->size.x) / 2.0f;
	float halfTotalLenY = (pThis->size.y + pOther->size.y) / 2.0f;

	// 下の２つの条件が両方trueなら衝突している
	return distX < halfTotalLenX && distY < halfTotalLenY;
}

