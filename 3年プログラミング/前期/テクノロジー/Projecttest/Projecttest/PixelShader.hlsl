Texture2D tex : register(t0);
Texture2D norm : register(t1);
SamplerState smp:register(s0);

struct PsInput
{
	float4 pos:SV_POSITION;
	float4 dif:COLOR;
	float2 uv:TEXCORD;
};

struct PsOutput 
{
	float4 col:SV_TARGET0;
	float4 norm: SV_TARGET1;
};

PsOutput main(PsInput input)
{
	float3 light = float3(1,-1,1);
	light = normalize(light);
	float3 n = norm.Sample(smp,input.uv).xyz;
	n.xy *= 2.0;
	n.xy -= 1.0;
	n.z = -sqrt(1 - n.x * n.x - n.y * n.y);
	float b = max(saturate(dot(n, -light)), 0.25);
	light = reflect(light, n);
	float spec = pow(saturate(dot(light, float3(0, 0, -1))),10);
	float4 col = tex.Sample(smp, input.uv);
	PsOutput output;
	output.col = float4(col.rgb * b + spec,col.a);
	output.norm = step(0.5, spec);
	return output;
}