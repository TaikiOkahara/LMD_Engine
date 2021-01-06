#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texDif : register(t0); //Diffuseテクスチャ
Texture2D g_texNor : register(t1); //Normalテクスチャ
Texture2D g_texMRA : register(t2); //Roughnessテクスチャ


SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

    
	//カラーテクスチャーへ出力 
    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex); 
    
	//座標テクスチャ―へ出力
    Out.vPosition = input.WorldPos;
	
	//ワールド法線テクスチャーへ出力
	
    float3 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex).rgb;
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
    bumpNormal = normalize(bumpNormal);
	
   
    
    Out.vNormal = float4(bumpNormal, 0);
    
    
    
    Out.vMotion = input.Velocity;

    
    
    float ambientOcclusion = g_texMRA.Sample(g_samLinear, input.Tex).r;
    float roughness = g_texMRA.Sample(g_samLinear, input.Tex).g;
    float metallic = g_texMRA.Sample(g_samLinear, input.Tex).b;
    
    
    
  
    
    Out.vDepthPBR = float4(input.Depth, roughness, metallic, ambientOcclusion);
    
    return Out;
}