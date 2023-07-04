#include <DxLib.h>
#include "Shape.h"


Shape::Shape()
{
	for (int j = 0; j < 2; j++)
	{
		pos_[j] = j;
	}
	SetColor(0x00ff00);
}

Shape::Shape(int posX, int posY, int sizeX, int sizeY)
{
	pos_ = Vector2D( posX,posY );
	pos_.x = posX;
	pos_.y = posY;
	size_.x = sizeX;
	size_.y = sizeY;
	SetColor(0x00ff00);
}

void Shape::SetColor(unsigned int color)
{
	color_ = color;
}

//void Shape::Draw(void)
//{
//	DrawBox(pos_.x,pos_.y,
//		pos_.x + size_.x - 1,pos_.y + size_.y - 1,
//		0x00ff00, true);
//}



void Shape::Draw(unsigned int color)
{

	DrawBox(pos_.x, pos_.y,
		pos_.x + size_.x - 1, pos_.y + size_.y - 1,
		color, true);
}
