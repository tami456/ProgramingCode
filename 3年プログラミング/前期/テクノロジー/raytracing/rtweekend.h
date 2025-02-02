#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

//using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//定数
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//ユーティリティ関数
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180;
}

inline double random_double()
{
	//[0,1)の実数乱数を返す
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	//[min,max)の実数乱数を返す
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
	if (x < min)return min;
	if (x > max)return max;
}

//共通ヘッダー
#include "ray.h"
#include "vec3.h"

#endif
