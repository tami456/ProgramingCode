#include "../Common/Pixel/PixelShader2DHeader.hlsli"

#define AMPLITUDE 0.1
#define SPEED 0.1


Texture2D noiseTex : register(t1); //ノイズテクスチャ

//定数バッファ：スロット番号4番目（b4）
cbuffer cbParam : register(b4)
{
    float g_time;
}

float4 rgbShift(in float2 p, in float4 shift)
{
    shift *= 2.0 * shift.w - 1.0;
    float2 rs = float2(shift.x, -shift.y);
    float2 gs = float2(shift.y, -shift.z);
    float2 bs = float2(shift.z, -shift.x);
    float r = tex.Sample(texSampler, p + rs).x;
    float g = tex.Sample(texSampler, p + gs).y;
    float b = tex.Sample(texSampler, p + bs).z;
    return float4(r, g, b, 1.0);
}

float4 noise(in float2 p)
{
    return noiseTex.Sample(texSampler, p);
}

float4 vec4pow(in float4 v, in float p)
{
    return float4(pow(v.x, p), pow(v.y, p), pow(v.z, p), v.w);
}
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //UV座標調整
    float2 uv = PSInput.uv;

    //変更されたuv座標をもとに色を取得する
    float4 srcCol = tex.Sample(texSampler, uv);

    //タイム
    float time = g_time % 5.0f;

    float4 c = float4(0.0, 0.0, 0.0, 1.0);

    float4 shift = vec4pow(noise(float2(SPEED * time, 2.0f * SPEED * time / 25.0)), 8.0)
            * float4(AMPLITUDE, AMPLITUDE, AMPLITUDE, 4.0f);
    c += rgbShift(uv, shift);
    srcCol = c;

    return srcCol;
}