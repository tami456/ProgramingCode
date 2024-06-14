#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Grid.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitle_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());

	
}

void TitleScene::Update(void)
{

	// ƒV[ƒ“‘JˆÚ
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TitleScene::Draw(void)
{

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 100,
		1.0f, 0.0, imgTitle_, true);

	grid_->Draw();

	VECTOR pos = { 0.0f,0.0f,0.0f };
	VECTOR pos2 = { 0.0f,0.0f,100.0f };
	DrawSphere3D(pos, 50.0f, 10, 0xffffff, 0xffffff, true);

	DrawSphere3D(pos2, 100.0f, 10, 0xff0000, 0xffffff, true);

	
}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);

	
}
