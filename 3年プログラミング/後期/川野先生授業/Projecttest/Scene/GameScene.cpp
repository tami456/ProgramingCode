#include<DxLib.h>
#include "GameScene.h"
#include"../Input.h"
#include"GameOverScene.h"
#include"SceneManager.h"
#include"PauseScene.h"
#include <sstream>
#include <iomanip>
#include <cassert>

GameScene::GameScene(SceneManager& manager):Scene(manager),
updateFunc_(&GameScene::InitializeUpdate),
drawFunc_(&GameScene::InitializeDraw)
{

	
}

void GameScene::Update(Input& input)
{
	(this->*updateFunc_)(input);
	fps_ = GetFPS();
}

void GameScene::Draw()
{
	(this->*drawFunc_)();
}

void GameScene::InitializeUpdate(Input& input)
{
	SetUseASyncLoadFlag(true);
	std::vector<std::wstring> filename =
	{
		L"adventurer-air-attack-3-end-00.png",
		L"adventurer-air-attack-3-end-01.png",
		L"adventurer-air-attack-3-end-02.png",
		L"adventurer-air-attack1-00.png",
		L"adventurer-air-attack1-01.png",
		L"adventurer-air-attack1-02.png",
		L"adventurer-air-attack1-03.png",
		L"adventurer-air-attack2-00.png",
		L"adventurer-air-attack2-01.png",
		L"adventurer-air-attack2-02.png",
		L"adventurer-air-attack3-loop-00.png",
		L"adventurer-air-attack3-loop-01.png",
		L"adventurer-air-attack3-rdy-00.png",
		L"adventurer-attack1-00.png",
		L"adventurer-attack1-01.png",
		L"adventurer-attack1-02.png",
		L"adventurer-attack1-03.png",
		L"adventurer-attack1-04.png",
		L"adventurer-attack2-00.png",
		L"adventurer-attack2-01.png",
		L"adventurer-attack2-02.png",
		L"adventurer-attack2-03.png",
		L"adventurer-attack2-04.png",
		L"adventurer-attack2-05.png",
		L"adventurer-attack3-00.png",
		L"adventurer-attack3-01.png",
		L"adventurer-attack3-02.png",
		L"adventurer-attack3-03.png",
		L"adventurer-attack3-04.png",
		L"adventurer-attack3-05.png",
		L"adventurer-cast-00.png",
		L"adventurer-cast-01.png",
		L"adventurer-cast-02.png",
		L"adventurer-cast-03.png",
		L"adventurer-cast-loop-00.png",
		L"adventurer-cast-loop-01.png",
		L"adventurer-cast-loop-02.png",
		L"adventurer-cast-loop-03.png",
		L"adventurer-crnr-clmb-00.png",
		L"adventurer-crnr-clmb-01.png",
		L"adventurer-crnr-clmb-02.png",
		L"adventurer-crnr-clmb-03.png",
		L"adventurer-crnr-clmb-04.png",
		L"adventurer-crnr-grb-00.png",
		L"adventurer-crnr-grb-01.png",
		L"adventurer-crnr-grb-02.png",
		L"adventurer-crnr-grb-03.png",
		L"adventurer-crnr-jmp-00.png",
		L"adventurer-crnr-jmp-01.png",
		L"adventurer-crouch-00.png",
		L"adventurer-crouch-01.png",
		L"adventurer-crouch-02.png",
		L"adventurer-crouch-03.png",
		L"adventurer-die-00.png",
		L"adventurer-die-01.png",
		L"adventurer-die-02.png",
		L"adventurer-die-03.png",
		L"adventurer-die-04.png",
		L"adventurer-die-05.png",
		L"adventurer-die-06.png",
		L"adventurer-fall-00.png",
		L"adventurer-fall-01.png",
		L"adventurer-hurt-00.png",
		L"adventurer-hurt-01.png",
		L"adventurer-hurt-02.png",
		L"adventurer-idle-00.png",
		L"adventurer-idle-01.png",
		L"adventurer-idle-02.png",
		L"adventurer-idle-03.png",
		L"adventurer-idle-2-00.png",
		L"adventurer-idle-2-01.png",
		L"adventurer-idle-2-02.png",
		L"adventurer-idle-2-03.png",
		L"adventurer-items-00.png",
		L"adventurer-items-01.png",
		L"adventurer-items-02.png",
		L"adventurer-jump-00.png",
		L"adventurer-jump-01.png",
		L"adventurer-jump-02.png",
		L"adventurer-jump-03.png",
		L"adventurer-ladder-climb-00.png",
		L"adventurer-ladder-climb-01.png",
		L"adventurer-ladder-climb-02.png",
		L"adventurer-ladder-climb-03.png",
		L"adventurer-run-00.png",
		L"adventurer-run-01.png",
		L"adventurer-run-02.png",
		L"adventurer-run-03.png",
		L"adventurer-run-04.png",
		L"adventurer-run-05.png",
		L"adventurer-slide-00.png",
		L"adventurer-slide-01.png",
		L"adventurer-smrslt-00.png",
		L"adventurer-smrslt-01.png",
		L"adventurer-smrslt-02.png",
		L"adventurer-smrslt-03.png",
		L"adventurer-stand-00.png",
		L"adventurer-stand-01.png",
		L"adventurer-stand-02.png",
		L"adventurer-swrd-drw-00.png",
		L"adventurer-swrd-drw-01.png",
		L"adventurer-swrd-drw-02.png",
		L"adventurer-swrd-drw-03.png",
		L"adventurer-swrd-shte-00.png",
		L"adventurer-swrd-shte-01.png",
		L"adventurer-swrd-shte-02.png",
		L"adventurer-swrd-shte-03.png",
		L"adventurer-wall-slide-00.png",
		L"adventurer-wall-slide-01.png"
	};

	for (const auto& fname : filename)
	{

		std::wstringstream wss;
		wss << L"./image/sprite/";
		wss << fname;
		auto imgH = LoadGraph(wss.str().c_str());
		assert(imgH >= 0);
		imgHs_.push_back(imgH);
	}
	updateFunc_ = &GameScene::LoadingUpdate;
	drawFunc_ = &GameScene::LoadingDraw;

	
}

