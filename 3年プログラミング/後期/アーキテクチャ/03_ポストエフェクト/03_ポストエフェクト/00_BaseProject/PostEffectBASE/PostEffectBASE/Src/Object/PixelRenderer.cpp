#include "PixelMaterial.h"
#include "PixelRenderer.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"

PixelRenderer::PixelRenderer(PixelMaterial& pixelMaterial):pixelMaterial_(pixelMaterial)
{
	MakeSquereVertex();
	mainScreen_ = SceneManager::GetInstance().GetMainScreen();
}

PixelRenderer::~PixelRenderer()
{
}

void PixelRenderer::MakeSquereVertex(Vector2 pos, Vector2 size)
{

}

//GameSceneのInitでやってたポリゴンの生成
void PixelRenderer::MakeSquereVertex(void)
{

	//画面いっぱいの2枚のポリゴン
	float sX = static_cast<float>(0.0f);
	float sY = static_cast<float>(0.0f);
	float eX = static_cast<float>(Application::SCREEN_SIZE_X);
	float eY = static_cast<float>(Application::SCREEN_SIZE_Y);

	// ４頂点の初期化
	for (int i = 0; i < 4; i++)
	{
		vertexs_[i].rhw = 1.0f;
		vertexs_[i].dif = GetColorU8(255, 255, 255, 255);
		vertexs_[i].spc = GetColorU8(255, 255, 255, 255);
		vertexs_[i].su = 0.0f;
		vertexs_[i].sv = 0.0f;
	}

	// 左上
	vertexs_[0].pos = VGet(sX, sY, 0.0f);
	vertexs_[0].u = 0.0f;
	vertexs_[0].v = 0.0f;

	// 右上
	vertexs_[1].pos = VGet(eX, sY, 0.0f);
	vertexs_[1].u = 1.0f;
	vertexs_[1].v = 0.0f;

	// 右下
	vertexs_[2].pos = VGet(eX, eY, 0.0f);
	vertexs_[2].u = 1.0f;
	vertexs_[2].v = 1.0f;

	// 左下
	vertexs_[3].pos = VGet(sX, eY, 0.0f);
	vertexs_[3].u = 0.0f;
	vertexs_[3].v = 1.0f;

	indexes_[0] = 0;
	indexes_[1] = 1;
	indexes_[2] = 3;

	indexes_[3] = 1;
	indexes_[4] = 2;
	indexes_[5] = 3;
}

//GameSceneのDrawでやってたやつ
void PixelRenderer::Draw()
{
	auto postEffectScreen = pixelMaterial_.GetPostEScreen();
	auto shader = pixelMaterial_.GetMShader();
	auto shaderConstBuf = pixelMaterial_.GetMConstBuf();
	
	// ポストエフェクト
	//-----------------------------------------
	SetDrawScreen(postEffectScreen);

	// 画面を初期化
	ClearDrawScreen();

	// シェーダー設定
	SetUsePixelShader(shader);

	SetUseTextureToShader(0, mainScreen_);

	int index = 1;
	for (const auto& mScreen : pixelMaterial_.GetTextures())
	{
		// テクスチャの設定
		SetUseTextureToShader(index, mScreen);
		index++;
	}

	// 定数バッファ
	FLOAT4* constBufsPtr =
		(FLOAT4*)GetBufferShaderConstantBuffer(shaderConstBuf);

	for (const auto& constBufs : pixelMaterial_.GetMConstBufs())
	{
		// 乗算色
		constBufsPtr->x = constBufs.x;
		constBufsPtr->y = constBufs.y;
		constBufsPtr->z = constBufs.z;
		constBufsPtr->w = constBufs.w;
		constBufsPtr++;
	}
	

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(shaderConstBuf);

	// 定数バッファをピクセルシェーダー用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		shaderConstBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// 描画
	DrawPolygonIndexed2DToShader(vertexs_, NUM_VERTEX, indexes_, NUM_POLYGON);

	// メインに戻す
	SetDrawScreen(mainScreen_);
	DrawGraph(0, 0, postEffectScreen, false);
	//-----------------------------------------
}

void PixelRenderer::Draw(int x, int y)
{
}
