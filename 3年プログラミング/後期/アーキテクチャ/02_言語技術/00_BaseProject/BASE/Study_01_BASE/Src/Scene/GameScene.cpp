#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{

}

void GameScene::Init(void)
{

	// プレイヤー
	player_ = std::make_shared<Player>();
	player_->Init();

	//ユニークポインタコピー不可例
	//使用箇所が複数あるポインタは無理
	//std::unique_ptr<Player> player2_ = player_;

	// ステージ
	//責任者が2人になってしまうから基本的には無理だけど
	//std::moveで所有権を渡すことはできる…std::move(player_)
	//ただ所有権を失ってしまっているから
	//プレイヤーを参照できなくてエラーが出る
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera().SetFollow(&player_->GetTransform());
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	skyDome_->Update();

	stage_->Update();

	player_->Update();

}

void GameScene::Draw(void)
{

	// 背景
	skyDome_->Draw();
	stage_->Draw();
	
	player_->Draw();

	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");

}
