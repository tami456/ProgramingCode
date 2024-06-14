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

	//�J�������t���[���[�h�ɂ���
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	//�G�̃��f��
	enemyModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Enemy/Birb.mv1").c_str());

	enemy_ = new EnemyBase(enemyModelId_);

	enemy_->Init();
	enemys_.push_back(enemy_);

	//�Q�[���I�[�o�[�n�_
	gameoverPoint_ = { 450.0f,30.0f,75.0f };

	//�Q�[���I�[�o�[����
	isGameover_ = false;

	isGameClear_ = false;

	//�Q�[���I�[�o�[�摜
	imgGameover_ = LoadGraph((Application::PATH_IMAGE + "Gameover.png").c_str());

	imgGameClear_ = LoadGraph((Application::PATH_IMAGE + "GameClear.png").c_str());

	auto e = new Enemy(enemyModelId_);
}

void GameScene::Update(void)
{
	//�V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (isGameover_ == true || isGameClear_ == true)
	{
		//�Q�[���I�[�o�[�Ȃ珈�����Ȃ�
		return;
	}

	stage_->Update();
	cannon_->Update();
	enemy_->Update();


	//�X�e�[�W���f��ID
	int stageModelId = stage_->GetModelId();

	auto shots = cannon_->GetShots();
	for (auto shot : shots)
	{
		shot->Update();

		if (!shot->IsShot())
		{
			//�����⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
			continue;
		}

		//�X�e�[�W���f���Ƃ̏Փ˔���
		auto info = MV1CollCheck_Sphere(
			stageModelId, -1, shot->GetPos(), ShotBase::COL_RADIUS);

		if (info.HitNum > 0)
		{
			shot->Blast();
		}

		//�����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);

		//�G�Ƃ̏Փ˔���
		for (auto enemy : enemys_)
		{
			//��{����15����I�I
			
			//���̂Ƌ��̂̏Փ˔���
			if (AsoUtility::IsHitSpheres(shot->GetPos(),ShotBase::COL_RADIUS,
				enemy->GetCollisionPos(),enemy->GetCollisionRadius()))
			{
				//�G�Ƀ_���[�W��^����
				enemy->Damage(1);
				shot->Blast();
				break;
			}
			
		}
	}
	//�G�Ƃ̏Փ˔���
	for (auto enemy : enemys_)
	{
		//���̂Ƌ��̂̏Փ˔���
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
	
	//�Q�[���I�[�o�[�摜�̕\��
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
