#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texDif		: register(t0);//Diffuseテクスチャ
Texture2D g_texNor		: register(t1);//Normalテクスチャ


SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex);
    Out.vPosition = input.WorldPos;
	
    float3 normal = input.WorldNormal;
       
    Out.vNormal = float4(normal,2);
    
    
    return Out;
}