#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/ParticleGenerator.h"
#include "../Object/Common/TextScroll.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	mImgTitleLogo = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	//追従するものはないのでヌル
	mSpaceDome = new SpaceDome(nullptr);
	mSpaceDome->Init();
	// タイトルロゴ
	mImgTitleLogo = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).mHandleId;

	//カメラモード(定点カメラ)
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	mParticleGenerator = new ParticleGenerator({0.0f,1.0f,0.0f},20.0f);
	mParticleGenerator->Init();

	mTextScroll = new TextScroll();
	mTextScroll->Init();
}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	mSpaceDome->Update();
	mParticleGenerator->Update();
	mTextScroll->Update();
}

void TitleScene::Draw(void)
{
	mSpaceDome->Draw();
	mParticleGenerator->Draw();
	mTextScroll->Draw();
	// ロゴ描画
	DrawLogo();

}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, mImgTitleLogo, true);

	// Pushメッセージ
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}

void TitleScene::Release(void)
{

	DeleteGraph(mImgTitleLogo);
	delete mSpaceDome;

	mParticleGenerator->Release();
	delete mParticleGenerator;

	mTextScroll->Release();
	delete mTextScroll;
}
