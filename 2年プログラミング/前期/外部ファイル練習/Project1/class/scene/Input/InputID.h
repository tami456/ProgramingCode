#pragma once
#include <type_traits>

enum class InputID
{
	NON,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	BTN1,
	BTN2,
	BTN3,
	MAX
};

static InputID begin(InputID) { return InputID::UP; };
static InputID end(InputID) { return InputID::MAX; };
/////////////////////////////////////////////////////////////////////////////////
static InputID operator++(InputID& state) { return(state = InputID(std::underlying_type<InputID>::type(state) + 1)); };
static InputID operator*(InputID state) { return state; };
