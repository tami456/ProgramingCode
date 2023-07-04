#pragma once
#include <map>
#include <array>
#include "InputID.h"

class BaseScene;
class Actor;

using InputData = std::array<int, static_cast<int>(InputID::Max)>;	//整数値をMaxの数だけ入れる
using InputDatas = std::array<InputData, 2>;


class Controller		//インターフェースを用意するクラス
{
public:
	Controller();
	~Controller();
	virtual void Update(BaseScene& scene,const Actor* own = nullptr) = 0;
	const InputDatas& GetInputData(void) { return inputData_; }
protected:
	std::map<InputID, int> keyMap_;		//keyData_の中に何を入れるかを格納
	InputDatas inputData_;	//一次元目：現在の値　二次元目：１フレーム前の値
};

