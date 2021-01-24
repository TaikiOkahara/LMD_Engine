#ifndef INCLUDE_DIRECTIONALLIGHT
#define INCLUDE_DIRECTIONALLIGHT

#include "ConstantBuffer.hlsl"

float4 DirectionalLighting(float4 color,float4 diffuse, float4 normal, float3 worldPos)
{
   
    
    float3 vLightVector = normalize(g_vDirectionalLightDir).xyz;
    float NL = saturate(-dot(normal.xyz, vLightVector));
    //NL = NL * 0.8f + 0.2f;//   �Œ�l�␳
   
    //float3 color = g_vLightColor.xyz;
    
    
    float3 Eye = g_vEyePos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    //float4 Color = diffuse;
    
    
    //AO�ݒ�
    float ao = g_fAmbientOcclusion.x;
    //Color.rgb -= PBR.w * ao;
    
    //Color.rgb *= diffuse.xyz * NL * color.xyz;
    
    if(normal.w ==2.0f)
        return diffuse;
    
    color.rgb *= diffuse.xyz * NL * g_vLightColor.xyz; //���C�e�B���O���s��
    color.a = diffuse.a;
 
	
    
    //�|�C���g���C�g�����Z
    //Color.rgb += pointLight;
    
    
    
    //���e�e�N�X�`���}�b�s���O
    
    //float shadowTex = 1.0f - g_texShadow.Sample(g_samDeferredLinear, tex).r;
    
    //Color.rgb *= shadowTex;
    
    return color;
   
}

float4 PointLighting(float4 color,float3 pointlight)
{
    color.rgb += pointlight;
    return color;
}



#endif // INCLUDE_DIRECTIONALLIGHT