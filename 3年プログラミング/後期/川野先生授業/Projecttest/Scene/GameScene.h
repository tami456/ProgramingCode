#pragma once
#include "Scene.h"
#include <vector>

class GameScene :
    public Scene
{
private:
    float fps_ = 0.0f;
    std::vector<int> imgHs_;
    int frame_ = 0;
    int nowLoadingCnt = 0;
    using UpdateFunc_t = void (GameScene::*)(Input& input);
    using DrawFunc_t = void(GameScene::*)();

    void InitializeUpdate(Input& input);
    void InitializeDraw();

    void LoadingUpdate(Input& input);
    void LoadingDraw();

    void LoadingCompletedUpdate(Input& input);

    void NormalUpdate(Input& input);
    void NormalDraw();

    UpdateFunc_t updateFunc_;
    DrawFunc_t drawFunc_;

public:
    GameScene(SceneManager& manager);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
    
};

