#pragma once
#include "Vector2D.h"
class Shape
{
public:
	Shape();
	Shape(int posX,int posY, int sizeX, int sizeY);
	void SetColor(unsigned int color);
	//仮想関数…virtualが付いた関数のこと
	//セミコロンの前に = 0を書くと実体を持たなくなる…純粋仮想関数
	virtual void Draw(void) = 0;		//通常描画
	virtual void Draw(float exp) = 0;	//倍率指定描画

	void Draw(unsigned int color);

protected:
	Vector2D pos_;
	Vector2D size_;
	unsigned int color_;
};

