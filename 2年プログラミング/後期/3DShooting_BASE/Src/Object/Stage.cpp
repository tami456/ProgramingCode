#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	//ダンジョン
	mModelDungeon = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::DUNGEON
	);

	mPosDungeon = { 1000.0f,400.0f,1300.0f };
	MV1SetScale(mModelDungeon,AsoUtility::VECTOR_ONE);
	MV1SetRotationXYZ(mModelDungeon, { 0.0f,180.0f,0.0f });
	MV1SetPosition(mModelDungeon, mPosDungeon);
	
	//戦艦
	mModelCargoShip = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::BOSS_SHIP
	);

	mPosCargoShip = { 14500.0f,-800.0f,6500.0f };
	MV1SetScale(mModelCargoShip, {2.0f,2.0f,2.0f});
	MV1SetRotationXYZ(mModelCargoShip, { 0.0f,180.0f,0.0f });
	MV1SetPosition(mModelCargoShip, mPosCargoShip);

	//当たり判定(コライダ)作成
	int ret;
	ret = MV1SetupCollInfo(mModelDungeon, -1);
	ret = MV1SetupCollInfo(mModelCargoShip, -1);
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(mModelDungeon);
	MV1DrawModel(mModelCargoShip);

	DrawGrid();

	//デバッグ用：戦艦の周囲を描画
	DrawSphere3D(mPosCargoShip,5000.0f,20,0xff0000,0xff0000,false);
	
}

void Stage::DrawGrid(void)
{
	float len = LEN_MASS * LEN_NUM;
	int hSize = AsoUtility::Round(LEN_NUM / 2.0f);
	float hLen = len / 2.0f;
	for (int i = -hSize; i < (hSize + 1); i++)
	{
		DrawLine3D({ -hLen,0.0f,i * LEN_MASS }, { hLen,0.0f,i * LEN_MASS }, 0xff0000);

		DrawLine3D({ i * LEN_MASS,0.0f,hLen }, { i * LEN_MASS,0.0f,-hLen }, 0x0000ff);
	}

	DrawLine3D(VGet(0.0f, -300.0f, 0.0f), VGet(0.0f, 600.0f, 0.0f), 0x00ff00);
}

void Stage::Release(void)
{
}

int Stage::GetModelDungeon(void)
{
	return mModelDungeon;
}

int Stage::GetModelCargoShip(void)
{
	return mModelCargoShip;
}

VECTOR Stage::GetPosCargoShip(void)
{
	return mPosCargoShip;
}
