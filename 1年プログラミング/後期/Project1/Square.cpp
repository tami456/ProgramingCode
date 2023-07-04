#include<DxLib.h>
#include "Square.h"

Square::Square()
{

}

Square::Square(int posX, int posY, int sizeX, int sizeY):Shape(posX,posY,sizeX,sizeY)
{
}


void Square::Draw(void)
{
	DrawBox(pos_.x, pos_.y,
		pos_.x + size_.x - 1, pos_.y + size_.y - 1,
		color_, true);
}

void Square::Draw(float exp)
{
	Vector2D expSize = size_ * exp;
	Vector2D offset = (expSize - size_) / 2;

	DrawBox(pos_.x - offset.x, pos_.y - offset.y,
		pos_.x + size_.x + offset.x - 1, pos_.y + size_.y + offset.y - 1,
		color_, true);
}
