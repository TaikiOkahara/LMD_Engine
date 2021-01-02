#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor	: register(t0);
Texture2D g_texNormal	: register(t1);
Texture2D g_texPosition	: register(t2);
Texture2D g_texPointLight: register(t3);
Texture2D g_texVelocity  : register(t4);
Texture2D g_texDepthPBR: register(t5);
Texture2D g_texShadow: register(t6);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);



float4 main(VS_OUT input) : SV_Target
{
    int3 sampleIndices = int3(input.Pos.xy, 0);

    //float4 tex_color = Texture.Sample(Sampler, input.texture_pos);
    //float4 vDiferredColor = g_texDeferred.Load(sampleIndices);
	//�e�N�X�`���[����������o��
    float4 diffuse = g_texColor.Load(sampleIndices);
    float4 normal = g_texNormal.Load(sampleIndices);
    float3 worldPos = g_texPosition.Load(sampleIndices).xyz;
    float3 pointLight = g_texPointLight.Load(sampleIndices);
    float4 PBR = g_texDepthPBR.Load(sampleIndices);
    
    float3 vLightVector = normalize(g_vDirectionalLightDir).xyz;
    float NL = saturate(-dot(normal.xyz, vLightVector));
    //NL = NL * 0.8f + 0.2f;//���E�ϓI�ɈÂ�
   
    float3 color = g_vLightColor.xyz;
    
    
    float3 Eye = g_vEyePos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    float4 Color = diffuse;
    
    
    //AO�ݒ�
    float ao = g_fAmbientOcclusion.x;
    Color.rgb += PBR.w * ao;
    
    //Color.rgb *= diffuse.xyz * NL * color.xyz;
    Color.rgb *= (normal.w == 2.0f) ? diffuse.xyz : diffuse.xyz * NL * color.xyz; //���C�e�B���O���s��
    Color.a = diffuse.a;
    //�A���r�G���g---------------------------------------------------
    //float4 Ambient = float4(0, 0, 0, 0);
    //Color.rgb += Ambient.xyz;
    //---------------------------------------------------------------
    //�X�؃L����-----------------------------------------------------
    //float4 Specular = pow(saturate(dot(Eye, Reflect)), 100);
    
    //float specularMap = g_texDepthPBR.Load(sampleIndices).g;
    //Specular *= specularMap;
    
    //Color.rgb = saturate(Color.rgb + Specular.rgb);
   
    //---------------------------------------------------------------
	
	//����-----------------------------------------------------
    //float Rim = 1.0f + dot(vLightVector, normal.xyz);
    //Rim = pow(Rim, 2) * 0.6;
    //Color.rgb += Rim;
    //---------------------------------------------------------------
	
    float3 L = -g_vDirectionalLightDir.xyz;
    L = normalize(L);
   
    float3 V = worldPos.xyz - g_vEyePos.xyz;
    V = normalize(V);
    float3 H = normalize(L + V);
    
    //Color.rgb = BRDF(diffuse.xyz, PBR.z, PBR.y, normal.xyz, V, L, H) * color.xyz;
    //Color.rgb = NormalizedLambert(diffuse.xyz) * color.xyz;
    //Color.rgb += SpecularBRDF(diffuse.xyz, PBR.z, PBR.y, normal.xyz, V, L, H);
    
  
    
    
    
    
    
   
    
    //�|�C���g���C�g�����Z
    Color.rgb += pointLight;
    
    
    
    //���e�e�N�X�`���}�b�s���O
    
    float shadowTex =1.0f -  g_texShadow.Sample(g_samDeferredLinear, input.Tex).r;
    
    Color.rgb *= shadowTex;
    
    return Color;
    //return finalColor;
}