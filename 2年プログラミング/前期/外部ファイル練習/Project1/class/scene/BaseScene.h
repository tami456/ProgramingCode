#pragma once
#include <memory>
#include "../Vector2D.h"
#include "../../class/ImageMng.h"

//クラス宣言
//エクスターン…後から出てくるよという宣言
class BaseScene;
using uniquScene = std::unique_ptr<BaseScene>;
 
class BaseScene
{
public:
	BaseScene(int screenSizeX,int screenSizeY);
	virtual ~BaseScene();
	virtual bool Init(void) = 0;
	//ownScene = 自分自身uniquScene
	virtual uniquScene Update(uniquScene ownScene) = 0;

	//固有の部分とそうじゃない部分で分ける
	virtual void Draw(void);

private:
protected:
	//既定のコンストラクタに要らない
	virtual void DrawOwnScreen(void) = 0;
	int screenID_;
	//constを付けると数値が変えれなくなる
	const Vector2D screenSize_;
	Vector2D drawOffset_;
};

