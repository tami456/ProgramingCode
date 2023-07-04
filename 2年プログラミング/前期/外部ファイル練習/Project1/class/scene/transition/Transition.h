#pragma once
#include "../BaseScene.h"
class Transition :
	public BaseScene
{
public:
	//new old before after
	//後ろか前に書くかは周りに合わせる

	Transition(uniquScene beforScene , uniquScene afterScene , int screenSizeX, int screenSizeY);
	~Transition();
private:
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	//派生でしか使わない
	virtual bool UpdateTransition(void) = 0;

protected:
	//beforやafterのSceneはコンストラクタから出ると消滅してしまう
	uniquScene beforScene_;
	uniquScene afterScene_;
};

