#pragma once
#include <string>
#include <vector>
#include <map>
#include "Vector2D.h"

#define lpImageMng ImageMng::GetInstance()
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);
	const VecInt& GetID(std::string f_name, std::string key);
	const VecInt& GetID(std::string f_name, std::string key, Vector2D divSize, Vector2D divCnt);
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> ImageMap_;
};

