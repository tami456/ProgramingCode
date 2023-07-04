#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "FireMap.h"
#include "../tmx/tmxObj.h"
#include "../Actor/ActorType.h"
#include "../Actor/Actor.h"
#include "Camera.h"
#include "../Input/AI/Enemy1AI.h"
class Actor;	//型を使うだけ→クラス宣言、中身を使う→インクルード


class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	bool Init(void);
	uniqueScene Update(uniqueScene ownScene) override;
	void DrawOwnScreen(void) override;
	std::weak_ptr<tmxObj> GetTmxObj(void) { return tmxObj_; }
	std::weak_ptr<FireMap> GetFireMap(void) { return fireMap_; }
	std::weak_ptr<Enemy1AI> GetEnemy1AI(void) { return enemy1AI_; }
	const ActorList GetActorList(void) { return actorList_; }

private:
	bool next_;
	bool next2_;
	bool CreateActor(ActorType type,Vector2D pos);

	//テンプレート宣言
	template<class MyActor>
	bool CreateActor(ActorType type, Vector2D pos, bool poolOnly);		//本体

	Vector2D viewArea_;

	std::shared_ptr<tmxObj> tmxObj_;		//ユニークなのでクラス間の受け渡しができない
	std::shared_ptr<FireMap> fireMap_;
	std::shared_ptr<Enemy1AI> enemy1AI_;
	ActorList actorList_;
	std::map<ActorType, std::unique_ptr<Actor>> actorPool_;

	Camera camera_;
};

template<class MyActor>
inline bool GameScene::CreateActor(ActorType type, Vector2D pos, bool poolOnly)		//指定したアクタの型でインスタンスする。
{
	if (!actorPool_.count(type))
	{
		//プールの中に該当Actorがいない場合
		
		//MyActorのところにクラスが入る
		actorPool_.try_emplace(type,std::make_unique<MyActor>(type, pos * tmxObj_->GetTileSize()));
		actorPool_[type]->LoadAnime();
		actorPool_[type]->animeCtl_.reConnect(&actorPool_[type]->pos_);;
	}
	//Actor cloneObj = *actorPool_[type].get();
	//cloneObj.animeCtl_.reConnect(&cloneObj.pos_);
	//std::unique_ptr<Actor> actor;
	//actor.reset(&cloneObj);
	if (poolOnly)
	{
		return true;
	}
	actorList_.emplace_back(std::make_unique<MyActor>(type, pos * tmxObj_->GetTileSize()));
	auto actor = (--actorList_.end());
	(*actor)->alive_ = true;
	(*actor)->animeCtl_ = actorPool_[type]->animeCtl_;
	(*actor)->animeCtl_.reConnect(&(*actor)->pos_);

	return true;
}
