#include <DxLib.h>
#include "KeyInput.h"

KeyInput::KeyInput()
{
	//InputIDに格納
	inputTbl_ = {
		{InputID::UP,KEY_INPUT_UP},
		{InputID::DOWN,KEY_INPUT_DOWN},
		{InputID::LEFT,KEY_INPUT_LEFT},
		{InputID::RIGHT,KEY_INPUT_RIGHT},
		{InputID::BTN1,KEY_INPUT_Z},
		{InputID::BTN2,KEY_INPUT_X},
		{InputID::BTN3,KEY_INPUT_HOME},
	};

	for (auto tblData : inputTbl_)
	{
		cntData_.emplace(tblData.first, TrgBool{ 0,0 });
	}
	ringBuf_ = RingBuf::Create(20);
}

KeyInput::~KeyInput()
{
	RingBuf::Destroy(ringBuf_);
}

void KeyInput::Update(void)
{
	//arrayのdataで生のデータにアクセスできる
	GetHitKeyStateAll(keyData_.data());
	for (auto id : InputID())
	{
		cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
		//アクセスしてキーが押されているか　該当キーをNowに格納
		cntData_[id][static_cast<int>(Trg::Now)] = keyData_[inputTbl_[id]];
	}
	WriteCmd();
}
