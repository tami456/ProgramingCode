#pragma once
#include <memory>
#include "../common/Vector2D.h"

class Controller;
class BaseScene;		//クラス宣言　今後登場予定のクラスを宣言する
//std::unique_ptr<BaseScene>をuniqueSceneという名前で使う
using uniqueScene = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	bool Init(void);
	virtual uniqueScene Update(uniqueScene ownScene) = 0;
	virtual void DrawOwnScreen(void) = 0;
	virtual void Draw(void);				//スクリーンに描画するだけなので、ここだけでいい
	//拡張しやすいようにvirtualをつける
private:
	Vector2D screenSize_;
protected:
	Vector2D drawOffset_;
	int screenID_;
	std::unique_ptr<Controller> controller_;
};

