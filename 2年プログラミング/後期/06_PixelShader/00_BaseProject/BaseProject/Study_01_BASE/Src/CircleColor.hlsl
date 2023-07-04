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
cbuffer cbParam : register(b3)
{
    //グローバル変数だからどこでも使える
    float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

    //UV座標を取得する
    float2 uv = PSInput.TexCoords0;
    //(a)左→右　
    //return float4(uv.x, uv.x, uv.x, 1.0f);

    //(b)上→下
    //return float4(1.0f - uv.y, 1.0f - uv.y, 1.0f - uv.y, 1.0f);

    //(c)左上→右下　
    float4 color = float4(uv.x + uv.y, uv.x + uv.y, uv.x + uv.y, 1.0f);
    //uv.x+uv.yは2になってしまうから1に戻したい
    /*color.x /= 2.0f;
    color.y /= 2.0f;
    color.z /= 2.0f;*/
    //シェーダー特有の計算方法
    color.xyz /= 2.0f;
    return color;
}