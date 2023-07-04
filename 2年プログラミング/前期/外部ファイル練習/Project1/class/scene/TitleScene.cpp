#include <DxLib.h>
#include <memory>
#include "TitleScene.h"
#include "GameScene.h"
#include "transition/Transition.h"
#include "transition/CrossOver.h"
#include "Input/InputID.h"
#include "../ImageMng.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"

TitleScene::TitleScene(int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	Init();
	//クラス"BaseScene"の既定のコンストラクターは存在しません…暗黙的に呼ばれない
	//解決の仕方…コンストラクタの文字のすぐ後に：を書いて呼び出す…明示的と呼ぶ
	//既定…あらかじめ決められたもの
	//デフォルトコンストラクタがないからエラーが出る
	//デフォルトコンストラクタの役目…インスタンス時に勝手に呼ばれる
	TRACE("TitleSceneの生成\n");
	DrawOwnScreen();
}

TitleScene::~TitleScene()
{
	TRACE("TitleSceneの破棄\n");
	
}

bool TitleScene::Init(void)
{
	lpImageMng.GetID( "image/map.png","TitleBG");
	return true;
}

uniquScene TitleScene::Update(uniquScene ownScene)
{
	DrawOwnScreen();
	if (CheckHitKey(KEY_INPUT_SPACE) || GetJoypadInputState(PAD_INPUT_3))
	{
		//TitleSceneだったものをGameSceneとして確定する
		//ownScene = TitleScene
		//ownScene = GameSceneにする
		
		return std::make_unique<CrossOver>(std::move(ownScene),
			std::make_unique<GameScene>(screenSize_.x, screenSize_.y),
			screenSize_.x, screenSize_.y);
	}
	//自分自身を返す
	//SceneMngのscene_が入っている
	
	//ローカル変数…関数を抜けると消滅する
	//GameSceneをreturnした後消滅する
	//破棄タイミングを意識しなくてよくなる
	return ownScene;
}

void TitleScene::DrawOwnScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(0, 0,lpImageMng.GetID("TitleBG")[0], true);
}
