#include "Geometry.hlsl"


Texture2D g_tex			: register(t0);
Texture2D g_texNor		: register(t1);
Texture2D g_texMRA : register(t2);



SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	//�J���[�e�N�X�`���[�֏o�� 
	Out.vColor = g_tex.Sample(g_samLinear, input.Tex);

	//���W�e�N�X�`���\�֏o��
    Out.vPosition = input.WorldPos;
	
	
	//���[���h�@���e�N�X�`���[�֏o��
    float4 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (bump.x * input.WorldTangent) + (bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
	
    bumpNormal = normalize(bumpNormal);
    
    
    
    Out.vNormal = float4(bumpNormal,0);
	
    Out.vMotion = input.Velocity;
    
    float metallic = g_texMRA.Sample(g_samLinear, input.Tex).r;
    float roughness = g_texMRA.Sample(g_samLinear, input.Tex).g;
    float ambientOcclusion = g_texMRA.Sample(g_samLinear, input.Tex).b;
    
    
    //Out.vColor *= Out.vColor * ambientOcclusion;
    
    Out.vDepthPBR = float4(input.Depth, roughness, metallic, ambientOcclusion);
    
	return Out;
}
