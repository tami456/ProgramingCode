#include "ParticleGenerator.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/Camera.h"
#include "Common/Transform.h"
#include "../Common/Quaternion.h"
#include "Particle.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"


ParticleGenerator::ParticleGenerator(VECTOR pos, float radius)
{
	mPos = pos;
	mRadius = radius;
}

ParticleGenerator::~ParticleGenerator(void)
{
}

void ParticleGenerator::Init(void)
{
	mImg = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::LIGHT
	).mHandleId;



	//四角メッシュを作成
	CreateMeshSquare();

	//円形メッシュを作成
	CreateMeshCircle();

	//一気に作る
	/*for (int i = 0; i < NUM_CREATE; i++)
	{
		mParticles.emplace_back(Generate(nullptr));
	}*/

	//徐々に作る
	for (int i = 0; i < NUM_CREATE; i++)
	{
		mParticles.emplace_back(new Particle());
	}

	//角度の初期化
	mRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	mStepGenerate = GEN_TIME;
}

void ParticleGenerator::Update(void)
{

	bool isGenerate = false;
	mStepGenerate -= SceneManager::GetInstance().GetDeltaTime();
	if (mStepGenerate < 0.0f)
	{
		isGenerate = true;
		
	}

	for (auto& p : mParticles)
	{
		p->Update();

		//生存時間が切れたさら再生成
		if (!p->isAlive())
		{
			if (isGenerate)
			{
				p = Generate(p);
				mStepGenerate = GEN_TIME;
				isGenerate = false;
			}
			
		}
	}


#ifdef _DEBUG

	//ParticleGenerator = mRotの回転処理
	bool isHitKey = false;
	//右回転
	if (CheckHitKey(KEY_INPUT_L))
	{
		isHitKey = true;
		Quaternion axis = Quaternion::AngleAxis(AsoUtility::Deg2RadD(1.0), { 0.0f,1.0f,0.0f });
		mRot = mRot.Mult(axis);
	}
	//左回転
	if (CheckHitKey(KEY_INPUT_J))
	{
		isHitKey = true;
		Quaternion axis = Quaternion::AngleAxis(AsoUtility::Deg2RadD(-1.0), { 0.0f,1.0f,0.0f });
		mRot = mRot.Mult(axis);
	}
	//上回転
	if (CheckHitKey(KEY_INPUT_I))
	{
		isHitKey = true;
		Quaternion axis = Quaternion::AngleAxis(AsoUtility::Deg2RadD(-1.0), { 1.0f,0.0f,0.0f });
		mRot = mRot.Mult(axis);
	}
	//下回転
	if (CheckHitKey(KEY_INPUT_K))
	{
		isHitKey = true;
		Quaternion axis = Quaternion::AngleAxis(AsoUtility::Deg2RadD(1.0), { 1.0f,0.0f,0.0f });
		mRot = mRot.Mult(axis);
	}

	if (isHitKey)
	{
		CreateMeshCircle();
	}

#endif
}

void ParticleGenerator::Draw(void)
{
	//DrawMeshSquare();
	//DrawMeshCircle();

	//Zソート
	ZSort();

	SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
	for (auto& p : mParticles)
	{
		p->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Quaternion tmpQ = Quaternion::Euler(
		AsoUtility::Deg2RadF(-90.0f),
		0.0f, 0.0f
	);

	tmpQ = mRot.Mult(tmpQ);

	DrawLine3D(mPos, VAdd(mPos, VScale(tmpQ.GetForward(), 300.0f)), 0xffffff);
}

void ParticleGenerator::DrawMeshSquare(void)
{
	DrawPolygon3D(mVerticesSquare, 2, DX_NONE_GRAPH, true);

	for (auto& vert : mVerticesSquare)
	{
		DrawSphere3D(vert.pos, 2.0f, 5, 0xff0000, 0xff0000, true);
	}
}

void ParticleGenerator::DrawMeshCircle(void)
{
	DrawPolygonIndexed3D(
		mVerticesCircle, NUM_ANGLE + 1, mIndexes, NUM_ANGLE,
		DX_NONE_GRAPH, true);

	for (auto& vert : mVerticesCircle)
	{
		DrawSphere3D(vert.pos,2.0f,5,0xff0000, 0xff0000, true);
	}
}

void ParticleGenerator::Release(void)
{
	for (auto& p : mParticles)
	{
		if (p != nullptr)
		{
			p->Release();
			delete p;
		}
		
	}
	mParticles.clear();
}

void ParticleGenerator::SetPos(const VECTOR& pos)
{
	mPos = pos;
	//デバッグ用
	CreateMeshCircle();
}

void ParticleGenerator::SetRot(const Quaternion& rot)
{
	mRot = rot;
}

void ParticleGenerator::CreateMeshSquare(void)
{
	//ディフューズカラー
	COLOR_U8 color = GetColorU8(255, 255, 255, 255);
	COLOR_U8 color2 = GetColorU8(255, 0, 0, 255);

	float z = 0.0f;
	float h = 100.0f;

	//四角を作る
	mVerticesSquare[0].pos = { z,z,z };
	mVerticesSquare[0].dif = color;
	mVerticesSquare[1].pos = { z,z,h };
	mVerticesSquare[1].dif = color;
	mVerticesSquare[2].pos = {h,z,h};
	mVerticesSquare[2].dif = color;

	mVerticesSquare[3].pos = { z,z,z };
	mVerticesSquare[3].dif = color2;
	mVerticesSquare[4].pos = { h,z,h };
	mVerticesSquare[4].dif = color2;
	mVerticesSquare[5].pos = { h,z,z };
	mVerticesSquare[5].dif = color2;




}

void ParticleGenerator::CreateMeshCircle(void)
{

	//ディフューズカラー
	COLOR_U8 color = GetColorU8(255, 255, 0, 255);

	mCntVertex = 0;

	//円の中心
	mVerticesCircle[0].pos = mPos;
	mVerticesCircle[0].dif = color;

	//円形の頂点を作成
	int cntAngle = 0;
	for (int i = 0; i < NUM_ANGLE; i++)
	{
		//クォータニオン
		Quaternion rot = Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);
		
		//mRotを使用して、位置や方向を回転させる
		rot = mRot.Mult(rot);
		
		/*int cntAngle = 0;
		for (int i = 0; i < NUM_ANGLE; i++)
		{
			MATRIX rot = MGetRotAxis(
				AsoUtility::DIR_U,
				AsoUtility::Deg2RadF(cntAngle));
		}*/

		//座標を回転
		VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

		

		VERTEX3D vertex = VERTEX3D();

		//中心座標に加えて頂点座標にする
		vertex.pos = VAdd(mPos, localPos);

		vertex.dif = color;

		mVerticesCircle[i + 1] = vertex;

		cntAngle += SPLIT_ANGLE;

	}

	//頂点インデックス
	int cntIdx = 0;

	int cntVIdx = 1;
	for (int i = 1; i < NUM_ANGLE; i++)
	{
		mIndexes[cntIdx++] = 0;
		mIndexes[cntIdx++] = cntVIdx;
		mIndexes[cntIdx++] = ++cntVIdx;
	}
	mIndexes[cntIdx++] = 0;
	mIndexes[cntIdx++] = cntVIdx;
	mIndexes[cntIdx++] = 1;
}

