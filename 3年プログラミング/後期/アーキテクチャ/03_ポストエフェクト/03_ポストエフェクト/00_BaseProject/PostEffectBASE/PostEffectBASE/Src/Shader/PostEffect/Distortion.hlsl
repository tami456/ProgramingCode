#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�(b4�Ə���)
cbuffer cbParam : register(b4)
{

}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // �����Y�̒��S���W���`
    float2 lensCenter = float2(0.5, 0.5);

    // �����Y�̒��S����̋������v�Z
    float lensRadius = distance(lensCenter, uv);

    // �����Y�̘c�݌W����ݒ�
    float distortionAmount = 0.4; // �����\�Ȓl

    // �c�݂�K�p
    uv = uv + (uv - lensCenter) * lensRadius * distortionAmount;

    // �c�݂�K�p�������W�ɑΉ�����e�N�X�`���s�N�Z�����擾
    float4 srcCol = tex.Sample(texSampler, uv);

    return srcCol;
}