void GameScene::InitializeDraw()
{
}

void GameScene::LoadingUpdate(Input& input)
{
	if (DxLib::GetASyncLoadNum() == 0)
	{
		updateFunc_ = &GameScene::LoadingCompletedUpdate;
		drawFunc_ = &GameScene::NormalDraw;
	}
}

void GameScene::LoadingDraw()
{
	std::wstring nowloadStr = L"Now Loading...";
	DrawString(100, 100, nowloadStr.substr(0, nowLoadingCnt).c_str(), 0xffffff);
	nowLoadingCnt = (nowLoadingCnt + 1) % nowloadStr.length();
}

void GameScene::LoadingCompletedUpdate(Input& input)
{
	SetUseASyncLoadFlag(false);
	updateFunc_ = &GameScene::NormalUpdate;
}

void GameScene::NormalUpdate(Input& input)
{
	
	if (input.IsTriggered("next"))
	{
		sceneManager_.ChangeScene(std::make_shared<GameOverScene>(sceneManager_));
	}
	else if (input.IsTriggered("pause"))
	{
		sceneManager_.PushScene(std::make_shared<PauseScene>(sceneManager_));
	}
	else if (input.IsTriggered("jump"))
	{
		DrawString(250, 250, L"Jump", 0xffffff);
	}
	else if (input.IsTriggered("attack"))
	{
		DrawString(250, 250, L"Attack", 0xffffff);
	}
}

void GameScene::NormalDraw()
{
	DrawFormatString(50, 30, 0xffffff, L"FPS=%2.2f", fps_);
	DrawString(50, 50, L"GameScene", 0xffffff);

	int idx = (frame_ / 5) % imgHs_.size();
	DrawRotaGraph(320, 240, 5.0f, 0.0f, imgHs_[idx], true);
	++frame_;
}
