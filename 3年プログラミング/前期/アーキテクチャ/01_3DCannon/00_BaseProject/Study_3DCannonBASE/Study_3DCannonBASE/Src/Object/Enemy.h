#pragma once
#include "EnemyBase.h"
class Enemy :
    public EnemyBase
{
public:
    Enemy(int baseModelId);
    ~Enemy();

	// 初期処理(最初の１回のみ実行)
	void Init(void) override;

	// パラメータ設定(純粋仮想関数)
	void SetParam(void) override;

	// 更新処理(毎フレーム実行)
	void Update(void) override;

	// 描画処理(毎フレーム実行)
	void Draw(void) override;

	// 解放処理(最後の１回のみ実行)
	void Release(void) override;

private:
};

