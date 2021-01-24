#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


float4 DirectionalLight(float4 Color, float2 tex, float4 diffuse, float4 normal, float3 worldPos)
{
    //int3 sampleIndices = int3(input.Pos.xy, 0);

	//�e�N�X�`���[����������o��
    //float4 diffuse = g_texColor.Load(sampleIndices);
    //float4 normal = g_texNormal.Load(sampleIndices);
    //float3 worldPos = g_texPosition.Load(sampleIndices).xyz;
    //float3 pointLight = g_texPointLight.Load(sampleIndices);
    //float4 PBR = g_texDepthPBR.Load(sampleIndices);
    
    float3 vLightVector = normalize(g_vDirectionalLightDir).xyz;
    float NL = saturate(-dot(normal.xyz, vLightVector));
    //NL = NL * 0.8f + 0.2f;//   �Œ�l�␳
   
    float3 color = g_vLightColor.xyz;
    
    
    float3 Eye = g_vEyePos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    //float4 Color = diffuse;
    
    
    //AO�ݒ�
    float ao = g_fAmbientOcclusion.x;
    //Color.rgb -= PBR.w * ao;
    
    //Color.rgb *= diffuse.xyz * NL * color.xyz;
    Color.rgb *= (normal.w == 2.0f) ? diffuse.xyz : diffuse.xyz * NL * color.xyz; //���C�e�B���O���s��
    Color.a = diffuse.a;
 
	
    float3 L = -g_vDirectionalLightDir.xyz;
    L = normalize(L);
   
    float3 V = worldPos.xyz - g_vEyePos.xyz;
    V = normalize(V);
    float3 H = normalize(L + V);
    
    
    
    //�|�C���g���C�g�����Z
    //Color.rgb += pointLight;
    
    
    
    //���e�e�N�X�`���}�b�s���O
    
    float shadowTex = 1.0f - g_texShadow.Sample(g_samDeferredLinear, tex).r;
    
    Color.rgb *= shadowTex;
    
    return Color;
   
}