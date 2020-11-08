#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor	: register(t0);
//Texture2D g_texNormal	: register(t1);
//Texture2D g_texPosition	: register(t2);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


//float4 PLight(float3 Pos, float4 LPos, float3 Dir, float3 Normal, float2 UV, float3 EyeVector);

float4 main(VS_OUT input) : SV_Target
{
    //int3 sampleIndices = int3(input.Pos.xy, 0);

   
	//テクスチャーから情報を取り出す
    //float4 vDiffuse = g_texColor.Load(sampleIndices);
    //float4 vWorldNormal = g_texNormal.Load(sampleIndices);
    //float3 vWorldPos = g_texPosition.Load(sampleIndices).xyz;
   
    //float3 vLightVector = normalize(g_vLight).xyz;
    //float NL = saturate(-dot(vWorldNormal.xyz, vLightVector));
    //NL = NL * 0.9f + 0.1f;//世界観的に暗め
    //NL = (vWorldNormal.w == 2.0f) ? 1.0f : NL;//ライティングを行か
    
  
	
    //float3 Eye = vWorldPos - g_vEye.xyz;
    //float3 Eye = g_vEye.xyz - vWorldPos;
   // Eye = normalize(Eye);

    //float3 Reflect = reflect(vLightVector, vWorldNormal.xyz);
    //Reflect = normalize(Reflect);
	
    
    
    float4 Color = g_texColor.Sample(g_samDeferredLinear, input.Tex);
    
    
    
    
    return Color;
}
