#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

Texture2D g_texFog : register(t3);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
    //�t�H�O�̗h�炬�e�N�X�`���[�̃e�N�Z���̓��[���h�s���K���������_���W��[ X, Z ]���g�p����B
   //���̂܂܂ł͎g�p�ł��Ȃ��̂�m_TexScale�ϐ��l�ɂ���Ē�������B
   //�܂��e�N�Z�����W��m_Texel�ϐ��l�ɂ���Ĉړ������A�Q�̐F���̕��ϒl���ŏI�I�ȃt�H�O�J���[�Ƃ���
    float4 color = g_texColor.Sample(g_samLinear, input.Tex);
    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
    
    //return (position.y < 1.0f) ? float4(1, 1, 1, 1) : float4(0, 0, 0, 1);
   
    //float m_TexScale = 0.01f; 
    //float2 m_Texel[2] = {1,1,1,1}; 
    //float m_MaxHeight = 10.0f; 
    //float m_MinHeight = 2.0f; 
    
    float FogColor = (g_texFog.Sample(g_samLinear, position.xz * g_fFogTexScale + g_fFogOffset0).r +
                      g_texFog.Sample(g_samLinear, position.xz * g_fFogTexScale + g_fFogOffset1).r) * 0.5f;
   
    //�㉺�ɔg�������邽�߂ɍ���������������
    float randomHeight = g_texFog.Sample(g_samLinear, input.Tex).r;
    
   //���_�̍����ɂ���ă��l���v�Z����B
   //���_���W >= m_MaxHeight �̂Ƃ� 1.0f
   //���_���W <= m_MinHeight �̂Ƃ� 0.0f
    float Alpha = saturate((position.y - g_fFogMinHeight) / (g_fFogMaxHeight - g_fFogMinHeight));
    
    
   
   //�t�H�O�̗h�炬���ɂ�胿�l�𒲐�����B
    Alpha = 1.0f - (1.0f - Alpha) * FogColor;
   
   //�I�u�W�F�N�g�̐F���ƃt�H�O�J���[����`��������
    float4 fogColor = float4(g_fFogColor, 1);
    float4 Out = fogColor * (1.0f - Alpha);
    //float4 Out = In.Col * tex2D(tex0, In.Tex) * Alpha + m_FogColor * (1.0f - Alpha);

    return Out;
}