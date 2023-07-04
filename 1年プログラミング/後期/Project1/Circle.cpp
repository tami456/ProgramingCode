#include "Circle.h"
#include <DxLib.h>
Circle::Circle()
{
}

Circle::Circle(int posX, int posY, int sizeX, int sizeY)
{
}

void Circle::Draw(void)
{
	DrawCircle(50,pos_.x, pos_.y,color_, true);
}

void Circle::Draw(float exp)
{
	Vector2D expSize = size_ * exp;
	Vector2D offset = (expSize - size_) / 2;

	DrawCircle(50,pos_.x - offset.x, pos_.y - offset.y,color_, true);
}
