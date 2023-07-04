#include <memory>
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "../_debug/_DebugDispOut.h"
#include "../_debug/_DebugConOut.h"

//SceneMng* SceneMng::s_Instance = nullptr;		//ポインタ
constexpr int ScreenSizeX = 640;
constexpr int ScreenSizeY = 480;

SceneMng::SceneMng()
{
	initFlag_ = SysInit();
}

SceneMng::~SceneMng()
{
}

void SceneMng::Run(void)
{
	if (!initFlag_)		//失敗したら、何もしない
	{
		return;
	}
	std::unique_ptr<BaseScene> scene = std::make_unique<TitleScene>();

	//引数付きコンストラクタを呼びたいときは、丸かっこの中に、引数を書く
	scene.reset(new TitleScene());



	//reset()のみ→中身を殻にする	

	//ドットで出てくるgetはunique_ptrクラスのメンバー関数、メンバー変数
	//->では飛んで、BaseSceneの関数が使える

	//BaseScene* scene = new TitleScene();

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgStartDraw();
		//scene.get()->Update();
		//(*scene).Update();	//実体にアクセス または->

		scene = scene->Update(std::move(scene));	//BaseSceneが基準だが、今いるシーンがTitleなので、TitleのDrawOwnが呼ばれる

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		scene->Draw();
		//_dbgAddDraw();
		ScreenFlip();
	}

}

Vector2D SceneMng::GetScreenSize(void)
{
	return {ScreenSizeX,ScreenSizeY};
}

bool SceneMng::SysInit(void)
{
	
	SetGraphMode(ScreenSizeX, ScreenSizeY, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)				//DxLibの初期化
	{
		//初期化失敗
		return false;
	}
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	return true;
}
