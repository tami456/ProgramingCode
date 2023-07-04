#pragma once
#include <DxLib.h>
#include "../Object/Common/Transform.h"
class SpaceDome
{
public:

	//状態
	enum class STATE {
		NONE,
		STAY,
		FOLLOW
	};

	// コンストラクタ
	SpaceDome(const Transform* target);

	// デストラクタ
	~SpaceDome(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:

	//モデルの3制御
	Transform mTransform;

	//追従相手
	const Transform* mTarget;

	STATE mState;

	void ChangeState(STATE state);

};

