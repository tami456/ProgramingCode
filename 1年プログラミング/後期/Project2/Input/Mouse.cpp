#include <dxlib.h>
#include "Mouse.h"
#include "../common/Frame.h"

Mouse::Mouse()
{	
	GetMousePoint(&pos_[OldFlame].x, &pos_[OldFlame].y);
}

Mouse::~Mouse()
{
}

void Mouse::Update(BaseScene& scene, const Actor* own)
{
	//memcpy(inputData_[1].data(), inputData_[0].data(), sizeof(inputData_[1]));
	
	GetMousePoint(&pos_[NowFlame].x, &pos_[NowFlame].y);

	//auto btn = GetMouseInput();
	//inputData_[NowFlame][static_cast<int>(InputID::Up)];

	//GetHitKeyStateAll(keyData_);

	//for (auto id : InputID())
	//{
	//	inputData_[0][static_cast<int>(id)] = keyData_[keyMap_[id]];
	//}

}
