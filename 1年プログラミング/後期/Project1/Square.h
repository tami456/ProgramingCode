#pragma once
#include "Shape.h"
class Square :
    public Shape
{
public:
    Square();
    Square(int posX, int posY, int sizeX, int sizeY);
    void Draw(void) override;
    void Draw(float exp)override;
};

