#include <DxLib.h>
#include "Player.h"
#include "Common.h"
#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"

#define SCREEN_SIZE_X 800	//画面の横サイズ
#define SCREEN_SIZE_Y 600	//画面の縦サイズ

int Player::playerCount = 0;

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Init(CharID charID,bool loadFlag,Vector2 chipSize)
{
	charID_ = charID;
	state_ = State::NON;
	chipSize_ = chipSize;
	if (playerCount == 0)
	{
		pos_ = { CHIP_SIZE * 5,CHIP_SIZE * 5 };
		dir_ = Dir::RIGHT;
		keyList_[static_cast<int>(Dir::LEFT)] = KEY_INPUT_A;
		keyList_[static_cast<int>(Dir::RIGHT)] = KEY_INPUT_D;
		keyList_[static_cast<int>(Dir::UP)] = KEY_INPUT_W;
		keyList_[static_cast<int>(Dir::DOWN)] = KEY_INPUT_S;
	}
	else
	{
		pos_ = { (CHIP_SIZE * (MAP_CHIP_COUNT_X - 5)) ,CHIP_SIZE * 5 };
		dir_ = Dir::LEFT;
		keyList_[static_cast<int>(Dir::LEFT)] = KEY_INPUT_LEFT;
		keyList_[static_cast<int>(Dir::RIGHT)] = KEY_INPUT_RIGHT;
		keyList_[static_cast<int>(Dir::UP)] = KEY_INPUT_UP;
		keyList_[static_cast<int>(Dir::DOWN)] = KEY_INPUT_DOWN;
	}
	if (loadFlag)
	{
		charImage_.Init(charID);
		state_ = State::ALIVE;
	}
	playerCount++;
	animCount = 0;
	return false;
}

bool Player::Release(void)
{
	charImage_.Release();
	playerCount--;
	return true;
}

Player::State Player::Update(MapCtl* mapCtl)
{
	animCount++;
	if (state_ == State::DEAD)
	{
		return state_;
	}
	SetDir();
	if (pos_.x_ % mapCtl->GetBlockSize() == 0 &&
		pos_.y_ % mapCtl->GetBlockSize() == 0
		)	//アロウと言う
	{
		if (mapCtl->isOnBlock(pos_))
		{
			state_ = State::DEAD;
			return state_;
		}
		mapCtl->SetBlock(pos_);
	}
	switch (dir_)
	{
	case Dir::RIGHT:
		if (pos_.x_ < SCREEN_SIZE_X - 32 - chipSize_.x_)
		{
			pos_.x_ += 2;
		}
		break;
	case Dir::LEFT:
		if (pos_.x_ > 32)
		{
			pos_.x_ -= 2;
		}
		break;
	case Dir::UP:
		if (pos_.y_ > 32)
		{
			pos_.y_ -= 2;
		}
		break;
	case Dir::DOWN:
		if (pos_.y_ < SCREEN_SIZE_Y - 152)
		{
			pos_.y_ += 2;
		}
		else
		{
			TRACE("動けない");
		}
		break;
	default:
		dir_ = Dir::LEFT;
		break;

	}
	return state_;

}

void Player::SetDir(void)
{
	if ((pos_.x_ % chipSize_.x_) || (pos_.y_ % chipSize_.y_)) //マス目ピッタリじゃなかったら
	{
		return;
	}
	
	if (keyNow_[static_cast<int>(Dir::RIGHT)])
	{
		dir_ = Dir::RIGHT;
	}
	if (keyNow_[static_cast<int>(Dir::LEFT)])
	{
		dir_ = Dir::LEFT;
	}
	if (keyNow_[static_cast<int>(Dir::UP)])
	{
		dir_ = Dir::UP;
	}
	if (keyNow_[static_cast<int>(Dir::DOWN)])
	{
		dir_ = Dir::DOWN;
	}
}

void Player::Draw(CharImage*charImage)
{
	DrawGraph(
		pos_.x_,
		pos_.y_,
		charImage[static_cast<int>(charID_)].chipImage_[0][0],
		true
	);
}

void Player::Draw(void)
{
	DrawGraph(
		pos_.x_,
		pos_.y_ - 12,
		charImage_.chipImage_[static_cast<int>(dir_)][0],
		true
	);
}

void Player::GetKeyState(char* keyData)
{
	memcpy(keyOld_,keyNow_,sizeof(keyOld_));
	for (int dir = 0; dir < static_cast<int>(Dir::MAX); dir++)
	{
		keyNow_[dir] = keyData[keyList_[dir]];
	}
}

bool Player::SetState(State state)
{
	state_ = state;
	return true;
}

Player::State Player::GetState(void)
{
	return state_;
}
