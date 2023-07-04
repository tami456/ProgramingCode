#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;
class AnimationController;


class GameOverScene:public SceneBase
{
public:

	// コンストラクタ
	GameOverScene(void);

	// デストラクタ
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 画像
	int mImgBG;
	int mImgPush;
	int mImgGameOver;

	// 背景
	SkyDome* mSkyDome;

	// 惑星
	Transform mPlanet;
	Transform mMovePlanet;

	// キャラクター
	Transform mChara;

	// アニメーション
	AnimationController* mAnimationController;
};

