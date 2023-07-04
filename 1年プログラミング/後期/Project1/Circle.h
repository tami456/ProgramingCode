#pragma once
#include "Shape.h"
class Circle :
    public Shape
{
public:
    Circle();
    Circle(int posX, int posY, int sizeX, int sizeY);
    void Draw(void) override;
    void Draw(float exp)override;
};

