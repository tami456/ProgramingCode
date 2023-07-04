#pragma once
#include "Transition.h"

class CrossOver :
    public Transition
{
public:

    CrossOver(uniquScene beforScene, uniquScene afterScene, int screenSizeX, int screenSizeY);
    ~CrossOver();
private:
    bool UpdateTransition(void)override;
    void DrawOwnScreen(void)override;
    float count_;
    float countA_;
};

