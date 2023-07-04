
//定数バッファ：スロット0番目(スロットの0番目)
cbuffer cbParam : register(b3)
{
	//グローバル変数だからどこでも使える
	float4 g_color;
}

float4 main() : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//受け取ったカラーを返す
	return g_color;
}