#include <DxLib.h>
#include "../../_debug/_debug/_DebugConOut.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "BaseScene.h"
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"

//よくわからない数字が入っている状態が良くないから初期化するためnullをいれる
//よくわからない数字 = 不定値
constexpr int ScreenSizeX = 1024;
constexpr int ScreenSizeY = 768;

void SceneMng::Run(void)
{
	//SysInitが失敗していた場合
	if (!SysInit())
	{
		return;
	}
	//あとからインスタンスする
	//unique_ptrの場合
	//どちらも重要
	//scene_.reset(new BaseScene(ScreenSizeX,ScreenSizeY));
	//C++の11以降の機能
	//アンドロイド開発ではmake_uniqueがないので上のresetの方がいい
	scene_ = std::make_unique<TitleScene>(ScreenSizeX, ScreenSizeY);

	//プログラミングは人に例えられそう

	/*SetDrawScreen(DX_SCREEN_BACK);*/
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
  		Update();
		Draw();
		//SetDrawScreen(DX_SCREEN_BACK);
		//ClsDrawScreen();
		//
		////_dbgAddDraw();
		//ScreenFlip();
	}
}

void SceneMng::Update(void)
{
	_dbgStartDraw();
	//ガード処理
	if (!scene_)
	{
		TRACE("【Error】scene_の中身が空\n");
		return;
	}
	//*scene_.Update　エラーが出る。.はアドレス同士を足す演算
	//*は.より優先度が低い*scene_ .Updateではなく * scene_.Updateになる
	//四則演算子と同じ
	//uniqu_ptrはコピーできない
	//std::move = 中身の所有権を他の人に渡すことはできる
	//左辺値を右辺値に変換

	//シンタックス=ルール、文法、書き方
	scene_ = scene_->Update(std::move(scene_));
}

void SceneMng::Draw(void)
{
	if (!scene_)
	{
		TRACE("【Error】scene_の中身が空\n");
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	scene_->Draw();
	_dbgDraw();
	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	SetGraphMode(ScreenSizeX, ScreenSizeY, 32);
	ChangeWindowMode(true);
	SetWindowText("2116020 水田渚");
	
	if (DxLib_Init() == -1)
	{
		//初期化失敗
		return false;
	}
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	return true;
}

SceneMng::SceneMng()
{
}

SceneMng::~SceneMng()
{
}
