#include <dxlib.h>
#include "Player.h"
#include "../common/Frame.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
#include "../Input/AI/Enemy1AI.h"
#include "../tmx/tmxObj.h"
#include "../Scene/FireMap.h"
#include "../Scene/GameScene.h"
#include "../_debug/_DebugDispOut.h"

Player::Player(ActorType type, Vector2D pos):
	Actor(type,pos)
{	
	Init();
	//moveFunc_ = &Player::MoveLeft;	//アドレスの計算式を全部入れる必要がある まだアドレスが確定していない
}

Player::~Player()
{

}

void Player::Init(void)
{
	if (type_ == ActorType::Player)
	{
		cont_ = std::make_unique<Gamepad>();		//キーボードクラスとしてインスタンス
	}
	else
	{
		cont_ = std::make_unique<Enemy1AI>();
	}
	moveFuncList_.push_back(&Player::SetBomb);
	moveFuncList_.push_back(&Player::MoveUp);
	moveFuncList_.push_back(&Player::MoveDown);
	moveFuncList_.push_back(&Player::MoveLeft);
	moveFuncList_.push_back(&Player::MoveRight);
	moveFuncList_.push_back(&Player::Idle);
}

void Player::Update(BaseScene& scene)
{
	cont_->Update(scene,this);
	animeCtl_.dir_[OldFlame] = animeCtl_.dir_[NowFlame];

	if (CheckErase())
	{
		return;
	}


	auto actorList = dynamic_cast<GameScene&>(scene).GetActorList();
	auto fireMap = dynamic_cast<GameScene&>(scene).GetFireMap();
	auto tmxObj = dynamic_cast<GameScene&>(scene).GetTmxObj();
	auto enemyAI = dynamic_cast<GameScene&>(scene).GetEnemy1AI();

	//炎に当たったか
	if (!fireMap.expired())
	{
		if (fireMap.lock()->CheckFire(animeCtl_.GetFootPos()))
		{
			//当たってた
			//Stateを死亡状態にする
			animeCtl_.SetState(State::Dead);
			//moveFuncList_をクリアーし、代わりにDead()をせっとする
			moveFuncList_.clear();
			moveFuncList_.insert(moveFuncList_.begin(), &Player::Dead);
		}
	}
	//if (!enemyAI.expired())
	//{
	//	if (enemyAI.lock()->CheckEnemy(animeCtl_.GetFootPos()))
	//	{
	//		//当たってた
	//		//Stateを死亡状態にする
	//		animeCtl_.SetState(State::Dead);
	//		//moveFuncList_をクリアーし、代わりにDead()をせっとする
	//		moveFuncList_.clear();
	//		moveFuncList_.insert(moveFuncList_.begin(), &Player::Dead);
	//	}
	//}
	

	bool isMove = false;	//true:動いている false:動いていない
	////animeCtl_.SetState(State::Idle);
						//(this->*moveFunc_)();
	//isMove	= (this->*moveFunc_)();
	//isMove |= MoveRight();
	//isMove |= MoveUp();
	//isMove |= MoveDown();
	//if (isMove == false)
	//{
	//	animeCtl_.SetState(State::Idle);
	//}

	for (auto itr = moveFuncList_.begin(); itr != moveFuncList_.end(); ++itr)
	{
		if ((this->*(*itr))(actorList,tmxObj))
		{
			moveFuncList_.insert(--moveFuncList_.end(),*itr);
			moveFuncList_.erase(itr);		//このイテレータが示す場所は使えないので、＋＋できない
			break;
		}
	}
	if (type_==ActorType::Player)
	{
		CheckHit(actorList);
	}
	_dbgDrawBox(0, 0, 100, 16, 0, true);
	_dbgDrawFormatString(0,0,0xffffff,"X:%d y%d",pos_.x,pos_.y);
	int pos = 0;
	for (auto data : moveFuncList_)
	{
		
		if (data == &Player::MoveUp)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveUp");
		}
		if (data == &Player::MoveDown)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveDown");
		}
		if (data == &Player::MoveLeft)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveLeft");
		}
		if (data == &Player::MoveRight)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveRight");
		}
		if (data == &Player::Idle)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveRight");
		}
		if (data == &Player::Dead)
		{
			_dbgDrawFormatString(0, 24 + pos * 16, 0xffffff, "MoveRight");
		}
		pos++;
	}
}

bool Player::CheckMove(Vector2D& pos, Vector2D&& offset, std::weak_ptr<tmxObj> tmxObj)
{
	auto ckPos = pos + offset;
	if (tmxObj.expired())
	{
		return false;
	}
	//if(pos_+移動量の位置に壁がない場合)
	if (tmxObj.lock()->GetMapData("wall", ckPos) == -1 && tmxObj.lock()->GetMapData("block", ckPos) == -1)
	{
		return true;
	}
	return false;
}

