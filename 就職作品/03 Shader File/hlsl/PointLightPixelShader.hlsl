#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);


float4 main(VS_LIGHT_OUT input) : SV_Target
{
    //PS_OUT Out = (PS_OUT) 0;
    
    int3 sampleIndices = int3(input.Pos.xy, 0);

    float3 normal = g_texNormal.Load(sampleIndices).xyz;
    normal = normalize(normal);

    float4 position = g_texPosition.Load(sampleIndices);
    float3 diffuse = g_texColor.Load(sampleIndices).xyz;

    

    float3 lpos  = input.LightPos;
    float3 L = position.xyz - lpos;
    float dist = length(L);
    //dist = abs(dist);
    
    if (dist > input.LightRange.x)//����
    {
        return float4(0, 0, 0, 1);
    }
    
     
        
        //�_�����̕����𐳋K��
        //L /= dist; //L = normalize(L)�Ɠ���
    L = normalize(L);
    
    uint index = input.LightIndex;
    
    
    // att= 1/0 / (a+b*d+c*d^2) d:���� a,b,c�F�萔
    //a�F��茸���W���@b�F���`�����W���@c�F�Q�������W��
    //float att = max(0.0f, 1.0f - (dist / input.LightRange.x));
    float att = 1.0 / (g_vPoint[index].x + (g_vPoint[index].y * dist) + (g_vPoint[index].z * dist * dist));
    
    //float lightAmount = saturate(dot(normal, L));
    float lightAmount = (dot(normal, L) + 1.0f) / 2.0f;
    
    float3 lightColor = g_vPointColor[index].xyz;
    float3 color = lightAmount * lightColor * att;

    
    //Specular calc
    float3 V = g_vEye.xyz - position.xyz;
    float3 H = normalize(L + V);
    float inputSpecularValue = g_vPoint[index].w;
    float specular = pow(saturate(dot(normal, H)), inputSpecularValue) * att;

    float3 finalDiffuse = color * diffuse;
    float3 finalSpecular = specular * diffuse * att;

    float4 totalColor = float4(finalDiffuse + finalSpecular, 1.0f);

    
    //float4 totalColor = float4(color, 1.0f);
       
    
    //Out.vColor =  totalColor;
    return totalColor;
}