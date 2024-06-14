#include "../Common/Pixel/PixelShader2DHeader.hlsli"

//�萔�o�b�t�@�F�X���b�g�ԍ�4�Ԗځib4�j
cbuffer cbParam : register(b4)
{
    float g_time;
    float2 screen_size;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // ���Ԍo�߂ɉ����Ăڂ����̋����𑝉�
    float blurStrength = 0.5 * g_time;

    //float blurStrength = 0.1 * sin(g_time);
    blurStrength = min(blurStrength, 3);
    float2 texelSize = 1.0 / screen_size;
    float4 color = float4(0.0, 0.0, 0.0, 0.0);

    //UV���W����
    float2 uv = PSInput.uv;
    float4 srcCol =
        tex.Sample(texSampler, uv);

    // �ڂ����̃J�[�l���T�C�Y���v�Z
     int kernelSize = int(1 + blurStrength * 3);

     for (int i = -kernelSize; i <= kernelSize; ++i)
     {
         // �s�N�Z���̃T���v�����O
         float2 offset = i * texelSize;
         color += tex.Sample(texSampler, uv + offset);
     }

     // �J�[�l�����̃s�N�Z���̕��ω�
     color /= 2 * kernelSize + 1;

     return color;
}