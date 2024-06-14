#include <DxLib.h>
#include <array>
#include <vector>
#include <string>

using namespace std;

void DrawRectVert(int x, int y, int sx,int sy,int sw,int sh,int handle)
{
	int w, h;
	GetGraphSize(handle, &w, &h);

	//arrayとvectorは配列の中身が
	array<VERTEX2DSHADER, 4> verts;

	//左上
	verts[0].pos = { (float)x,(float)y,0.5f };
	verts[0].u = (float)sx / (float)w;
	verts[0].v = (float)sy / (float)h;

	//右上
	verts[1].pos = { (float)x + sw,(float)y,0.5f };
	verts[1].u = (float)(sx + sw)/ (float)w;
	verts[1].v = (float)sy / (float)h;

	//左下
	verts[2].pos = { (float)x,(float)y + sh,0.5f };
	verts[2].u = (float)sx / (float)w;
	verts[2].v = (float)(sy + sh) / (float)h;

	//右下
	verts[3].pos = { (float)x + sw,(float)y + sh,0.5f };
	verts[3].u = (float)(sx + sw) / (float)w;
	verts[3].v = (float)(sy + sh) / (float)h;

	for (auto& v : verts)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.rhw = 1.0f;
		v.spc = {};
		v.su = 0.0f;
		v.sv = 0.0f;
	}



	DrawPrimitive2DToShader(
		verts.data(),
		verts.size(),
		DX_PRIMTYPE_TRIANGLESTRIP
	);
}

void DrawQuadVert(int x, int y, int handle, int ps)
{
	//arrayとvectorは配列の中身が
	array<VERTEX2DSHADER, 4> verts;
	int w, h;
	GetGraphSize(handle, &w, &h);
	//左上
	verts[0].pos = { (float)x,(float)y,0.5f };
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;

	//右上
	verts[1].pos = { (float)x + w,(float)y,0.5f };
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;

	//左上
	verts[2].pos = { (float)x,(float)y + h,0.5f };
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;

	//右下
	verts[3].pos = { (float)x + w,(float)y + h,0.5f };
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;

	for (auto& v : verts)
	{
		v.dif = GetColorU8(255, 255, 255, 255);
		v.rhw = 1.0f;
		v.spc = {};
		v.su = 0.0f;
		v.sv = 0.0f;
	}

	DrawPrimitive2DToShader(
		verts.data(),
		verts.size(),
		DX_PRIMTYPE_TRIANGLESTRIP
	);
}

void DrawGraphToShader(int x, int y, int handle, int ps)
{

	SetUseTextureToShader(0, handle);
	SetUsePixelShader(ps);
	DrawQuadVert(x,y, handle,ps);

}

void DrawGraphToShaderWithNormal(int x, int y, int handle, int normH,int ps)
{

	SetUseTextureToShader(0, handle);
	SetUseTextureToShader(1, normH);
	SetUsePixelShader(ps);
	DrawQuadVert(x, y, handle, ps);

}

void DrawRectGraphToShaderWithNormal(int x, int y, int sx,int sy,int sw,int sh,int handle, int normH, int ps)
{

	SetUseTextureToShader(0, handle);
	SetUseTextureToShader(1, normH);
	SetUsePixelShader(ps);
	DrawRectVert(x,y,sx,sy,sw,sh,handle);

}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	auto texH = LoadGraph("image/kaeru.png");
	auto normH = LoadGraph("image/kaeru_n.png");
	auto psH = LoadPixelShader("PixelShader.pso");
	
	int RT0 = MakeScreen(640,480,true);
	int RT1 = MakeScreen(640,480,true);
	
	int gause0 = MakeScreen(640, 480, true);
	int gause1 = MakeScreen(640, 480, true);
	int shrink0 = MakeScreen(640, 480, true);

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		SetRenderTargetToShader(0, RT0);
		SetRenderTargetToShader(1, RT1);
		DrawGraphToShaderWithNormal(50,50,texH, normH,psH);
		SetRenderTargetToShader(0, -1);
		SetRenderTargetToShader(1, -1);
		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, RT0,true);
		GraphFilterBlt(RT1, gause0, DX_GRAPH_FILTER_GAUSS, 32, 1400);
		GraphFilterBlt(RT1, shrink0, DX_GRAPH_FILTER_DOWN_SCALE,4);
		GraphFilter(shrink0, DX_GRAPH_FILTER_GAUSS, 8, 400);

		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, RT1,true);
		DrawGraph(0, 0, gause0,true);

		DrawExtendGraph(0, 0,640,480, shrink0,true);
		DrawExtendGraph(0, 0,640,480, shrink0,true);
		DrawExtendGraph(0, 0,640,480, shrink0,true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}