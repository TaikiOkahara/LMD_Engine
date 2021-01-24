#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


float4 DirectionalLight(float4 Color, float2 tex, float4 diffuse, float4 normal, float3 worldPos)
{
    //int3 sampleIndices = int3(input.Pos.xy, 0);

	//テクスチャーから情報を取り出す
    //float4 diffuse = g_texColor.Load(sampleIndices);
    //float4 normal = g_texNormal.Load(sampleIndices);
    //float3 worldPos = g_texPosition.Load(sampleIndices).xyz;
    //float3 pointLight = g_texPointLight.Load(sampleIndices);
    //float4 PBR = g_texDepthPBR.Load(sampleIndices);
    
    float3 vLightVector = normalize(g_vDirectionalLightDir).xyz;
    float NL = saturate(-dot(normal.xyz, vLightVector));
    //NL = NL * 0.8f + 0.2f;//   最低値補正
   
    float3 color = g_vLightColor.xyz;
    
    
    float3 Eye = g_vEyePos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    //float4 Color = diffuse;
    
    
    //AO設定
    float ao = g_fAmbientOcclusion.x;
    //Color.rgb -= PBR.w * ao;
    
    //Color.rgb *= diffuse.xyz * NL * color.xyz;
    Color.rgb *= (normal.w == 2.0f) ? diffuse.xyz : diffuse.xyz * NL * color.xyz; //ライティングを行か
    Color.a = diffuse.a;
 
	
    float3 L = -g_vDirectionalLightDir.xyz;
    L = normalize(L);
   
    float3 V = worldPos.xyz - g_vEyePos.xyz;
    V = normalize(V);
    float3 H = normalize(L + V);
    
    
    
    //ポイントライトを加算
    //Color.rgb += pointLight;
    
    
    
    //投影テクスチャマッピング
    
    float shadowTex = 1.0f - g_texShadow.Sample(g_samDeferredLinear, tex).r;
    
    Color.rgb *= shadowTex;
    
    return Color;
   
}