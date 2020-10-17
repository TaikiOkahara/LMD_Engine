#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

//GBuffer
Texture2D g_texColor : register(t2);
Texture2D g_texNormal : register(t3);
Texture2D g_texPosition : register(t4);


float4 main(VS_OUT input) : SV_Target
{
    
    int3 sampleIndices = int3(input.Pos.xy, 0);

    float3 normal = g_texNormal.Load(sampleIndices).xyz;

    float3 position = g_texPosition.Load(sampleIndices).xyz;

    float3 diffuse = g_texColor.Load(sampleIndices).xyz;

    
    //今は1つだけ
    float3 lpos  = g_vPointLight[0].xyz;
    float3 L = lpos - position;
    float dist = length(L);

    if (dist > 1.0f)//距離
    {
        //チェック赤色
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    //点光源の方向を正規化
    L /= dist; //L = normalize(L)と同じ

    float att = max(0.0f, 1.0f - (dist / 1.0f));

    //float lightAmount = saturate(dot(normal, L));
    float lightAmount = (dot(normal, L) + 1.0f)/2.0f;
    
    float3 lightColor = float3(1, 1, 1);
    float3 color = lightAmount * lightColor * att;

	//Specular calc
    //float3 V = g_vEye.xyz - position;
    //float3 H = normalize(L + V);
    //float specular = pow(saturate(dot(normal, H)), 10) * att;

    float3 finalDiffuse = color * diffuse;
    //float3 finalSpecular = specular * diffuse * att;

    //float4 totalColor = float4(finalDiffuse + finalSpecular, 1.0f);
    float4 totalColor = float4(finalDiffuse , 1.0f);
    return totalColor;
}