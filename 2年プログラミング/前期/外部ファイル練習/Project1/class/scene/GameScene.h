#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "TmxObj.h"
#include "Obj/Obj.h"


class GameScene :
    public BaseScene
{
public:
	GameScene(int screenSizeX, int screenSizeY);
	~GameScene();
private:
	bool Init(void) override;
	uniquScene Update(uniquScene ownScene) override;
	void DrawOwnScreen(void)override;
	TmxObj tmxObj_;

	//生のポインタと比べても速度は変わらない
	//shared_ptrは若干遅い
	//unique_ptrで済むところはunique_ptrで
	ObjList objList_;

	//データをドン！と置きたいときstd::vector
	//std::listを使うメリット：要素が入っている箱、その箱を紐づけて連携させる
	//要素を変えたいときmoveを使わなくて済む(紐づけを切ればよい)
	//デメリット：ランダムアクセスができない前からか後ろからか途中からアクセスできない
	//対処法：Playerが先に来るようにソートすればよい
};

