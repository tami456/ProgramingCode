#include <chrono>
#include <math.h>
#include <string>
#include <DxLib.h>
#include "main.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(TRUE);
	SetGraphMode(SCREEN_X, SCREEN_Y, 32, 60);

	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら直ちに終了
		return -1;
	}

	// 初期化
	Init();
	// 実行
	Run();
	// 解放
	Release();
	// ＤＸライブラリの後始末
	DxLib_End();
	// ソフトの終了
	return 0;
}

void Init(void)
{

	// 描画に使用する画像の読み込み
	texDragon = LoadGraph("Data/Dragon.png");

	// 描画に使用する画像の読み込み
	texDragonNor = LoadGraph("Data/Dragon_n.png");

	// 描画に使用する画像の読み込み
	texFire = LoadGraph("Data/Fire.png");

	// 描画に使用する画像の読み込み
	texNoize = LoadGraph("Data/Noise128.png");

	// 描画に使用する画像の読み込み
	texWhite = LoadGraph("Data/White.png");

	//ピクセルシェーダのロード
	//string型からconst chara型に
	psSimpleColor = LoadPixelShader(
		(PATH_SHADER + "SimpleColor.cso").c_str());

	psCustomColor = LoadPixelShader(
		(PATH_SHADER + "CustomColor.cso").c_str());

	//ピクセルシェーダー用の定数バッファを作成
	//sizeof float型を1個用意してね 4の倍数じゃないと渡せない
	psCustomColorConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);

	psCircleColor = LoadPixelShader(
		(PATH_SHADER + "CircleColor.cso").c_str());

	psCircleColorConstBuf = CreateShaderConstantBuffer(sizeof(float) * 4);
}

void Release(void)
{

	// ピクセルシェーダーを解放
	DeleteShader(psSimpleColor);
	DeleteShader(psCustomColor);
	DeleteShader(psCircleColor);

	// ピクセルシェーダー用定数バッファを解放
	DeleteShaderConstantBuffer(psCustomColorConstBuf);
	DeleteShaderConstantBuffer(psCircleColorConstBuf);

}

void Run(void)
{

	// メインスクリーン作成
	mainScreen = MakeScreen(SCREEN_X, SCREEN_Y, true);

	// デルタタイム
	mPreTime = std::chrono::system_clock::now();
	mTotalTime = 0.0f;

	// ESCキーが押されるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		// デルタタイム
		auto nowTime = std::chrono::system_clock::now();
		mDeltaTime = static_cast<float>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - mPreTime).count() / 1000000000.0);
		mPreTime = nowTime;

		// ゲーム起動時間
		mTotalTime += mDeltaTime;

		// メインへ切替
		SetDrawScreen(mainScreen);

		// 画面を初期化
		ClearDrawScreen();

		// 背景色の描画
		DrawBox(0, 0, SCREEN_X, SCREEN_Y, 0x00aaaa, true);

		// 座標の初期化
		mPosX = 10;
		mPosY = 80;
		
		// 通常の描画
		DrawTitle("DrawGraph");
		DrawGraph(mPosX, mPosY, texDragon, true);
		mPosX += PLUS_X;

		//単色
		DrawSimpleColor();
		mPosX += PLUS_X;

		//指定色
		DrawCustomColor();
		mPosX += PLUS_X;

		//円
		DrawCircleColor();
		mPosX += PLUS_X;

		// 裏画面に描画
		SetDrawScreen(DX_SCREEN_BACK);
		// 画面を初期化
		ClearDrawScreen();
		// 描画
		DrawGraph(0, 0, mainScreen, true);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

	}

	// メインスクリーン削除
	DeleteGraph(mainScreen);

}

