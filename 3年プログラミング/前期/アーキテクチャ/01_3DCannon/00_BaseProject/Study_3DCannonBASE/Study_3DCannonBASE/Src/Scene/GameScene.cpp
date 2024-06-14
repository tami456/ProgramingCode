#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/ShotBase.h"
#include "../Object/EnemyBase.h"
#include "../Object/Enemy.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_ = new Stage();
	stage_->Init();

	cannon_ = new Cannon();
	cannon_->Init();

	//カメラをフリーモードにする
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	//敵のモデル
	enemyModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Enemy/Birb.mv1").c_str());

	enemy_ = new EnemyBase(enemyModelId_);

	enemy_->Init();
	enemys_.push_back(enemy_);

	//ゲームオーバー地点
	gameoverPoint_ = { 450.0f,30.0f,75.0f };

	//ゲームオーバー判定
	isGameover_ = false;

	isGameClear_ = false;

	//ゲームオーバー画像
	imgGameover_ = LoadGraph((Application::PATH_IMAGE + "Gameover.png").c_str());

	imgGameClear_ = LoadGraph((Application::PATH_IMAGE + "GameClear.png").c_str());

	auto e = new Enemy(enemyModelId_);
}

void GameScene::Update(void)
{
	//シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (isGameover_ == true || isGameClear_ == true)
	{
		//ゲームオーバーなら処理しない
		return;
	}

	stage_->Update();
	cannon_->Update();
	enemy_->Update();


	//ステージモデルID
	int stageModelId = stage_->GetModelId();

	auto shots = cannon_->GetShots();
	for (auto shot : shots)
	{
		shot->Update();

		if (!shot->IsShot())
		{
			//爆発や処理終了後は、以降の処理は実行しない
			continue;
		}

		//ステージモデルとの衝突判定
		auto info = MV1CollCheck_Sphere(
			stageModelId, -1, shot->GetPos(), ShotBase::COL_RADIUS);

		if (info.HitNum > 0)
		{
			shot->Blast();
		}

		//当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);

		//敵との衝突判定
		for (auto enemy : enemys_)
		{
			//基本教材15から！！
			
			//球体と球体の衝突判定
			if (AsoUtility::IsHitSpheres(shot->GetPos(),ShotBase::COL_RADIUS,
				enemy->GetCollisionPos(),enemy->GetCollisionRadius()))
			{
				//敵にダメージを与える
				enemy->Damage(1);
				shot->Blast();
				break;
			}
			
		}
	}
	//敵との衝突判定
	for (auto enemy : enemys_)
	{
		//球体と球体の衝突判定
		if (AsoUtility::IsHitSpheres(gameoverPoint_, OVER_COL_RADIUS,
			enemy->GetCollisionPos(), enemy->GetCollisionRadius()))
		{
			isGameover_ = true;
			break;
		}
	}

	if (enemy_->IsAlive() == false)
	{
		isGameClear_ = true;
	}
}

void GameScene::Draw(void)
{
	stage_->Draw();
	cannon_->Draw();
	enemy_->Draw();
	DrawSphere3D(gameoverPoint_, OVER_COL_RADIUS, 10,0xff0000, 0xff0000, false);
	auto pos = enemy_->GetPos();
	
	//ゲームオーバー画像の表示
	if (isGameover_)
	{
		DrawGraph((Application::SCREEN_SIZE_X - 651) / 2,
			(Application::SCREEN_SIZE_Y - 109) / 2, imgGameover_, true);
	}

	if (isGameClear_)
	{
		DrawGraph((Application::SCREEN_SIZE_X - 651) / 2,
			(Application::SCREEN_SIZE_Y - 109) / 2, imgGameClear_, true);
	}
}

void GameScene::Release(void)
{
	delete stage_;

	delete cannon_;

	delete enemy_;
}
