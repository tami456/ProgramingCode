#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
	//UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
	float4 srcCol = tex.Sample(texSampler,PSInput.uv);
	if (srcCol.a < 0.01f)
	{
		//�`�悵�Ȃ�(�A���t�@�e�X�g)
		discard;
	}

	return dot(srcCol.rgb, float3(0.299, 0.587, 0.114));
}