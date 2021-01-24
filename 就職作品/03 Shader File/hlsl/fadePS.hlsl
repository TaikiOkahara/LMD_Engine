#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texPointLight : register(t3);
Texture2D g_texMotion : register(t4);

Texture2D g_texPBR : register(t5);
Texture2D g_teShadow : register(t6);
Texture2D g_texFog : register(t7);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
  
    
    if(g_fFogEnable == 0) 
        discard;
    
    float4 normal = g_texNormal.Sample(g_samLinear, input.Tex);
    if(normal.w == 2)
        discard;
    
    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
   
    float texScale = g_fFogData.x;
    float maxHeight = g_fFogData.y;
    float minHeight = g_fFogData.z;
    
    
    if (position.y >= maxHeight)
        discard;
    
        
        
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