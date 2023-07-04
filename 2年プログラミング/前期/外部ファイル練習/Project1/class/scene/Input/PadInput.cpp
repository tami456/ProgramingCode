#include <DxLib.h>
#include "../../../_debug/_debug/_DebugConOut.h"
#include "../../../_debug/_debug/_DebugDispOut.h"
#include "PadInput.h"

PadInput::PadInput()
{
	inputTbl_ = {
	{InputID::UP,PAD_INPUT_UP},
	{InputID::DOWN,PAD_INPUT_DOWN},
	{InputID::LEFT,PAD_INPUT_LEFT},
	{InputID::RIGHT,PAD_INPUT_RIGHT},
	{InputID::BTN1,PAD_INPUT_1},
	{InputID::BTN2,PAD_INPUT_2},
	{InputID::BTN3,PAD_INPUT_3},
	};

	for (auto tblData : inputTbl_)
	{
		cntData_.emplace(tblData.first, TrgBool{ 0,0 });
	}
	ringBuf_ = RingBuf::Create(20);

}

PadInput::~PadInput()
{
	RingBuf::Destroy(ringBuf_);
}

void PadInput::Update(void)
{
	
	auto inputData = GetJoypadInputState(DX_INPUT_PAD1);
	//‰ö‚µ‚¢//////////////////
	
	for (auto id : inputTbl_)
	{
		
		cntData_[id.first][static_cast<int>(Trg::Old)] = cntData_[id.first][static_cast<int>(Trg::Now)];
		cntData_[id.first][static_cast<int>(Trg::Now)] = ((inputData & id.second) > 0);
	}
	
	//for (auto id : InputID())
	//{
	//	
	//	//cntData_[id][static_cast<int>(Trg::Now)] = padData_[inputData & inputTbl_[id]];
	//}
	WriteCmd();
}
