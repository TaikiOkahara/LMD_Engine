#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

//GBuffer
Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);


float4 main(VS_LIGHT_OUT input) : SV_Target
{
    
    int3 sampleIndices = int3(input.Pos.xy, 0);

    float3 normal = g_texNormal.Load(sampleIndices).xyz;
    float3 position = g_texPosition.Load(sampleIndices).xyz;
    float3 diffuse = g_texColor.Load(sampleIndices).xyz;

    
    //今は1つだけ
    float3 lpos  = input.LightPos;
    float3 L = lpos - position;
    float dist = length(L);

    if (dist > input.LightRange.x)//距離
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    //点光源の方向を正規化
    L /= dist; //L = normalize(L)と同じ

    
    uint index = input.LightIndex;
    
    
    //float att = max(0.0f, 1.0f - (dist / input.LightRange.x));
    // att= 1/0 / (a+b*d+c*d^2) d:距離 a,b,c：定数
    //a：一定減衰係数　b：線形減衰係数　c：２次減衰係数
    float att = 1.0 / (g_vPoint[index].x + g_vPoint[index].y * dist + g_vPoint[index].z * dist * dist);
    
    float lightAmount = saturate(dot(normal, L));
    //float lightAmount = (dot(normal, L) + 1.0f)/2.0f;
    
    float3 lightColor = g_vPointColor[index].xyz;
    float3 color = lightAmount * lightColor * att;

    
    
	//Specular calc
    float3 V = g_vEye.xyz - position;
    float3 H = normalize(L + V);
    float inputSpecularValue = g_vPoint[index].w;
    float specular = pow(saturate(dot(normal, H)), inputSpecularValue) * att;

    float3 finalDiffuse = color * diffuse;
    float3 finalSpecular = specular * diffuse * att;

    float4 totalColor = float4(finalDiffuse + finalSpecular, 1.0f);
    //float4 totalColor = float4(finalDiffuse , 1.0f);
    return totalColor;
}