bool Player::MoveLeft(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	auto cont = cont_->GetInputData();
	
	if (cont[NowFlame][static_cast<int>(InputID::Left)])		//現在のフレームのレフトにアクセス
	{
		//weak_ptrがあるか（終わってないか）を確認
		if (tmxObj.expired())		//trueの場合、終わっている
		{
			return false;
		}
		if (pos_.y % tmxObj.lock()->GetTileSize().y == 0)
		{
			//壊せるブロックがあるレイヤーと壊せないブロックがあるレイヤー
			if (CheckMove(pos_, Vector2D{ -speed_,0 }, tmxObj))
			{
				animeCtl_.dir_[NowFlame] = Dir::Left;
				animeCtl_.SetState(State::Run);
				//隣にボムがなければ移動する
				if (CheckSideActor(actList, {20,0}) != ActorType::Bomb)
				{
					pos_.x -= speed_;
				}
				return true;
			}
		}
	}
	return false;
}

bool Player::MoveRight(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	auto cont = cont_->GetInputData();
	if (cont[NowFlame][static_cast<int>(InputID::Right)])
	{
		if (tmxObj.expired())		//trueの場合、終わっている
		{
			return false;
		}
		auto tileSize = tmxObj.lock()->GetTileSize();
		if (pos_.y % tileSize.y == 0)
		{
			if (CheckMove(pos_, Vector2D{ tileSize.x + speed_ - 1,0 }, tmxObj))
			{
				animeCtl_.dir_[NowFlame] = Dir::Right;
				animeCtl_.SetState(State::Run);
				if (CheckSideActor(actList, { -20,0 }) != ActorType::Bomb)
				{
					pos_.x += speed_;
				}
				
				return true;
			}
		}
	}
	return false;
}

bool Player::MoveUp(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	auto cont = cont_->GetInputData();
	if (cont[NowFlame][static_cast<int>(InputID::Up)])
	{
		if (tmxObj.expired())		//trueの場合、終わっている
		{
			return false;
		}
		if (pos_.x % tmxObj.lock()->GetTileSize().x == 0)
		{
			if (CheckMove(pos_, Vector2D{ 0,-speed_ }, tmxObj))
			{
				animeCtl_.dir_[NowFlame] = Dir::Up;
				animeCtl_.SetState(State::Run);
				if (CheckSideActor(actList, { 0,20 }) != ActorType::Bomb)
				{
					pos_.y -= speed_;
				}
				
				return true;
			}
		}
	}
	return false;
}

bool Player::MoveDown(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	auto cont = cont_->GetInputData();
	if (cont[NowFlame][static_cast<int>(InputID::Down)])
	{
		if (tmxObj.expired())		//trueの場合、終わっている
		{
			return false;
		}
		auto tileSize = tmxObj.lock()->GetTileSize();
		if (pos_.x % tileSize.x == 0)
		{
			if (CheckMove(pos_, Vector2D{ 0,tileSize.y + speed_ - 1 }, tmxObj))
			{
				animeCtl_.dir_[NowFlame] = Dir::Down;
				animeCtl_.SetState(State::Run);
				if (CheckSideActor(actList, { 0,-20 }) != ActorType::Bomb)
				{
					pos_.y += speed_;
				}
				
				return true;
			}
		}
	}
	return false;
}

bool Player::Idle(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	if (tmxObj.expired())		//trueの場合、終わっている
	{
		return false;
	}

	animeCtl_.SetState(State::Idle);
	return true;
}

bool Player::SetBomb(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	bombPair_.first = false;
	auto cont = cont_->GetInputData();
	if (cont[NowFlame][static_cast<int>(InputID::Btn1)] && !cont[OldFlame][static_cast<int>(InputID::Btn1)])
	{
		bombPair_.first = true;
	
		bombPair_.second = pos_ / tmxObj.lock()->GetTileSize();
	}

	return false;
}

bool Player::Dead(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj)
{
	if (!animeCtl_.IsAnimeEnd())
	{
		return true;
	}
	return true;
}

bool Player::CheckHit(const ActorList& actList)
{
	for (auto actor : actList)
	{
		if ((actor->GetActorType() == ActorType::Enemy1) || (actor->GetActorType() == ActorType::Enemy2))
		{
			if (actor->GetPos() == pos_)
			{
				animeCtl_.SetState(State::Dead);
				return true;
			}
		}
	}
	return false;
}

ActorType Player::CheckSideActor(const ActorList& actList, Vector2D offset)
{
	for (auto actor : actList)
	{
		if (this == actor.get())
		{
			continue;
		}
		if (pos_ == offset + actor->GetPos())
		{
			return actor->GetActorType();
		}
	}
	
	
	return ActorType::Non;
}
