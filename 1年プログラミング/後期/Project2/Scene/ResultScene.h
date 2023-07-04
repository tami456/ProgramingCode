#pragma once
#include "BaseScene.h"
#include "../common/Vector2D.h"
#include "../common/ImageCtl.h"

class ResultScene :
    public BaseScene
{
public:
    ResultScene();
    ~ResultScene();
    void Init(void);
    uniqueScene Update(uniqueScene ownScene) override;
    void DrawOwnScreen(void) override;
private:
    int resultImage;
};

