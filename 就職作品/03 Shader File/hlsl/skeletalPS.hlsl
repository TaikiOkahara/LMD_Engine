#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texDif : register(t0); //Diffuse�e�N�X�`��
Texture2D g_texNor : register(t1); //Normal�e�N�X�`��
Texture2D g_texMRA : register(t2); //Roughness�e�N�X�`��


SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

    
	//�J���[�e�N�X�`���[�֏o�� 
    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex); 
    
	//���W�e�N�X�`���\�֏o��
    Out.vPosition = input.WorldPos;
	
	//���[���h�@���e�N�X�`���[�֏o��
	
    float3 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex).rgb;
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
    bumpNormal = normalize(bumpNormal);
	
   
    
    Out.vNormal = float4(bumpNormal, 0);
    
    
    
    Out.vMotion = input.Velocity;

    
    
    float ambientOcclusion = g_texMRA.Sample(g_samLinear, input.Tex).r;
    float roughness = g_texMRA.Sample(g_samLinear, input.Tex).g;
    float metallic = g_texMRA.Sample(g_samLinear, input.Tex).b;
    
    
    
  
    
    Out.vDepthPBR = float4(input.Depth, roughness, metallic, ambientOcclusion);
    
    return Out;
}