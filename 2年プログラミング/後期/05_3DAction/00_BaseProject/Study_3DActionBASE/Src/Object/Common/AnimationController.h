#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{
	
public :

	// アニメーションデータ
	struct Animation
	{
		int model = -1;
		int attachNo = -1;
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// コンストラクタ
	AnimationController(int modelId);
	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(int type, const std::string& path, float speed);

	// アニメーション再生
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);
	void Release(void);

	// アニメーション終了後に繰り返すループステップ
	void SetEndLoop(float startStep, float endStep, float speed);

	// 再生中のアニメーション
	int GetPlayType(void) const;

	// 再生終了
	bool IsEnd(void) const;

private :

	// モデルのハンドルID
	int mModelId;

	// 種類別のアニメーションデータ
	std::map<int, Animation> mAnimations;

	int mPlayType;
	Animation mPlayAnim;

	// アニメーションをループするかしないか
	bool mIsLoop;

	// アニメーションを止めたままにする
	bool mIsStop;

	// アニメーション終了後に繰り返すループステップ
	float mStepEndLoopStart;
	float mStepEndLoopEnd;
	float mEndLoopSpeed;
	// 逆再生
	float mSwitchLoopReverse;

};