Particle* ParticleGenerator::Generate(Particle* particle)
{
	

	if (particle == nullptr)
	{
		particle = new Particle();
	}

	//位置大きさ方向ランダムな方向

	float size = 20.0f;
	float speed = 20.0f;
	float lifeTime = 10.0f;

	//大きさ
	const int MIN_SCL = 5.0f;
	const int MAX_SCL = 10.0f;

	int s = MIN_SCL + GetRand(MAX_SCL - MIN_SCL);
	size = static_cast<float>(s);

	//スピード
	const int MIN_SP = 15.0f;
	const int MAX_SP = 25.0f;

	int h = MIN_SP + GetRand(MAX_SP - MIN_SP);
	speed = static_cast<float>(h);

	//生存時間
	const int MIN_LIFE = 5.0f;
	const int MAX_LIFE = 10.0f;

	int l = MIN_LIFE + GetRand(MAX_LIFE - MIN_LIFE);
	lifeTime = static_cast<float>(l);

	//位置
	//クォータニオンバージョン
	Quaternion rotY = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(static_cast<float>(GetRand(360))),
		AsoUtility::AXIS_Y);

	rotY = mRot.Mult(rotY);

	//座標を回転
	float min = mRadius * (3.0f/4.0f);
	VECTOR pos = rotY.PosAxis(
		{ 0.0f,0.0f,static_cast<float>(
			min + GetRand(AsoUtility::Round(mRadius - min))) });

	//中心座標に加える
	VECTOR randPos = VAdd(mPos, pos);

	//位置
	//自作バージョン
	/*const int MIN_POS = -20.0f;
	const int MAX_POS = 20.0f;

	VECTOR p;
	p.x = MIN_POS + GetRand(MAX_POS - MIN_POS);
	p.y = { 0.0f };
	p.z = MIN_POS + GetRand(MAX_POS - MIN_POS);

	pos.x = static_cast<float>(p.x);
	pos.y = static_cast<float>(p.y);
	pos.z = static_cast<float>(p.z);*/

	
	
	//方向
	//クォータニオンバージョン
	const int MIN_DIR = -40;
	const int MAX_DIR = -40;

	float randDegX = MIN_DIR + GetRand(MAX_DIR - MIN_DIR);

	Quaternion rotX = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(randDegX), 
		AsoUtility::AXIS_X);
	rotX = rotY.Mult(rotX);
	
	//VECTOR dir = VTransform({0.0f,0.0f,1.0f}, rotX.ToMatrix());
	VECTOR dir = rotX.GetForward();


	//方向
	//自作バージョン
	/*const int MIN_DIR = 0.0f;
	const int MAX_DIR = 1.0f;

	VECTOR d;
	d.x = MIN_DIR + GetRand(MAX_DIR - MIN_DIR);
	d.y = { 1.0f };
	d.z = MIN_DIR + GetRand(MAX_DIR - MIN_DIR);

	dir.x = static_cast<float>(d.x);
	dir.y = static_cast<float>(d.y);
	dir.z = static_cast<float>(d.z);*/

	

	particle->Generate(mImg, randPos, size, dir,speed, lifeTime);

	return particle;
}

void ParticleGenerator::ZSort(void)
{
	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//カメラの方向
	VECTOR cameraDir = camera->GetDir();
	
	for (auto p : mParticles)
	{
		//カメラから見たエフェクト方向
		//Z距離を設定
		//camera2Particle
		//cameraDir
		VECTOR camera2Particle;
		camera2Particle = VSub(p->GetPos(), camera->GetPos());

		
		
		//VDot = 内積の計算関数
		auto zSort = VDot(cameraDir, camera2Particle);
		p->SetZLen(zSort);
	}

	sort(mParticles.begin(), mParticles.end(),
		[](Particle* x, Particle* y)
		{
			return x->GetZLen() > y->GetZLen();
		});
}
