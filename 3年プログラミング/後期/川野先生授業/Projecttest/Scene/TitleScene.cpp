#include<DxLib.h>
#include "TitleScene.h"
#include"GameScene.h"
#include"KeyconfigScene.h"
#include"../Input.h"
#include"SceneManager.h"
#include"../StringUtility.h"

//コンストラクタの初期化リストには親クラスの引数付きコンストラクタじゃないとダメ
//sceneManager_(manager)←ダメ
TitleScene::TitleScene(SceneManager& manager):Scene(manager)
{
	auto str = StringUtility::WstringToString(L"StringTest In TitleScene\n");
	::OutputDebugStringA(str.c_str());
}

void TitleScene::Update(Input& input)
{
	if (input.IsTriggered("next"))
	{
		sceneManager_.ChangeScene(std::make_shared<GameScene>(sceneManager_));
	}
	else if (input.IsTriggered("keyconfig"))
	{
		sceneManager_.PushScene(std::make_shared <KeyconfigScene>(sceneManager_));
	}
}

void TitleScene::Draw()
{
	std::string str = "Title Scene MultiByte";
	std::wstring wstr = L"Title Scene MultiByte";
	DrawString(50, 50, L"TitleScene", 0xffffff);
}
