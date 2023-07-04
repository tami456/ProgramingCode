#pragma once
#include "../Vector2D.h"
#include "Collision.h"

struct Raycast
{
	struct Ray 
	{
		Vector2DDouble p;	//始点
		Vector2DDouble vec;	//方向ベクトル
	};

	struct Line 
	{
		Vector2DDouble p;	//始点
		Vector2DDouble end;	//終点
	};
	bool CheckCollision(Ray ray, Collision collision);
	bool CheckRay(Ray ray, Line line);
};

