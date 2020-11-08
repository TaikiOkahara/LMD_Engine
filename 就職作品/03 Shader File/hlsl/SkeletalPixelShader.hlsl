#include "Geometry.hlsl"


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
	
    float4 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
    bump.w = 0.0f;
	
    //float3 bumpNormal;
    //bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
    ////bumpNormal = normalize(bumpNormal);
	
    //float4 normal;
    //normal.x = -bumpNormal.x;
    //normal.y = bumpNormal.y;
    //normal.z = bumpNormal.z;
    //normal.w = 0.0f;
    
    
    //Out.vNormal = float4(bumpNormal, 0);
    
    
    Out.vNormal = bump;
    

    return Out;
}