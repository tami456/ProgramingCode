#include "TitleScene.h"
#include "GameScene.h"
#include <DxLib.h>
#include "TmxObj.h"
#include "Obj/Player.h"
#include "Obj/Hadouken.h"
#include "Obj/SonicBoom.h"
#include "Obj/StateMng.h"
#include "transition/CrossOver.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "../../_debug/_debug/_DebugConOut.h"


//デフォルトコンストラクタがないからエラーが出る
//コンストラクタの後にBaseSceneを書くことによってエラーは無くなる
GameScene::GameScene(int screenSizeX, int screenSizeY):BaseScene(screenSizeX,screenSizeY)
{
	TRACE("GameSceneの生成\n");
	Init();
	DrawOwnScreen();
	
}

GameScene::~GameScene()
{
	TRACE("GameSceneの破棄\n");
	
}

bool GameScene::Init(void)
{
	StateMng::Create();
	lpStateMng.Load("./data/Player.xml", "Player");
	lpStateMng.Load("./data/Hadouken.xml","Hadouken");
	lpStateMng.Load("./data/SonicBoom.xml", "SonicBoom");
	tmxObj_.LoadTMX("./tmx/map.tmx");
	//現在ある要素の後ろに追加していく
	//イテレーター：コンテナにアクセスするポインタみたいなやつ
	//行き来する
	
	//obj.hにtmxObj_の参照ある
	objList_.emplace_back(std::make_unique<Player>(tmxObj_,objList_));
	
	
	return true;
}

uniquScene GameScene::Update(uniquScene ownScene)
{
	//何を取り出しているのかわからない場合autoを使った方がいいかも
	/*std::list<int> intList;
	auto num = intList.begin();*/
	//後ろ演算は少し遅い
	//後ろ演算は5行も書かれている
	//比べて前演算は2行で済んでいる
	//イテレーターを使うときは気を付けた方がいい
	//イテレーターに対して><は使えない
	for (auto obj = objList_.begin(); obj != objList_.end(); ++obj)
	{
		//()を付けて優先度を上げる
		(*obj)->Update();
	}

	//検索と削除をまとめてやってくれる
	//ラムダでおけ
	//死んでいるかどうかの確認
	//生きているのを確認したい場合はisAliveでもいい
	objList_.remove_if([](UniqueObj& obj) {return obj->isDeth(); });

	DrawOwnScreen();
	//if (CheckHitKey(KEY_INPUT_SPACE))
	//{
	//	//ここら辺の説明はTitleSceneに書いてある
	//	return std::make_unique<TitleScene>(screenSize_.x, screenSize_.y);
	//}
	
	return ownScene;
}

void GameScene::DrawOwnScreen(void)
{

	SetDrawScreen(screenID_);
	ClsDrawScreen();

	auto worldArea_ = tmxObj_.GetWorldArea();
	auto tileSize_ = tmxObj_.GetTileSize();
	
	
	for (const auto& layer: tmxObj_.GetMapData())
	{
		for (int y = 0; y < worldArea_.y; y++)
		{
			for (int x = 0; x < worldArea_.x; x++)
			{

				//auto mapData = tmxObj_.GetMapData().at("block")[x + y * worldArea_.x];
				//constを書かないとconstが外れてしまう

				//0が返ってきたらアクセスしない
				//1だったらアクセスする
				if (x + y * worldArea_.x < layer.second.size())
				{
					//gidはあくまでidなので画像ではない
					auto gid = layer.second[x + y * worldArea_.x];
					if (gid >= 0)
					{
						//TMXの方で返り値を作った方がいいかも
						DrawGraph(
							x * tileSize_.x,
							y * tileSize_.y,
							lpImageMng.GetID(tmxObj_.GetMapKey())[gid],
							true);
					}
				}
				
			}
		}
	}
	//元の場所も一緒に変えないといけないから参照渡し
	for (auto& obj: objList_)
	{
		obj->Draw();
	}
}
