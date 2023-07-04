#pragma once
#include <type_traits>

enum class InputID
{
	Up,
	Down,
	Left,
	Right,
	Btn1,
	Btn2,
	Btn3,
	Max
};

//enumクラスのオペレーターのオーバーロード

static InputID begin(InputID)
{
	return InputID::Up;
}
static InputID end(InputID)
{
	return InputID::Max;
}

static InputID operator* (InputID& id)
{
	return id;		//実体を返す
}

static InputID operator++ (InputID& id)	//渡したオブジェクトを変えたいので、constはいらない
{
	id = static_cast<InputID>(std::underlying_type<InputID>::type(id) + 1);	//クラスの既定の型(この場合はid)を取得する
	return id;		//数値を++して返す
}