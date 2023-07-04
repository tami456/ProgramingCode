#include "Transition.h"
#include "../../../_debug/_debug/_DebugDispOut.h"
#include "../../../_debug/_debug/_DebugConOut.h"

Transition::Transition(uniquScene beforScene, uniquScene afterScene, int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	TRACE("Transitionの生成\n");
	//SceneMngのUpdateに説明
	beforScene_ = std::move(beforScene);
	afterScene_ = std::move(afterScene);

}

Transition::~Transition()
{
	
}

bool Transition::Init(void)
{
	return true;
}

uniquScene Transition::Update(uniquScene ownScene)
{
	if (UpdateTransition())
	{
		//これも同じでstd::move
		//終わったのでアフターシーン
		return std::move(afterScene_);
	}
	//ownSceneに入っているものは消滅する
	//道連れでbeforSceneも消滅する
	return ownScene;
}


