#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{
	float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	float2 uv = PSInput.uv;

	//UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
	float4 srcCol = tex.Sample(texSampler,PSInput.uv);

	if (srcCol.a < 0.01f)
	{
		//�`�悵�Ȃ�(�A���t�@�e�X�g)
		discard;
	}
	//�Ԋu�����
	float area = sin(uv.y * 2.0f - g_time * 0.5f);

	//sin�J�[�u���ψꉻ���ꂽ����͈͂Ƃ��邽�߂ɁA
	//step�֐����g�p���āA�l��1 or 0 �ɂ���B
	float isArea = step(0.996f, area * area);

	//����͈͂����A�F���Z���s�����邭����B
	srcCol.rgb += isArea * 0.3f;

	//���̂܂܂��ƁA����͈͂ɂ����n�̎Ȗ͗l�������Ă��܂��̂ŁA�����悤�ɂ���B
	srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
	srcCol.rgb -= (1.0f - isArea) * abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

	return srcCol;
}