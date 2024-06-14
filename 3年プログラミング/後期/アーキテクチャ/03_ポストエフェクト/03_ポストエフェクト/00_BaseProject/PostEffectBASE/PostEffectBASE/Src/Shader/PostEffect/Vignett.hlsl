#include "../Common/Pixel/PixelShader2DHeader.hlsli"

//�萔�o�b�t�@�F�X���b�g�ԍ�3�Ԗځib3�j
cbuffer cbParam : register(b4)
{
    float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //UV���W����
    float2 uv = PSInput.uv;
    //�ύX���ꂽuv���W�����ƂɐF���擾����
    float4 srcCol =
        tex.Sample(texSampler, uv);

    // ��ʒ��S�̍��W
    float2 center = float2(0.5, 0.5);
    // �s�N�Z�����W�����ʒ��S�ւ̋������v�Z
    float distance = length(uv - center);
    // �r�l�b�g���ʂ̋��x�i�����\�j
    float strength = 1.5; // �������Ă�������
    // �r�l�b�g���ʂ̋��x���v�Z
    float vignette = saturate(1.0 - strength * distance);
    // �r�l�b�g���ʂ�K�p
    srcCol.rgb *= vignette; // �F�Ƀr�l�b�g���ʂ�K�p

    return srcCol;
}