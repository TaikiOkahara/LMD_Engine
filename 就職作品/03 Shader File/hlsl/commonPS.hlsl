#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texDif		: register(t0);
Texture2D g_texNor		: register(t1);
Texture2D g_texMRA : register(t2);

SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex);
    Out.vPosition = input.WorldPos;
	
    float4 bump;
	bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
	
    
    float3 bumpNormal;
    bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
    bumpNormal = normalize(bumpNormal);
    
    Out.vNormal = float4(bumpNormal,0);
    Out.vMotionDepth.xy = input.Velocity;
    Out.vMotionDepth.zw = input.DepthZW;
    
    
    float metallic = g_texMRA.Sample(g_samLinear, input.Tex).r;
    float roughness = g_texMRA.Sample(g_samLinear, input.Tex).g;
    float ambientOcclusion = g_texMRA.Sample(g_samLinear, input.Tex).b;
    
    
    
    Out.vPBR = float4(roughness,metallic,ambientOcclusion,0);
    
    return Out;
}