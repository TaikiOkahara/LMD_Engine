#include "geometry.hlsl"
#include "constantBuffer.hlsl"
#include "lighting.hlsli"

//GBuffer
Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texPBR : register(t3);


PS_PL_OUT main(VS_PL_OUT input)
{
    PS_PL_OUT Out = (PS_PL_OUT) 0;
    
    int3 sampleIndices = int3(input.Pos.xy, 0);

    float4 normal = g_texNormal.Load(sampleIndices);

    float4 position = g_texPosition.Load(sampleIndices);
    float3 diffuse = g_texColor.Load(sampleIndices).xyz;
    float4 PBR = g_texPBR.Load(sampleIndices);

   

    float3 lpos  = g_vPointLight.pos;
    float3 L = position.xyz - lpos;
    float dist = length(L);
    dist = abs(dist);
    
    L = normalize(L);
    
    
    
    // att= 1/0 / (a+b*d+c*d^2) d:���� a,b,c�F�萔
    //a�F��茸���W���@b�F���`�����W���@c�F�Q�������W��
    float att;
    att = (g_vPointLight.calc.x + (g_vPointLight.calc.y * dist) + (g_vPointLight.calc.z * dist * dist));
    att = 1/att;
    
   
    float lightAmount = max(dot(normal.xyz, L),0);
    
    float3 lightColor = g_vPointLight.color;
    float3 color = lightAmount * lightColor * att;
   
    
    //Specular calc
    float3 V = position.xyz - g_vCameraPos.xyz;
    V = normalize(V);
    float3 H = normalize(L + V);
  
    float3 finalDiffuse = color * diffuse;
 
    float attenuation = DistanceAttenuation(distance(lpos, position.xyz), g_vPointLight.size);
    float3 irradiance = attenuation * g_vPointLight.color * (g_vPointLight.intensity / (4.0 * PI)) * max(dot(normal.xyz, L), 0);
   
    
    Out.PointLight.rgb = BRDF(diffuse.xyz, PBR.g, PBR.r, normal.xyz, V, L, H) * irradiance + finalDiffuse;
    Out.PointLight.w = 0;
    
    
    
    return Out;
}