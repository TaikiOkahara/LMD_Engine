#include "FBX_Geometry.hlsl"


Texture2D g_texDif : register(t0); //Diffuse�e�N�X�`��
Texture2D g_texNor : register(t1); //Normal�e�N�X�`��

//GBuffer
Texture2D g_texColor : register(t2);
Texture2D g_texNormal : register(t3);
Texture2D g_texPosition : register(t4);

SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

	//�J���[�e�N�X�`���[�֏o�� 
    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex); //+float4(0.01, 0.01, 0.01, 0);
  
	//���W�e�N�X�`���\�֏o��
    Out.vPosition = input.WorldPos;
	
	//���[���h�@���e�N�X�`���[�֏o��
	//float3 vNormal = input.WorldNormal;
    //vNormal = vNormal * 0.5 + 0.5;
	
    float4 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (bump.x * input.WorldTangent) + (bump.y * input.WorldBinormal) + (bump.z * input.WorldNormal);
    bumpNormal = normalize(bumpNormal);
	
    //Out.vNormal = float4(bumpNormal, 0);//�|�C���g���C�g�����a����������
    Out.vNormal = bump;
    //Out.vNormal = normalize(float4(input.WorldNormal, 0));
	
	//float  NL = saturate(dot(input.WorldNormal, input.LightVector));
	//NL = NL * 0.9f + 0.1f;
	//Out.vColor *= NL;


	

	//���[���h���W�e�N�X�`���[�֏o��
	//Out.vPosition = input.ShadowPos;


    return Out;
}