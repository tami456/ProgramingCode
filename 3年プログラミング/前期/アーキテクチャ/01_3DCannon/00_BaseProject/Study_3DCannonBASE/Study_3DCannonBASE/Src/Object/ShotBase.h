#pragma once
#include<DxLib.h>

class ShotBase
{
public:

	static constexpr float COL_RADIUS = 10.0f;

	//弾の状態
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	// コンストラクタ(元となるモデルのハンドルID)
	ShotBase(int baseModelId,int* blastImg,int blastAnimNum);

	// デストラクタ
	virtual ~ShotBase(void);

	// 弾の生成(表示開始座標、弾の進行方向)
	void CreateShot(VECTOR pos, VECTOR dir);

	// 更新ステップ
	void Update(void);
	void UpdateShot(void);
	void UpdateBlast(void);
	void UpdateEND(void);

	// 描画
	void Draw();
	void DrawShot(void);
	void DrawBlast(void);
	void DrawEND(void);

	// 解放処理
	void Release(void);

	//弾判定
	bool IsShot(void);

	//爆発処理
	void Blast(void);

	// 生存判定
	bool IsAlive(void);

	VECTOR GetPos(void) const;

private:

	//弾の状態
	STATE state_;

	// 元となる弾のモデルID
	int baseModelId_;

	// 弾のモデルID
	int modelId_;

	// 方向
	VECTOR dir_;

	// 弾の大きさ
	VECTOR scl_;

	// 弾の角度
	VECTOR rot_;

	// 弾の座標
	VECTOR pos_;

	// 弾の移動速度
	float speed_;

	// 弾の生存判定
	bool isAlive_;

	//重力
	float gravityPow_;

	//爆発アニメーション画像配列のポインタ
	int* blastImgs_;

	//爆発アニメーション数
	int blastAnimNum_;

	//爆発のアニメーション用カウンタ
	int blastCntAnim_;

	//爆発アニメーション速度
	float blastSpeedAnim_;

	//爆発のアニメーション番号
	int blastIdxAnim_;

	//状態遷移
	void ChangeState(STATE state);
};

