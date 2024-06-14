#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{

}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // レンズの中心座標を定義
    float2 lensCenter = float2(0.5, 0.5);

    // レンズの中心からの距離を計算
    float lensRadius = distance(lensCenter, uv);

    // レンズの歪み係数を設定
    float distortionAmount = 0.4; // 調整可能な値

    // 歪みを適用
    uv = uv + (uv - lensCenter) * lensRadius * distortionAmount;

    // 歪みを適用した座標に対応するテクスチャピクセルを取得
    float4 srcCol = tex.Sample(texSampler, uv);

    return srcCol;
}

