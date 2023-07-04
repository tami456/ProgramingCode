#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	mImgPush = -1;
	mImgTitle = -1;
	mSkyDome = nullptr;
	mAnimationController = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	GravityManager::GetInstance().Init();

	ResourceManager& rm = ResourceManager::GetInstance();

	// ‰æ‘œ“Ç‚Ýž‚Ý
	mImgTitle = rm.Load(ResourceManager::SRC::TITLE).mHandleId;
	mImgPush = rm.Load(ResourceManager::SRC::PUSH_SPACE).mHandleId;

	// ”wŒi
	mSkyDome = new SkyDome(nullptr);
	mSkyDome->Init();

	float size;

	// ƒƒCƒ“˜f¯
	mPlanet.SetModel(rm.LoadModelDuplicate(ResourceManager::SRC::FALL_PLANET));
	mPlanet.pos = AsoUtility::VECTOR_ZERO;
	mPlanet.scl = AsoUtility::VECTOR_ONE;
	mPlanet.Update();

	// ‰ñ“]‚·‚é˜f¯
	mMovePlanet.SetModel(rm.LoadModelDuplicate(ResourceManager::SRC::LAST_PLANET));
	mMovePlanet.pos = { -250.0f, -100.0f, -100.0f };
	size = 0.7f;
	mMovePlanet.scl = { size, size, size };
	mMovePlanet.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mMovePlanet.Update();

	// ƒLƒƒƒ‰
	mChara.SetModel(rm.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	mChara.pos = { -250.0f, -32.0f, -105.0f };
	size = 0.4f;
	mChara.scl = { size, size, size };
	mChara.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(90.0f), 0.0f);
	mChara.Update();

	// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌÝ’è
	std::string path = Application::PATH_MODEL + "Player/";
	mAnimationController = new AnimationController(mChara.modelId);
	mAnimationController->Add(0, path + "Run.mv1", 20.0f);
	mAnimationController->Play(0);

	// ’è“_ƒJƒƒ‰
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

}

void TitleScene::Update(void)
{

	// ƒV[ƒ“‘JˆÚ
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// ˜f¯‚Ì‰ñ“]
	mMovePlanet.quaRot = mMovePlanet.quaRot.Mult(
		Quaternion::Euler(0.0f, 0.0f, AsoUtility::Deg2RadF(1.0f)));
	mMovePlanet.Update();

	// ƒLƒƒƒ‰ƒAƒjƒ[ƒVƒ‡ƒ“
	mAnimationController->Update();

	mSkyDome->Update();

}

void TitleScene::Draw(void)
{

	mSkyDome->Draw();

	MV1DrawModel(mPlanet.modelId);
	MV1DrawModel(mMovePlanet.modelId);
	MV1DrawModel(mChara.modelId);

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 1.0, 0.0, mImgTitle, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, mImgPush, true);

}

void TitleScene::Release(void)
{

	mSkyDome->Release();
	delete mSkyDome;

	mAnimationController->Release();
	delete mAnimationController;

}
