#pragma once
#include <type_traits>

enum class Dir
{
	Down,
	Left,
	Right,
	Up,
	MAX,
};

static Dir begin(Dir)
{
	return Dir::Down;
}

static Dir end(Dir)
{
	return Dir::MAX;
}

static Dir operator++(Dir& dir)
{
	return (dir = static_cast<Dir>(std::underlying_type<Dir>::type(dir) + 1));
}

static Dir operator*(const Dir& dir)
{
	return dir;
}