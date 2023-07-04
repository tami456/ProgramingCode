#include <vector>
#include <dxlib.h>
#include "../Input/InputID.h"
#include "GameScene.h"
#include "../Actor/Actor.h"
#include "../Actor/Player.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

GameScene::GameScene()
{
	gage = 610;
	gage2 = 20;
	Init();
}

GameScene::~GameScene()
{

}

bool GameScene::Init(void)
{
	stamina = LoadGraph("./data/image/stamina.png");
	get = LoadGraph("./data/image/getitem.png");
	sH = LoadSoundMem("data/bgm/Swish_whizz.mp3");
	volume = 120;
	bg_.Init();
	ChangeVolumeSoundMem(volume, sH);
	PlaySoundMem(sH, DX_PLAYTYPE_LOOP);
	
	CreateActor<Item>(ActorType::Item, Vector2D{ 0,-50 }, false);
	CreateActor<Item1>(ActorType::Item1, Vector2D{ 0,-700 }, false);
	CreateActor<Player>(ActorType::Player, Vector2D{ 300,360 }, false);
	return true;
}

uniqueScene GameScene::Update(uniqueScene ownScene)
{
	controller_->Update(*ownScene.get());
	
	for (auto& actor : actorList_)		//é¿ëÃÇéQè∆Ç≈çÏÇÈ
	{
		/*actor->GetActorType() == ActorType::Player;
		actor->GetActorType() == ActorType::Item;*/
		actor->Update(*this);
	}

	actorList_.remove_if(
		[](const sharedActor actor) {
			return !actor->IsAlive();		//trueÇæÇ∆è¡ÇµÇƒÇµÇ‹Ç§ÇÃÇ≈ÅAîΩì]Ç∑ÇÈ
		});
	


	

	if (gage2 <= 460)
	{
		gage2 += 0.3f;
	}

	
	
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawOwnScreen();
	
	

	if (gage2 >= 460)
	{
		StopSoundMem(sH);
		return std::make_unique<ResultScene>();
	}

	return std::move(ownScene);

}

void GameScene::DrawOwnScreen(void)
{
	
	bg_.Draw();
	DrawGraph(0, 430, get, true);
	DrawGraph(600, 0, stamina, true);
	if (DrawBox(gage, gage2, 630, 460, 0xffff00, true) == -1)
	{
		TRACE("éläpÇ»Ç¢ÇÊ");
	}
	for (auto& actor : actorList_)
	{
		actor->Draw();
	}
	point++;
	DrawFormatString(18, 8, 0x000000, "%dM", point);
}

bool GameScene::CreateActor(ActorType type, Vector2D pos)
{
	switch (type)
	{
	case ActorType::Player:
		CreateActor<Player>(type, pos, true);
		break;
	case ActorType::Item:
		CreateActor<Item>(type, pos, true);
		break;
	case ActorType::Item1:
		CreateActor<Item1>(type, pos, true);
		break;
	default:
		break;
	}
	return false;
}