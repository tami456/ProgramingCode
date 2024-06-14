#include "Grid.h"

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{

	

	for (int x = 1; x < NUM; x++)
	{
		VECTOR sPos = { -HLEN,0.0f,HLEN - TERM * x };
		VECTOR ePos = { HLEN,0.0f,HLEN - TERM * x };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, 20, 10, 0xff0000, 0xff0000, true);

		VECTOR sPos2 = { HLEN - TERM * x,0.0f, -HLEN };
		VECTOR ePos2 = { HLEN - TERM * x ,0.0f, HLEN };
		DrawLine3D(sPos2, ePos2, 0x0000ff);
		DrawSphere3D(ePos2, 20, 10, 0x0000ff, 0x0000ff, true);
	}
	
}

void Grid::Release(void)
{
}
