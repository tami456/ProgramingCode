#pragma once
#include <DxLib.h>
#include <vector>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"

class Transform;
class Particle;

class ParticleGenerator
{
public:

	
	static constexpr int NUM_CREATE = 30;
	//自作円形メッシュ
	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f / NUM_ANGLE;

	//徐々に生成
	static constexpr float GEN_TIME = 0.1f;

	// コンストラクタ
	ParticleGenerator(VECTOR pos,float radius);

	// デストラクタ
	~ParticleGenerator(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawMeshSquare(void);
	void DrawMeshCircle(void);
	void Release(void);

	void SetPos(const VECTOR& pos);
	void SetRot(const Quaternion& rot);
private:

	//パーティクル画像
	int mImg;

	//座標
	VECTOR mPos;

	//向き(角度)
	Quaternion mRot;

	
	//半径
	float mRadius;
	
	Transform* mTransform;

	//頂点数
	int mCntVertex;

	float mTestRad;

	//3D描画に使用する頂点データの型(四角用)
	VERTEX3D mVerticesSquare[6];

	//頂点データ(円形)
	VERTEX3D mVerticesCircle[NUM_ANGLE + 1];

	//頂点インデックス
	WORD mIndexes[NUM_ANGLE * 3];

	//パーティクル
	std::vector<Particle*> mParticles;

	//半徐々に生成するための経過時間
	float mStepGenerate;

	//四角メッシュを作る関数
	void CreateMeshSquare(void);

	//円形メッシュを作る関数
	void CreateMeshCircle(void);

	Particle* Generate(Particle* particle);

	//Zソート
	void ZSort(void);
};

