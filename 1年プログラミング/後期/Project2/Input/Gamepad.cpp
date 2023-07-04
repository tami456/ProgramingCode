#include <dxlib.h>
#include "Gamepad.h"
#include "../common/Frame.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Gamepad::Gamepad()
{	//キーボードの設定
	keyMap_.try_emplace(InputID::Up, PAD_INPUT_UP);			//キーがないときに、キーを作る
	keyMap_.try_emplace(InputID::Down, PAD_INPUT_DOWN);
	keyMap_.try_emplace(InputID::Left, PAD_INPUT_LEFT);
	keyMap_.try_emplace(InputID::Right, PAD_INPUT_RIGHT);
	keyMap_.try_emplace(InputID::Btn1, PAD_INPUT_1);
	keyMap_.try_emplace(InputID::Btn2, PAD_INPUT_2);
	keyMap_.try_emplace(InputID::Btn3, PAD_INPUT_START);
}

Gamepad::~Gamepad()
{
}

void Gamepad::Update(BaseScene& scene, const Actor* own)
{
	memcpy(inputData_[OldFlame].data(), inputData_[NowFlame].data(), sizeof(inputData_[OldFlame][NowFlame]) * inputData_[OldFlame].size());
	auto padData = GetJoypadInputState(DX_INPUT_PAD1);
	
	//if((padData & keyMap_[InputID::Up])!= 0)		//上キーが押されていたら、

	//sizeof→()の中のサイズを取る

	//GetHitKeyStateAll(keyData_);

	for (auto id : InputID())
	{
		//(padData & keyMap_[InputID::Up]) > 0

		inputData_[0][static_cast<int>(id)] = (padData & keyMap_[id]);		//>で出た結果は0か1が入る
	}

}
