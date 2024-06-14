#include<DxLib.h>
#include "GameOverScene.h"
#include"TitleScene.h"
#include"../Input.h"
#include"SceneManager.h"

GameOverScene::GameOverScene(SceneManager& manager):Scene(manager)
{
}
void GameOverScene::Update(Input& input)
{
	if (input.IsTriggered("next"))
	{
		sceneManager_.ChangeScene(std::make_shared<TitleScene>(sceneManager_));
	}
}

void GameOverScene::Draw()
{
	DrawString(50, 50, L"GameOverScene", 0xffffff);
}
