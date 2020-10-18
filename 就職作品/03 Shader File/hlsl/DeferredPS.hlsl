#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

//GBuffer
Texture2D g_texColor	: register(t2);
Texture2D g_texNormal	: register(t3);
Texture2D g_texPosition	: register(t4);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 PLight(float3 Pos, float4 LPos, float3 Dir, float3 Normal, float2 UV, float3 EyeVector);

float4 main(VS_OUT input) : SV_Target
{
    int3 sampleIndices = int3(input.Pos.xy, 0);

   
	//テクスチャーから情報を取り出す
    float4 vDiffuse = g_texColor.Load(sampleIndices);
    float4 vWorldNormal = g_texNormal.Load(sampleIndices);
    float3 vWorldPos = g_texPosition.Load(sampleIndices).xyz;
   
    float3 vLightVector = normalize(g_vLight).xyz;
    float NL = saturate(-dot(vWorldNormal.xyz, vLightVector));
    NL = NL * 0.9f + 0.1f;//世界観的に暗め
    NL = (vWorldNormal.w == 2.0f) ? 1.0f : NL;//ライティングを行か
    
    //float3 LightColor = float3(0.5f, 0.5f, 0.5f);
    //LightColor *= NL;
	
	
    //float3 Eye = vWorldPos - g_vEye.xyz;
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
    
    
    //float4 lightColor = float4(1, 1, 1, 1);
    
    //float4 lightpos = g_vPointLight[0];
    //float Distance = length(lightpos.xyz - vWorldPos);
    //float3 lightDir = normalize(lightpos.xyz - vWorldPos);
    //float attenution = 1.0f / length(lightDir);
    
    
    
    
    
   
	//ライト強度を反映
    //Color *= 1;
	//減衰
    //Color *= 1.0 / (0 + 0 * Distance + 0.1 * Distance * Distance); // att= 1/0 / (a+b*d+c*d^2) d:距離 a,b,c：定数
    //a：一定減衰係数　b：線形減衰係数　c：２次減衰係数
   
    //--------------------------------
    
    //Color += PLight(vWorldPos, lightpos, lightDir, vWorldNormal.xyz, input.Tex, Eye);
    
    //--------------------------------
    
    return Color;
   
}

float4 PLight(float3 Pos, float4 LPos, float3 Dir, float3 Normal, float2 UV, float3 EyeVector)
{
	//環境光　項
    float4 ambient = float4(0, 0, 0, 0);
	//拡散反射光　項
    float nl = saturate(-dot(Normal, Dir));
    float4 diffuse = g_texColor.Sample(g_samLinear, UV) * nl;
	//鏡面反射光　項
    float3 ref = reflect(Normal, LPos.xyz);
    float4 specular = pow(saturate(dot(ref, EyeVector)), 4) * float4(0.4, 0.4, 0.4,0);
	//フォンモデル最終色　３つの項の合計
    float4 color = diffuse;// = ambient + diffuse + specular;
	//ライト強度を反映
    color *= LPos.w;
	//減衰
    float Distance = length(LPos.xyz - Pos);
    //color *= pow(saturate(1 / Distance), 2); //適当に1単位から減衰開始
    color *= 1.0 / (0 + 0 * Distance + 0.2 * Distance * Distance); // att= 1/0 / (a+b*d+c*d^2) d:距離 a,b,c：定数
	
    return color;
}

