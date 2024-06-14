#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "ShotBase.h"
#include "../Manager/Camera.h"
#include "Cannon.h"

Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

void Cannon::Init()
{
	standScl_ = { 0.8f,0.8f,0.8f };
	standRot_ = { 0.0f,0.0f,0.0f };
	standPos_ = { 0.0f,10.0f,-200.0f };

	barrelScl_ = { 0.8f,0.8f,0.8f };
	barrelRot_ = { 0.0f,0.0f,0.0f };
	barrelPos_ = { 0.0f,0.0f,0.0f };


	//位置の設定
	//土台からの相対座標とする
	barrelLocalPos_ = { 0.0f,120.0f,0.0f };

	barrelPos_ = VAdd(standPos_, barrelLocalPos_);

	// 外部ファイルの３Ｄモデルをロード
	barrelModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());
	standModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Stand.mv1").c_str());

	//弾のモデル
	shotModelId_ =
		MV1LoadModel((Application::PATH_MODEL + "Cannon/Shot.mv1").c_str());
	
	spikeShotModelId_ =
		MV1LoadModel((Application::PATH_MODEL + "Cannon/SpikeShot.mv1").c_str());

	//爆発エフェクト読み込み
	LoadDivGraph((Application::PATH_IMAGE + "Blast.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	//弾発射の硬直時間
	stepShotDelay = 0.0f;

	//初期設定をモデルに反映(最初は実装しない)
	Update();
}

void Cannon::Update()
{
	

	MV1SetPosition(standModelId_, standPos_);
	MV1SetRotationXYZ(standModelId_, standRot_);
	MV1SetPosition(barrelModelId_, barrelPos_);
	MV1SetRotationXYZ(barrelModelId_, barrelRot_);
	for (auto shot : shots_)
	{
		shot->Update();
	}
	ProcessRot();
	ProcessShot();

	
}

void Cannon::Draw()
{
	MV1DrawModel(standModelId_);
	
	//方針のモデル描画
	MV1DrawModel(barrelModelId_);

	for (auto shot : shots_)
	{
		shot->Draw();
	}

}

void Cannon::Release()
{
	MV1DeleteModel(barrelModelId_);
	MV1DeleteModel(standModelId_);
	MV1DeleteModel(shotModelId_);
	MV1DeleteModel(spikeShotModelId_);

	for (auto shot : shots_)
	{
		shot->Release();
		delete shot;
	}

	// 読み込んだ画像の解放
	for (int i = 0; i < BLAST_ANIM_NUM; i++)
	{
		DeleteGraph(blastImgs_[i]);
	}
}

std::vector<ShotBase*> Cannon::GetShots(void)
{
	return shots_;
}

void Cannon::ProcessRot()
{
	auto& ins = InputManager::GetInstance();
	float barrelRad = 1.0 * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_RIGHT)) 
	{
		barrelRot_.y = standRot_.y  += 1.0 * DX_PI_F / 180.0f;
	}

	if(ins.IsNew(KEY_INPUT_LEFT))
	{
		barrelRot_.y = standRot_.y -= 1.0 * DX_PI_F / 180.0f;
	}


	if (ins.IsNew(KEY_INPUT_UP)) 
	{ 
		barrelRot_.x += barrelRad; 
		if (barrelRot_.x > MAX_ANGLE_X)
		{
			barrelRot_.x = MAX_ANGLE_X;
		}
	}

	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		barrelRot_.x -= barrelRad;
		if (barrelRot_.x < MIN_ANGLE_X)
		{
			 barrelRot_.x = MIN_ANGLE_X;
		}
	}

	
}

void Cannon::ProcessShot(void)
{
	// 弾の発射位置
	VECTOR pos = barrelPos_;
	// 砲身からの相対座標
	VECTOR localPos = { 0.0f, 25.0f, 30.0f };

	

	//単位行列(無回転の状態)
	MATRIX matRot = MGetIdent();

	//それぞれの軸の行列をつくり、さらに行列を合成していく
	matRot = MMult(matRot, MGetRotX(barrelRot_.x));
	matRot = MMult(matRot, MGetRotY(barrelRot_.y));
	matRot = MMult(matRot, MGetRotZ(barrelRot_.z));

	//回転行列を使用して、ベクトルを回転させる
	VECTOR dir = VTransform({ 0.0f,0.0f,1.0f }, matRot);

	// 方向と同じ要領で、相対座標を回転
	VECTOR localPosRot = VTransform(localPos, matRot);

	pos = VAdd(pos, localPosRot);

	auto& ins = InputManager::GetInstance();

	//攻撃キーを押すと、弾を生成
	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay <= 0.0f)
	{
		//有効な球を取得する
		ShotBase* shot = GetValidShot();

		//弾を生成(方向は仮で正面方向)
		shot->CreateShot(pos, dir);

		//玉発射後の硬直時間セット
		stepShotDelay = SHOT_DELAY;
	}

	//弾発射後の硬直時間を減らしていく
	if (stepShotDelay > 0.0f)
	{
		stepShotDelay -= 1.0f / SceneManager::DEFFAULT_FPS;
	}
}

ShotBase* Cannon::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}

	ShotBase* shot = new ShotBase(shotModelId_, blastImgs_, BLAST_ANIM_NUM);
	
	shots_.push_back(shot);

	return shot;
}

