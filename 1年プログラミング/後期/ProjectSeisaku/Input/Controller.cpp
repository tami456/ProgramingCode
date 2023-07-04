#include<dxlib.h>
#include "Controller.h"
#include "../common/Frame.h"

Controller::Controller()
{
	//[0][0]は要素の先頭　先頭の型のサイズを渡せば、そのときの正しいサイズをとってきてくれる
	//そして、その要素数とかけてあげる。
	memset(inputData_[NowFlame].data(), 0xff, inputData_[NowFlame].size() * sizeof(inputData_[NowFlame][0]));
	memset(inputData_[OldFlame].data(), 0xff, inputData_[OldFlame].size() * sizeof(inputData_[OldFlame][0]));

}
Controller::~Controller()
{

}

