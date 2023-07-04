#include <dxLib.h>
#include "Bg.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
Bg::Bg()
{
}

Bg::~Bg()
{
}

void Bg::Init()
{
	bgImage = LoadGraph("./data/image/bg.png");
	
}

void Bg::Update()
{

	

}

void Bg::Draw()
{
	DrawGraph(0, bgPosY, bgImage, true);
}
