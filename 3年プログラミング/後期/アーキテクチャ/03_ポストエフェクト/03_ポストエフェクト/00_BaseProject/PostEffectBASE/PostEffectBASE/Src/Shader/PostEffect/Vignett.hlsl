#include "../Common/Pixel/PixelShader2DHeader.hlsli"

//定数バッファ：スロット番号3番目（b3）
cbuffer cbParam : register(b4)
{
    float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //UV座標調整
    float2 uv = PSInput.uv;
    //変更されたuv座標をもとに色を取得する
    float4 srcCol =
        tex.Sample(texSampler, uv);

    // 画面中心の座標
    float2 center = float2(0.5, 0.5);
    // ピクセル座標から画面中心への距離を計算
    float distance = length(uv - center);
    // ビネット効果の強度（調整可能）
    float strength = 1.5; // 調整してください
    // ビネット効果の強度を計算
    float vignette = saturate(1.0 - strength * distance);
    // ビネット効果を適用
    srcCol.rgb *= vignette; // 色にビネット効果を適用

    return srcCol;
}