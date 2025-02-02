#pragma once
#include<DxLib.h>
#include "../Common/Vector2.h"

class PixelMaterial;

class PixelRenderer
{
public:
	// コンストラクタ
	PixelRenderer(PixelMaterial& pixelMaterial);
	~PixelRenderer();

	// 描画矩形の生成
	void MakeSquereVertex(Vector2 pos, Vector2 size);
	void MakeSquereVertex(void);

	// 描画
	void Draw();
	void Draw(int x, int y);

private:
	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

	// 座標
	Vector2 pos_;

	// 描画サイズ
	Vector2 size_;

	// 頂点
	VERTEX2DSHADER vertexs_[NUM_VERTEX];

	// 頂点インデックス
	WORD indexes_[NUM_VERTEX_IDX];

	// ピクセルマテリアル
	PixelMaterial& pixelMaterial_; //※マテリアルの実体生成前設定を
								//強制する目的で参照

	int mainScreen_;
};

