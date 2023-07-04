#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	mImgPush = -1;
	mImgBG = -1;
	mImgGameOver = -1;
	mSkyDome = nullptr;
	mAnimationController = nullptr;
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	ResourceManager& rm = ResourceManager::GetInstance();

	// 画像読み込み
	mImgPush = rm.Load(ResourceManager::SRC::PUSH_SPACE).mHandleId;
	mImgBG = rm.Load(ResourceManager::SRC::GAMEOVERBG).mHandleId;
	mImgGameOver = rm.Load(ResourceManager::SRC::GAMEOVER).mHandleId;

	float size;

	// キャラ
	mChara.SetModel(rm.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	mChara.pos = { 0.0f, -150.0f, 0.0f };
	size = 3.0f;
	mChara.scl = { size, size, size };
	mChara.quaRot = Quaternion::Euler(
		0.0f, 0.0f, 0.0f);
	mChara.Update();

	// アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	mAnimationController = new AnimationController(mChara.modelId);
	mAnimationController->Add(0, path + "Dead.mv1", 15.0f);
	mAnimationController->Play(0,false);

	// 定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
}

void GameOverScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// キャラアニメーション
	mAnimationController->Update();
}

void GameOverScene::Draw(void)
{
	DrawRotaGraph(512,320, 1.0, 0.0, mImgBG, false);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 1.0, 0.0, mImgGameOver, true);
	MV1DrawModel(mChara.modelId);
	
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, mImgPush, true);
	
}

void GameOverScene::Release(void)
{
	mAnimationController->Release();
	delete mAnimationController;
}
