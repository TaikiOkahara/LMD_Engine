#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texDepthPBR : register(t3);


PS_PL_OUT main(VS_PL_OUT input)
{
    PS_PL_OUT Out = (PS_PL_OUT) 0;
    
    int3 sampleIndices = int3(input.Pos.xy, 0);

    float4 normal = g_texNormal.Load(sampleIndices);
    //normal.w = 0.0f;
    //normal = normalize(normal);

    float4 position = g_texPosition.Load(sampleIndices);
    float3 diffuse = g_texColor.Load(sampleIndices).xyz;
    float4 PBR = g_texDepthPBR.Load(sampleIndices);

   

    float3 lpos  = input.LightPos;
    float3 L = position.xyz - lpos;
    float dist = length(L);
    dist = abs(dist);
    
    //if (dist > input.LightRange.x)//距離
    //{
    //    Out.vPointLight = float4(0, 0, 0, 1);
    //    return Out;
    //}
    
    if (input.LightIndex < 0)
    {
        Out.PointLight = float4(0, 0, 0, 0);
        //Out.vColor = float4(0,0,0, 1);
        return Out;
    }
     
        
        //点光源の方向を正規化
        //L /= dist; //L = normalize(L)と同じ
    L = normalize(L);
    
    uint index = input.LightIndex;
    
    
    // att= 1/0 / (a+b*d+c*d^2) d:距離 a,b,c：定数
    //a：一定減衰係数　b：線形減衰係数　c：２次減衰係数
    //float att = max(0.0f, 1.0f - (dist / input.LightRange.x));
    
    float att;
    att = (g_vPointLight[index].calc.x + (g_vPointLight[index].calc.y * dist) + (g_vPointLight[index].calc.z * dist * dist));
    att = 1/att;
    
   
    
    
    
    
   /// float vanish = 1.0f - dist / input.LightRange;//中心から遠くなると暗くなる
    
    //float lightAmount = saturate(dot(normal, L));
    float lightAmount = max(dot(normal.xyz, L),0);
    
    float3 lightColor = g_vPointLight[index].color;
    float3 color = lightAmount * lightColor * att; // * (g_vPointLight[index].intensity / (4.0 * PI)); // * vanish;
   
    
    //Specular calc
    float3 V = position.xyz - g_vEyePos.xyz;
    V = normalize(V);
    float3 H = normalize(L + V);
    //float inputSpecularValue = g_vPointLight[index].specular;
    //float specular = pow(saturate(dot(normal.xyz, H)), inputSpecularValue) * att;

    float3 finalDiffuse = color * diffuse;
    //float3 finalSpecular = specular * diffuse * att;

    //float4 totalColor = float4(finalDiffuse + finalSpecular, 1.0f);

    
    float4 totalColor = float4(color, 1.0f);
    float attenuation = DistanceAttenuation(distance(lpos, position.xyz), input.LightRange);
    float3 irradiance = attenuation * g_vPointLight[index].color * (g_vPointLight[index].intensity / (4.0 * PI)) * max(dot(normal.xyz, L), 0);
   
    
    
    Out.PointLight.rgb = BRDF(diffuse.xyz, PBR.z, PBR.y, normal.xyz, V, L, H) * irradiance + finalDiffuse;
    Out.PointLight.w = 0;//wは投影テクスチャシャドウマップで利用する
    
    //return totalColor;
    
    
    
    
    return Out;
}