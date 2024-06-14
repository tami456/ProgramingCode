#pragma once
#include <vector>
#include <DxLib.h>

class ShotBase;
class Camera;

class Cannon
{
public:

	// 砲身のX回転制限(最大)
	static constexpr float MAX_ANGLE_X = 15.0f * DX_PI_F / 180.0f;

	// 砲身のX回転制限(最小)
	static constexpr float MIN_ANGLE_X = -30.0f * DX_PI_F / 180.0f;

	//弾発射後の硬直時間
	static constexpr float SHOT_DELAY = 1.0f;

	//爆発のサイズ
	static constexpr int BLAST_SIZE_X = 32;
	static constexpr int BLAST_SIZE_Y = 32;

	//爆発のアニメーション数
	static constexpr int BLAST_ANIM_NUM = 16;

	Cannon();
	~Cannon();

	void Init();
	void Update();
	void Draw();
	void Release();

	std::vector<ShotBase*> GetShots(void);

private:

	VECTOR standScl_;
	VECTOR standRot_;
	VECTOR standPos_;

	VECTOR barrelScl_;
	VECTOR barrelRot_;
	VECTOR barrelPos_;

	//砲台からの相対座標
	VECTOR barrelLocalPos_;


	// ３ＤモデルのハンドルID
	int barrelModelId_;
	int standModelId_;

	//ショット(ポインタ)
	std::vector<ShotBase*> shots_;

	//弾のモデルID
	int shotModelId_;
	int spikeShotModelId_;

	float stepShotDelay;

	//爆発の画像(本来は外部ソース用の管理クラスを作るべき。弾もモデル。)
	int blastImgs_[BLAST_ANIM_NUM];

	void ProcessRot(void);

	//発射操作
	void ProcessShot(void);

	//有効な弾を取得する
	ShotBase* GetValidShot(void);
};

