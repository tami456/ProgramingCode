#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	float2 uv = PSInput.uv;

	//UV座標とテクスチャを参照して、最適な色を取得する
	float4 srcCol = tex.Sample(texSampler,PSInput.uv);

	if (srcCol.a < 0.01f)
	{
		//描画しない(アルファテスト)
		discard;
	}
	//間隔を作る
	float area = sin(uv.y * 2.0f - g_time * 0.5f);

	//sinカーブを均一化された特定範囲とするために、
	//step関数を使用して、値を1 or 0 にする。
	float isArea = step(0.996f, area * area);

	//特定範囲だけ、色加算を行い明るくする。
	srcCol.rgb += isArea * 0.3f;

	//このままだと、特定範囲にも下地の縞模様が入ってしまうので、消すようにする。
	srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
	srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

	return srcCol;
}