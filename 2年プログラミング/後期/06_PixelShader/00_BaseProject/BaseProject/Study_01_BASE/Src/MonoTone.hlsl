struct PS_INPUT
{
    // 座標( プロジェクション空間 )
    float4 Position        : SV_POSITION;
    // ディフューズカラー
    float4 Diffuse         : COLOR0;
    // テクスチャ座標
    float2 TexCoords0      : TEXCOORD0;
};

//定数バッファ：スロット0番目(スロットの0番目)
cbuffer cbParam : register(b6)
{
    //グローバル変数だからどこでも使える
    float4 g_color;
}

// 描画するテクスチャ
Texture2D g_SrcTexture : register(t0);

// サンプラー：適切な色を決める処理
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{

    //UV座標を取得する
    float2 uv = PSInput.TexCoords0;

    //UV座標とテクスチャを参照して、最適な色を取得する
    float4 srcCol =
        g_SrcTexture.Sample(g_SrcSampler, PSInput.TexCoords0);

    float4 dstCol = srcCol;

    //①3色の平均値をとる
    //3色分を足す(1.0f+1.0f+1.0f)/3.0fで平均が取れる
    //float gray = (srcCol.r + srcCol.g + srcCol.b) / 3.0f;

    //②ITU-R Rec BT.601
    //float gray = dot(dstCol.rgb, float3(0.299, 0.587, 0.114));
    //dstCol.rgb = float3(gray, gray, gray);

    //③シンプルセピア
    //float gray = dot(dstCol.rgb, float3(0.299, 0.587, 0.114));
    //dstCol.rgb = float3(gray, gray, gray);
    //dstCol.rgb *= float3(1.07f, 0.74f, 0.43f);

    //④調整付きセピア
    float3 sepia = dstCol.rgb;
    sepia.r = dot(dstCol.rgb, float3(0.393f, 0.769f, 0.189f));
    sepia.g = dot(dstCol.rgb, float3(0.349f, 0.686f, 0.168f));
    sepia.b = dot(dstCol.rgb, float3(0.272f, 0.534f, 0.131f));
    dstCol.rgb = lerp(dstCol.rbg, sepia, 1.0f);

    return dstCol;
}