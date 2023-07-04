#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "Player.h"
#include "Planet.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(Player* player) : mResourceManager(ResourceManager::GetInstance())
{
	mPlayer = player;
	mActivePlanet = nullptr;
	mActiveName = NAME::MAIN_PLANET;
	mStep = 0.0f;
}

void Stage::Init(void)
{
	MakeMainStage();
	MakeFallPlanet();
	MakeFlatPlanet();
	MakeLastPlanet();
	MakeSpecialPlanet();
	MakeWarpStar();
	MakeGoalStar();

	mStep = -1.0f;
}

void Stage::Update(void)
{

	// 惑星
	for (auto& s : mPlanets)
	{
		s.second->Update();

		if (mActiveName != s.second->GetName())
		{
			//今アクティブになっていない惑星との
			//重力判定を行う
			if (s.second->InRangeGravity(mPlayer->GetTransform()->pos))
			{
				//惑星の重力圏内に入った
				//重力圏内に入った惑星の名前を渡していく
				ChangeStage(s.second->GetName());
			}

			if (s.second->InRangeDead(mPlayer->GetTransform()->pos))
			{
				mPlayer->IsDead();
			}
		}
	}

}

void Stage::Draw(void)
{

	// 惑星
	for (const auto s : mPlanets)
	{
		s.second->Draw();
	}

}

void Stage::Release(void)
{

	// 惑星
	for (const auto s : mPlanets)
	{
		s.second->Release();
		delete s.second;
	}
	mPlanets.clear();

}

void Stage::ChangeStage(NAME type)
{

	mActiveName = type;

	// 対象のステージを取得する
	mActivePlanet = GetPlanet(mActiveName);

	// ステージの当たり判定をプレイヤーに設定
	if (mPlayer != nullptr)
	{
		mPlayer->ClearCollider();
		mPlayer->AddCollider(mActivePlanet->GetTransform()->collider);
	}

	// 重力制御に惑星を渡す
	GravityManager::GetInstance().ChangeActivePlanet(mActivePlanet);

	mStep = TIME_STAGE_CHANGE;

}

Planet* Stage::GetPlanet(NAME type)
{
	if (mPlanets.count(type) == 0)
	{
		return nullptr;
	}

	return mPlanets[type];
}

void Stage::MakeMainStage(void)
{

	// 最初の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	planetTrans.scl = AsoUtility::VECTOR_ONE;
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { 0.0f, -100.0f, 0.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet = new Planet(name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeFallPlanet(void)
{

	// 落とし穴の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FALL_PLANET));
	float scale = 10.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -12000.0f, 4100.0f, 10200.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::FALL_PLANET;
	Planet* planet = new Planet(name, Planet::TYPE::SPHERE, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(3000.0f);
	planet->SetDeadLength(500.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeFlatPlanet(void)
{

	Transform planetTrans;
	Planet* planet;
	NAME name;
	float size;
	VECTOR scale;

	// ① GROUNDタイプ(重力がY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	size = 10.0f;
	planetTrans.scl = { size, size, size };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -18000.0f, 7000.0f, 15000.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_BASE;
	planet = new Planet(name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(1300.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

	// 以後の惑星のスケール
	//------------------------------------------------------------------------------
	size = 3.0f;
	scale = { size, size, size };
	//------------------------------------------------------------------------------

	// ② TRANS_ROTタイプ(重力が回転方向のY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-10.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 16800.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT01;
	planet = new Planet(name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// ③ TRANS_ROTタイプ(重力が回転方向のY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-70.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 17900.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT02;
	planet = new Planet(name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// ④ TRANS_ROTタイプ(重力が回転方向のY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(-45.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 19000.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT03;
	planet = new Planet(name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// ⑤ TRANS_ROTタイプ(重力が回転方向のY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(0.0f));
	planetTrans.pos = { -18000.0f, 7000.0f, 20100.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_ROT04;
	planet = new Planet(name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// ⑥ TRANS_CAMERA_FIXEDタイプ(重力が回転方向のY+だがカメラ固定)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_02));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(60.0f));
	planetTrans.pos = { -18000.0f, 7200.0f, 21200.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_FIXED01;
	planet = new Planet(name, Planet::TYPE::TRANS_CAMERA_FIXED, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(420.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------


	// ⑦ TTRANS_ROTタイプ(重力が回転方向のY+)
	//------------------------------------------------------------------------------
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::FLAT_PLANET_01));
	planetTrans.scl = scale;
	planetTrans.quaRot = Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(0.0f));
	planetTrans.pos = { -18000.0f, 7300.0f, 22400.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	name = NAME::FLAT_PLANET_FIXED02;
	planet = new Planet(name, Planet::TYPE::TRANS_ROT, planetTrans);
	planet->Init();
	planet->SetGravityPow(12.0f);
	planet->SetGravityRadius(520.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeLastPlanet(void)
{

	// 最後の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::LAST_PLANET));
	float scale = 8.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(-15.0f), 0.0f, 0.0f);
	planetTrans.pos = { -18000.0f, 8400.0f, 25200.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::LAST_STAGE;
	Planet* planet = new Planet(name, Planet::TYPE::SPHERE, planetTrans);
	planet->Init();
	planet->SetGravityPow(20.0f);
	planet->SetGravityRadius(2300.0f);
	planet->SetDeadLength(0.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeSpecialPlanet(void)
{

	// 特別な惑星惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		mResourceManager.LoadModelDuplicate(ResourceManager::SRC::SPECIAL_PLANET));
	float scale = 8.0f;
	planetTrans.scl = { scale, scale, scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -6000.0f, -150.0f, -1500.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::SPECIAL_STAGE;
	Planet* planet = new Planet(name, Planet::TYPE::ROAD, planetTrans);
	planet->Init();
	planet->SetGravityRadius(700.0f);
	planet->SetDeadLength(1000.0f);
	mPlanets.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeWarpStar(void)
{

	Transform trans;

	// テスト惑星へのワープスター
	//------------------------------------------------------------------------------
	//trans.pos = { 0.0, 50.0f, -600.0f };
	//trans.scl = { 0.6f, 0.6f, 0.6f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(0.0f),
	//	AsoUtility::Deg2RadF(180.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);

	//star = new WarpStar(mPlayer, trans);
	//star->Init();
	//mWarpStars.push_back(star);
	//------------------------------------------------------------------------------

	// 落とし穴惑星へのワープスター
	//------------------------------------------------------------------------------
	trans.pos = { -910.0f, 200.0f, 894.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-25.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	//------------------------------------------------------------------------------


	// 平坦な惑星へのワープスター
	//------------------------------------------------------------------------------
	trans.pos = { -11360.0f, 6200.0f, 9600.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-12.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	//------------------------------------------------------------------------------

}

void Stage::MakeGoalStar(void)
{
}
