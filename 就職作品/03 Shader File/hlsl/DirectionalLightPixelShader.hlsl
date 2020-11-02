#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

//GBuffer
Texture2D g_texDeferred	: register(t0);
Texture2D g_texColor	: register(t1);
Texture2D g_texNormal	: register(t2);
Texture2D g_texPosition	: register(t3);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);



float4 main(VS_OUT input) : SV_Target
{
    int3 sampleIndices = int3(input.Pos.xy, 0);

    //float4 tex_color = Texture.Sample(Sampler, input.texture_pos);
    //float4 vDiferredColor = g_texDeferred.Load(sampleIndices);
	//テクスチャーから情報を取り出す
    float4 vDiffuse = g_texColor.Load(sampleIndices);
    float4 vWorldNormal = g_texNormal.Load(sampleIndices);
    float3 vWorldPos = g_texPosition.Load(sampleIndices).xyz;
   
    float3 vLightVector = normalize(g_vLight).xyz;
    float NL = saturate(-dot(vWorldNormal.xyz, vLightVector));
    NL = NL * 0.9f + 0.1f;//世界観的に暗め
    NL = (vWorldNormal.w == 2.0f) ? 1.0f : NL;//ライティングを行か
    
    
    
    float3 Eye = g_vEye.xyz - vWorldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, vWorldNormal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    float4 Color = vDiffuse;
    
    
    Color.rgb *= vDiffuse.xyz * NL;
    Color.a = vDiffuse.a;
    //アンビエント---------------------------------------------------
    float4 Ambient = float4(0, 0, 0, 0);
    Color.rgb += Ambient.xyz;
    //---------------------------------------------------------------
    //スぺキュラ-----------------------------------------------------
    //float4 Specular = pow(saturate(-dot(Eye, Reflect)), 30);
    //Color.rgb += Specular;
    //---------------------------------------------------------------
	
	//リム-----------------------------------------------------
    //float Rim = 1.0f + dot(Eye, vWorldNormal.xyz);
    //Rim = pow(Rim, 2) * 0.6f;
    //Color.rgb += Rim;
    //---------------------------------------------------------------
	

    return Color;
   
}