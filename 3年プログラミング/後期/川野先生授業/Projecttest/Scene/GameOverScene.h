#pragma once
#include "Scene.h"
class GameOverScene :
    public Scene
{
public:
    GameOverScene(SceneManager& manager);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

