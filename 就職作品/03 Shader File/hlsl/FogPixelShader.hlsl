#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texPointLight : register(t3);
Texture2D g_texMotion : register(t4);

Texture2D g_texFog : register(t5);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
    //�t�H�O�̗h�炬�e�N�X�`���[�̃e�N�Z���̓��[���h�s���K���������_���W��[ X, Z ]���g�p����B
   //���̂܂܂ł͎g�p�ł��Ȃ��̂�m_TexScale�ϐ��l�ɂ���Ē�������B
   //�܂��e�N�Z�����W��m_Texel�ϐ��l�ɂ���Ĉړ������A�Q�̐F���̕��ϒl���ŏI�I�ȃt�H�O�J���[�Ƃ���
    
    
    if(g_fFogEnable == 0) 
        return float4(0, 0, 0, 1);
    
    
    
    //float3 motion = g_texMotion.Sample(g_samLinear, input.Tex).xyz;
    
    //float4 color = g_texColor.Sample(g_samLinear, input.Tex);
    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
    
    //if (length(motion) != 0)
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    //float2 uv = input.Tex;
    
    
    
    
    float texScale = g_fFogData.x;
    float maxHeight = g_fFogData.y;
    float minHeight = g_fFogData.z;
    
    
    float FogColor = (g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.xy).r +
                      g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.zw).r * 0.2f) * 0.5f; //0.2f�̓t�H�O�Ɍ��Ԃ���������臒l
    
    
    //�㉺�ɔg�������邽�߂ɍ���������������
    float randomHeight = g_texFog.Sample(g_samLinear, g_fFogOffset.xy).r + g_texFog.Sample(g_samLinear, g_fFogOffset.xz).r;
    randomHeight *= 0.5f;
    
   //���_�̍����ɂ���ă��l���v�Z
  
    float Alpha = saturate((position.y + randomHeight - minHeight) / (maxHeight - minHeight));
    
    
   
   //�t�H�O�̗h�炬���ɂ�胿�l�𒲐�
    Alpha = 1.0f - (1.0f - Alpha) * FogColor;
   
   //�I�u�W�F�N�g�̐F���ƃt�H�O�J���[����`����
    float4 fogColor = float4(g_fFogColor, 1);
    float4 Out = fogColor * (1.0f - Alpha);

    return Out;
}