void MakeSquereVertex(void)
{

	// 毎回頂点データを作成するのは無駄ですが、
	// シェーダー追加時の作業を減らすため、毎フレーム作成

	int cnt = 0;
	float sX = static_cast<float>(mPosX);
	float sY = static_cast<float>(mPosY);
	float eX = static_cast<float>(mPosX + IMG_SIZE_X - 1);
	float eY = static_cast<float>(mPosY + IMG_SIZE_Y - 1);

	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		mVertex[i].rhw = 1.0f;
		mVertex[i].dif = GetColorU8(255, 255, 255, 255);
		mVertex[i].spc = GetColorU8(255, 255, 255, 255);
		mVertex[i].su = 0.0f;
		mVertex[i].sv = 0.0f;
	}

	// 左上
	mVertex[cnt].pos = VGet(sX, sY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// 右上
	mVertex[cnt].pos = VGet(eX, sY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// 右下
	mVertex[cnt].pos = VGet(eX, eY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 1.0f;
	cnt++;

	// 左下
	mVertex[cnt].pos = VGet(sX, eY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 1.0f;

	/*
	　〜〜〜〜〜〜
		0-----1
		|     |
		|     |
		3-----2
	　〜〜〜〜〜〜
		0-----1
		|  ／
		|／
		3
	　〜〜〜〜〜〜
			  1
		   ／ |
		 ／   |
		3-----2
	　〜〜〜〜〜〜
	*/


	// 頂点インデックス
	cnt = 0;
	mIndex[cnt++] = 0;
	mIndex[cnt++] = 1;
	mIndex[cnt++] = 3;

	mIndex[cnt++] = 1;
	mIndex[cnt++] = 2;
	mIndex[cnt++] = 3;

}

/// <summary>
/// 改行
/// </summary>
/// <param name=""></param>
void NewLine(void)
{
	mPosX = 10;
	mPosY += 180;
}

void DrawTitle(std::string title)
{
	DrawString(mPosX, mPosY + TITLE_Y, title.c_str(), 0x000000);
}

void DrawFrame(void)
{
	DrawBox(mPosX - 1, mPosY - 1, mPosX + IMG_SIZE_X, mPosY + IMG_SIZE_Y, 0x000000, false);
}

void DrawSimpleColor(void)
{

	DrawTitle("単色");

	//シェーダーの設定
	SetUsePixelShader(psSimpleColor);

	//四角ポリゴン(三角2つ)を生成
	MakeSquereVertex();

	//描画
	DrawPolygonIndexed2DToShader(mVertex,4,mIndex,2);

	//枠描画
	DrawFrame();
}

void DrawCustomColor(void)
{
	DrawTitle("指定色");

	//シェーダーの設定
	SetUsePixelShader(psCustomColor);

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf = 
		(COLOR_F*)GetBufferShaderConstantBuffer(
			psCustomColorConstBuf);
	//COLOR_F = float*4と一緒
	cbBuf->r = 0.5f;
	cbBuf->g = 0.5f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(psCustomColorConstBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセットする
	//ピクセルシェーダーの0番目にセットしたよ
	SetShaderConstantBuffer(psCustomColorConstBuf, DX_SHADERTYPE_PIXEL, 3);

	//四角ポリゴン(三角2つ)を生成
	MakeSquereVertex();

	//描画
	DrawPolygonIndexed2DToShader(mVertex, 4, mIndex, 2);

	//枠描画
	DrawFrame();
}

void DrawCircleColor(void)
{
	DrawTitle("円形");

	//シェーダーの設定
	SetUsePixelShader(psCircleColor);

	//ピクセルシェーダー用の定数バッファのアドレスを取得
	COLOR_F* cbBuf =
		(COLOR_F*)GetBufferShaderConstantBuffer(
			psCircleColorConstBuf);
	//COLOR_F = float*4と一緒
	cbBuf->r = 0.5f;
	cbBuf->g = 0.5f;
	cbBuf->b = 1.0f;
	cbBuf->a = 1.0f;

	//ピクセルシェーダー用の定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(psCircleColorConstBuf);

	//ピクセルシェーダー用の定数バッファを定数バッファレジスタにセットする
	//ピクセルシェーダーの0番目にセットしたよ
	SetShaderConstantBuffer(psCircleColorConstBuf, DX_SHADERTYPE_PIXEL, 4);

	//四角ポリゴン(三角2つ)を生成
	MakeSquereVertex();

	//描画
	DrawPolygonIndexed2DToShader(mVertex, 4, mIndex, 2);

	//枠描画
	DrawFrame();
}
