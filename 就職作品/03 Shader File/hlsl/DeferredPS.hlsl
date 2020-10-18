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

   
	//�e�N�X�`���[����������o��
    float4 vDiffuse = g_texColor.Load(sampleIndices);
    float4 vWorldNormal = g_texNormal.Load(sampleIndices);
    float3 vWorldPos = g_texPosition.Load(sampleIndices).xyz;
   
    float3 vLightVector = normalize(g_vLight).xyz;
    float NL = saturate(-dot(vWorldNormal.xyz, vLightVector));
    NL = NL * 0.9f + 0.1f;//���E�ϓI�ɈÂ�
    NL = (vWorldNormal.w == 2.0f) ? 1.0f : NL;//���C�e�B���O���s��
    
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
    //�A���r�G���g---------------------------------------------------
    float4 Ambient = float4(0, 0, 0, 0);
    Color.rgb += Ambient.xyz;
    //---------------------------------------------------------------
    //�X�؃L����-----------------------------------------------------
    //float4 Specular = pow(saturate(-dot(Eye, Reflect)), 30);
    //Color.rgb += Specular;
    //---------------------------------------------------------------
	
	//����-----------------------------------------------------
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
    
    
    
    
    
   
	//���C�g���x�𔽉f
    //Color *= 1;
	//����
    //Color *= 1.0 / (0 + 0 * Distance + 0.1 * Distance * Distance); // att= 1/0 / (a+b*d+c*d^2) d:���� a,b,c�F�萔
    //a�F��茸���W���@b�F���`�����W���@c�F�Q�������W��
   
    //--------------------------------
    
    //Color += PLight(vWorldPos, lightpos, lightDir, vWorldNormal.xyz, input.Tex, Eye);
    
    //--------------------------------
    
    return Color;
   
}

float4 PLight(float3 Pos, float4 LPos, float3 Dir, float3 Normal, float2 UV, float3 EyeVector)
{
	//�����@��
    float4 ambient = float4(0, 0, 0, 0);
	//�g�U���ˌ��@��
    float nl = saturate(-dot(Normal, Dir));
    float4 diffuse = g_texColor.Sample(g_samLinear, UV) * nl;
	//���ʔ��ˌ��@��
    float3 ref = reflect(Normal, LPos.xyz);
    float4 specular = pow(saturate(dot(ref, EyeVector)), 4) * float4(0.4, 0.4, 0.4,0);
	//�t�H�����f���ŏI�F�@�R�̍��̍��v
    float4 color = diffuse;// = ambient + diffuse + specular;
	//���C�g���x�𔽉f
    color *= LPos.w;
	//����
    float Distance = length(LPos.xyz - Pos);
    //color *= pow(saturate(1 / Distance), 2); //�K����1�P�ʂ��猸���J�n
    color *= 1.0 / (0 + 0 * Distance + 0.2 * Distance * Distance); // att= 1/0 / (a+b*d+c*d^2) d:���� a,b,c�F�萔
	
    return color;
}

