#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texCol : register(t0);
Texture2D g_Nor : register(t1);
Texture2D g_MRA : register(t2);
Texture2D g_texShadow : register(t3);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);

float4 main(VS_SHADOW_OUT input) : SV_Target
{
    input.ProjectorSpacePos_1.xyz /= input.ProjectorSpacePos_1.w;
    input.ProjectorSpacePos_2.xyz /= input.ProjectorSpacePos_2.w;
    input.ProjectorSpacePos_3.xyz /= input.ProjectorSpacePos_3.w;
    
    float2 uv_1 = input.ProjectorSpacePos_1.xy;
    float2 uv_2 = input.ProjectorSpacePos_2.xy;
    float2 uv_3 = input.ProjectorSpacePos_3.xy;
    
    
    float shadow;
    shadow = g_texShadow.Sample(g_samDeferredLinear, uv_1).r;
    shadow += g_texShadow.Sample(g_samDeferredLinear, uv_2).g;
    shadow += g_texShadow.Sample(g_samDeferredLinear, uv_3).b;
    
   
    float dist =(distance(input.WorldPos, g_vPointLightPlayerPos.xyz));
    
    if (dist > 1.0f)
        return float4(0, 0, 0, 0);
    
    dist = 1 - clamp(dist, 0, 1);
    
    shadow *= dist;
    
    return float4(0,0,0,shadow);
}
