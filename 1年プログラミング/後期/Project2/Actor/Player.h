#pragma once
#include <list>
#include<memory>
#include "Actor.h"
#include "../Input/Controller.h"
#include "../common/Vector2D.h"
#include "../common/ImageCtl.h"



class Player:
	public Actor
{
public:
	Player(ActorType type, Vector2D pos);
	~Player();

private:
	void Init(void);
	void Update(BaseScene& scene)override;
	std::list<bool (Player::*)(const ActorList& actList,std::weak_ptr<tmxObj>)> moveFuncList_;
	bool CheckMove(Vector2D& pos,Vector2D&& offset,std::weak_ptr<tmxObj> tmxObj);	//右と下は幅や高さがあるのでめりこむ
	//右辺値参照→その場で作って消えるもの→＆＆
	//左辺値参照→変数を宣言して、その中に代入する→＆

	//bool (Player::*moveFunc_)(void);
	bool MoveLeft(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveRight(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveUp(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool MoveDown(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool Idle(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool SetBomb(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool Dead(const ActorList& actList,std::weak_ptr<tmxObj> tmxObj);
	bool CheckHit(const ActorList& actList);
	ActorType CheckSideActor(const ActorList& actList,Vector2D offset);
	std::unique_ptr<Controller> cont_;		//Updataを呼べば、キー情報を取得できる
};

