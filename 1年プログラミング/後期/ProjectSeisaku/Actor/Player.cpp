#include <dxlib.h>
#include "Player.h"
#include "../common/Frame.h"
#include "../Scene/GameScene.h"
#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"


#define CHIP_SIZE_X 32
#define CHIP_SIZE_Y 32

Player::Player(ActorType type, Vector2D pos):Actor(type,pos)
{
	Init();
}


Player::~Player()
{

}

void Player::Init()
{	
}

void Player::Update(BaseScene& scene)
{
	

	if (CheckHitKey(KEY_INPUT_RIGHT))
		if (pos_.x < 600 - CHIP_SIZE_X)
		{
			pos_.x += speed_;
		}
	if (CheckHitKey(KEY_INPUT_LEFT))
		if (pos_.x > 0)
		{
			pos_.x -= speed_;
		}
	if (CheckHitKey(KEY_INPUT_UP))
		if (pos_.y > 0)
		{
			pos_.y -= speed_;
		}
	if (CheckHitKey(KEY_INPUT_DOWN))
		if (pos_.y < 430 - CHIP_SIZE_Y)
		{
			pos_.y += speed_;
		}

}

