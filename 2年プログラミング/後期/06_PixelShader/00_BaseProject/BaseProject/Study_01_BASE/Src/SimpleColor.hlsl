//SV_TARGET セマンティクス 
//どの要素で返すかを指定する　今回の場合は色
float4 main() : SV_TARGET
{
	//            R     G     B     透明度
	return float4(0.0f, 0.68f, 0.68f, 1.0f);
}