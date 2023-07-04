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
    bool Init(void);
    uniqueScene Update(uniqueScene ownScene) override;
    void DrawOwnScreen(void) override;
    void Draw();
private:
    int resultImage;
    DrawObj resultObj;
    int blinkCount;
    int sh;
    int vol;
};

