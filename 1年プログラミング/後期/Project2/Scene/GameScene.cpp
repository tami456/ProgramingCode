#include <dxlib.h>
#include "GameScene.h"
#include "ResultScene.h"
#include "../Actor/Actor.h"
#include "../Actor/Player.h"
#include "../Actor/Enemy.h"
#include "../Actor/Bomb.h"
#include "../Input/Keyboard.h"
#include "../Input/Gamepad.h"
#include "../common/Frame.h"
#include "MapID.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

bool GameScene::Init(void)
{
	next_ = false;
	SetDrawScreen(screenID_);
	tmxObj_ = std::make_shared<tmxObj>();		//実体を作り、新規オブジェクトを作る
	//tmxObj_->LoadTSX("data/tmx/map.tsx");
	tmxObj_->LoadTMX("data/tmx/map.tmx");

	fireMap_ = std::make_shared<FireMap>(tmxObj_->GetWorldArea(), tmxObj_->GetTileSize());

	auto create = [&](MapID mapID,ActorType actorType) 
	{
		auto PosList = tmxObj_->SearchID(mapID, "block");
		for (auto pos:PosList)
		{
			if (CreateActor(actorType, pos))
			{
				tmxObj_->SetMapData(-1, "block", pos.x, pos.y);
			}
		}
	};

	viewArea_ = { 640,480 };

	create(MapID::Player, ActorType::Player);
	create(MapID::Enemy1, ActorType::Enemy1);
	create(MapID::Enemy2, ActorType::Enemy2);
	//CreateActor<Bomb>(ActorType::Bomb, { 10,10 }, true);
	//探索し、Playerが見つかったらカメラのReConnectを呼び、Playerをカメラのターゲットにする
	for (auto actor : actorList_)
	{
		if (actor->type_ == ActorType::Player)
		{
			camera_.ReConnect(actor);
			camera_.Init(tmxObj_->GetWorldArea() * tmxObj_->GetTileSize());
			break;
		}
	}
	int gameSizeY = tmxObj_->GetWorldArea().y * tmxObj_->GetTileSize().y;
	drawOffset_.y = (viewArea_.y - gameSizeY) / 2;
	//任意のidをmapから消す
	//for (int y = 0; y < tmxObj_->GetWorldArea().y; y++)
	//{
	//	for (int x = 0; x < tmxObj_->GetWorldArea().x; x++)
	//	{
	//		tmxObj_->SetMapData(-1, "block",x,y);			
	//	}
	//}
	TRACE("PLAYERがDeadでシーン遷移\n");
	return true;
}

uniqueScene GameScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	for (auto& actor : actorList_)		//実体を参照で作る
	{
		actor->Update(*this);
		if (actor->IsBomb().first)
		{
			CreateActor<Bomb>(ActorType::Bomb, actor->IsBomb().second,false);
		}
	}

	camera_.Update();
	//メンバー関数
	//remove_if
	//コンテナの内容を消す


	/*auto itr = std::remove_if(actorList_.begin(),actorList_.end(),
		[](const sharedActor& actor) {
			return !actor->IsAlive();
		});
	actorList_.erase(itr, actorList_.end());*/

	actorList_.remove_if(
		[&](const sharedActor& actor) {
			if (!actor->IsAlive() && 
				actor->GetActorType() == ActorType::Player)
			{
				next_ = true;
			}
			return !actor->IsAlive();
		});
	
	fireMap_->Update(tmxObj_);
	
	_dbgDrawBox(220, 0, 400, 16, 0, true);
	_dbgDrawFormatString(220, 0, 0xffffff, "ActorListの登録数:%d", actorList_.size());
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwnScreen();
	
	//auto contData = controller_->GetInputData();
	//if (contData[NowFlame][static_cast<int>(InputID::Btn3)] && !contData[OldFlame][static_cast<int>(InputID::Btn3)])
	//{
	//	//ownScene.reset(new GameScene());
	//	//return std::move(ownScene);

	//	//ゲームシーンが入っているユニークポインタが返る。
	//	//TitleSceneが破棄されたので、~TitleSceneが呼ばれる。
	//	return std::make_unique<ResultScene>();
	//}
	if (next_)
	{
		return std::make_unique<ResultScene>();
	}
	_dbgDrawBox(110, 0, 210, 18, 0, true);
	_dbgDrawFormatString(110,0,0xffffff,"X:%d,Y:%d",camera_.GetPos().x, camera_.GetPos().y);

	return std::move(ownScene);

}

void GameScene::DrawOwnScreen(void)
{
	//ずらす範囲-カメラの座標
	auto offset = (viewArea_ / 2) - camera_.GetPos();
	

	SetDrawScreen(screenID_);
	const Vector2D& worldArea = tmxObj_->GetWorldArea();

	//マップの描画
	for (auto layerID : tmxObj_->GetMapData())
	{
		for (int y = 0; y < worldArea.y; y++)
		{
			for (int x = 0; x < worldArea.x; x++)
			{
				tmxObj_->DrawMapData(layerID.first, x, y, offset);
			}
		}
	}
	//Actor関連の描画
	
	//Zソート
	actorList_.sort([](sharedActor actor1,sharedActor actor2) {
		return actor1->GetFootPos().y < actor2->GetFootPos().y;
		});

	
	//tmxObj_->Draw();
	for (auto& actor : actorList_)		//実体を参照で作る
	{
		actor->Draw(offset);
	}

	//炎の描画
	for (int y = 0; y < worldArea.y; y++)
	{
		for (int x = 0; x < worldArea.x; x++)
		{
			fireMap_->Draw(tmxObj_,x, y, offset,drawOffset_);
		}
	}
	
}

bool GameScene::CreateActor(ActorType type, Vector2D pos)
{
	switch (type)
	{
	case ActorType::Player:
		CreateActor<Player>(type,pos,false);
		break;
	case ActorType::Enemy1:
	case ActorType::Enemy2:
		CreateActor<Enemy>(type, pos,false);	
		break;
	case ActorType::Bomb:
		CreateActor<Bomb>(type, pos, false);
		break;
	default:
		TRACE("type異常");
		return false;
		//break;
	}
		//auto actor = (--actorList_.end());
		//(*actor)->LoadAnime();
		//(*actor)->animeCtl_.reConnect(&(*actor)->pos_);
	
	return true;
}
