#include "../Common/Pixel/PixelShader2DHeader.hlsli"

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_time;
    float2 screen_size;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 時間経過に応じてぼかしの強さを増加
    float blurStrength = 0.5 * g_time;

    //float blurStrength = 0.1 * sin(g_time);
    blurStrength = min(blurStrength, 3);
    float2 texelSize = 1.0 / screen_size;
    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    //UV座標調整
    float2 uv = PSInput.uv;
    float4 srcCol =
        tex.Sample(texSampler, uv);

    // ぼかしのカーネルサイズを計算
     int kernelSize = int(1 + blurStrength * 3);

     for (int i = -kernelSize; i <= kernelSize; ++i)
     {
         // ピクセルのサンプリング
         float2 offset = i * texelSize;
         color += tex.Sample(texSampler, uv + offset);
     }

     // カーネル内のピクセルの平均化
     color /= 2 * kernelSize + 1;

     return color;
}