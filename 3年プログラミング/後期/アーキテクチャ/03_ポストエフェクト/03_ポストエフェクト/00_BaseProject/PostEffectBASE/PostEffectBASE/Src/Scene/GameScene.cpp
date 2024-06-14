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
#include "../Application.h"
#include "GameScene.h"
#include "../Object/PixelMaterial.h"
#include "../Object/PixelRenderer.h"

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
	player_ = std::make_unique<Player>();
	player_->Init();

	// ステージ
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	//int mainScreen = SceneManager::GetInstance().GetMainScreen();
	
	//モノクロ
	monoShaderConstBuf_ = 1;
	matMono_ = std::make_unique<PixelMaterial>("Monotone.cso", monoShaderConstBuf_);
	matMono_->Init();

	//走査線
	slShaderConstBuf_ = 1;
	matScanL_ = std::make_unique<PixelMaterial>("ScanLine.cso", slShaderConstBuf_);
	matScanL_->Init();

	//レンズの歪み
	distorShaderConstBuf_ = 1;
	matDistortion_ = std::make_unique<PixelMaterial>("Distortion.cso", distorShaderConstBuf_);
	matDistortion_->Init();

	//周り暗いやつ
	vignettShaderConstBuf_ = 1;
	matVignett_ = std::make_unique<PixelMaterial>("Vignett.cso", vignettShaderConstBuf_);
	matVignett_->Init();

	int noise = LoadGraph("Data/Image/noise.png");
	//グリッチ
	afterImageShaderConstBuf_ = 1;
	matAfterImage_ = std::make_unique<PixelMaterial>("AfterImage.cso", afterImageShaderConstBuf_);
	matAfterImage_->Init({noise });

	//ぼかし
	blurShaderConstBuf_ = 1;
	matBlur_ = std::make_unique<PixelMaterial>("Blur.cso", blurShaderConstBuf_);
	matBlur_->Init();

	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matMono_)));
	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matScanL_)));
	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matDistortion_)));
	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matVignett_)));
	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matAfterImage_)));
	renderer_.push_back(std::move(std::make_unique<PixelRenderer>(*matBlur_)));


	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);
	
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


	auto time_ = SceneManager::GetInstance().GetTotalTime();

	matScanL_->SetConstBuf(
		{
		FLOAT4{ time_,0.0f,0.0f,0.0f },
		});

	matAfterImage_->SetConstBuf(
		{
		FLOAT4{ time_},
		});

	matBlur_->SetConstBuf(
		{
		FLOAT4{ time_,Application::SCREEN_SIZE_X,Application::SCREEN_SIZE_Y,0.0f },
		});
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

	for (const auto& render : renderer_)
	{
		render->Draw();
	}
}
