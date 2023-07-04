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
cbuffer cbParam : register(b5)
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


    return srcCol;
}