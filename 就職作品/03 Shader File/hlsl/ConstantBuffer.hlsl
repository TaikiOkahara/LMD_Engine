cbuffer WorldBuffer : register(b0)
{
    matrix g_mWorld;
    matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1)
{
    matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix g_mProj;
}

cbuffer EyeBuffer : register(b3)
{
    float4 g_vEye;
}

//�@�|�C���g���C�g
cbuffer PointLightBuffer : register(b4)
{
    float4 g_vPointLight[10];
}


//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b5)
{
    float4 g_vLight; //���C�g�̍��W
	//float4 g_fIntensity;//���邳	
	
	//matrix g_mLightView;
}

//
//register7�̓A�j���[�V�����R���X�^���g�o�b�t�@