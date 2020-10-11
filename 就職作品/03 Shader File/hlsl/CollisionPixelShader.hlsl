#include "Geometry.hlsl"


//GBuffer
Texture2D g_texColor : register(t2);
Texture2D g_texNormal : register(t3);
Texture2D g_texPosition : register(t4);


PS_OUT main(VS_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

	//�J���[�e�N�X�`���[�֏o�� 
    Out.vColor = float4(0, 1, 0, 1);
    Out.vNormal = float4(0,0,-1,2); //w���Q�Ȃ烉�C�e�B���O���s��Ȃ��i�V�ˁj
    Out.vPosition = input.WorldPos;
    
    return Out;
}