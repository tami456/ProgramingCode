#include <dxlib.h>
#include "SceneMng.h"
#include "../../_debug/_DebugDispOut.h"
#include "../../_debug/_DebugConOut.h"


#define SCREEN_SIZE_X 800	//画面の横サイズ
#define SCREEN_SIZE_Y 600	//画面の縦サイズ

SceneMng::SceneMng()
{
	TRACE("SceneMngの生成\n");
	Init();
}

SceneMng::~SceneMng()
{
	TRACE("SceneMngの破棄\n");
	Release();
}

void SceneMng::Run(void)
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
		if (scnID_ != nextScnID_)
		{
			SetupScene();
		}
		scnID_ = nextScnID_;
		
		Update();
		Draw();
	}
}

void SceneMng::SetupScene(void)
{
	ReleaseScene();
	switch (nextScnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Init();
		break;
	case SCN_ID::SELECT:
		selectScene_.Init();
		break;
	case SCN_ID::GAME:
		gameScene_.Init(selectScene_.GetSelPlayer(0),selectScene_.GetSelPlayer(1));
		break;
	case SCN_ID::RESULT:
		resultScene_.Init(gameScene_.GetWinner());
		break;
	default:
		TRACE("未知のSCN_ID:%d", scnID_);
		break;
	}
}

bool SceneMng::Init(void)
{
	TRACE("SceneMngのInitの呼び出し\n");
	if (SysInit())
	{
		scnID_ = SCN_ID::TITLE;
		nextScnID_ = SCN_ID::TITLE;
		return titleScene_.Init();
		TRACE("出来てんで\n");
	}
	return false;
}


bool SceneMng::SysInit(void)
{
	TRACE("SceneMngのSysInitの呼び出し\n");
	SetWindowText("2116020_水田渚");

	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return false;
	}
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	
	return true;
}



bool SceneMng::Release(void)
{
	//TRACE("SceneMngのReleaseの呼び出し\n");
	DxLib_End();
	return true;
}

void SceneMng::Update(void)
{
	/*for (int no = 0; no < 256; no++)
	{
		keyBufOld_[no] = keyBuf_[no];
	}*/
	//sizeof(keyBufOld_) / sizeof(keyBufOld_[0]);

	memcpy(keyBufOld_,keyBuf_,sizeof(keyBufOld_));
	//TRACE("SceneMngのUpdateの呼び出し\n");
	
	GetHitKeyStateAll(&keyBuf_[0]);	//変数のポインターを渡す
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		nextScnID_ = titleScene_.Update(keyBuf_,keyBufOld_);
		break;
	case SCN_ID::SELECT:
		nextScnID_ = selectScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GAME:
		nextScnID_ = gameScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::RESULT:
		nextScnID_ = resultScene_.Update(keyBuf_, keyBufOld_);
		break;
	default:
		TRACE("未知のSCN_ID:%d",scnID_);
			break;
	}

}

void SceneMng::Draw(void)
{
	//TRACE("SceneMngのDrawの呼び出し\n");
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Draw();
		break;
	case SCN_ID::SELECT:
		selectScene_.Draw();
		break;
	case SCN_ID::GAME:
		gameScene_.Draw();
		break;
	case SCN_ID::RESULT:
		resultScene_.Draw();
		break;
	default:
		TRACE("未知のSCN_ID:%d", scnID_);
		break;
	}
	ScreenFlip();
}
void SceneMng::ReleaseScene(void)
{
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Release();
		break;
	case SCN_ID::SELECT:
		selectScene_.Release();
		break;
	case SCN_ID::GAME:
		gameScene_.Release();
		break;
	case SCN_ID::RESULT:
		resultScene_.Release();
		break;
	default:
		TRACE("未知のSCN_ID:%d", scnID_);
		break;
	}

}

