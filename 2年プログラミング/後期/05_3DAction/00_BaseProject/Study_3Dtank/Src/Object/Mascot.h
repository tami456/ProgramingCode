#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
class ResourceManager;
class AnimationController;

class Mascot
{
public:


	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		IDLE,
		WARP_RESERVE,
		WARP_MOVE,
		ENEMYDEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		ENEMYDEAD
	};

	int ImgMascotIdle;
	int ImgMascotNice;

	Mascot();
	~Mascot();

	void Init(void);

	void Update(void);
	void UpdateIdle(void);
	void UpdateEnemyDead(void);

	void Draw(void);

	void Release(void);

private:
	ResourceManager& mResourceManager;

	Transform mTransform;

	// アニメーション
	AnimationController* mAnimationController;

	// 状態
	STATE mState;

	void ChangeState(STATE state);
};