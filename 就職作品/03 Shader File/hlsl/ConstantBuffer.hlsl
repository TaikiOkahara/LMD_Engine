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


//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b4)
{
    float4 g_vLight; //���C�g�̍��W
	float4 g_vLightColor;//�F	
	
	//matrix g_mLightView;
}

#define LIGHT_NUM (128)
cbuffer PointLightBuffer : register(b5)
{
    //float g_vPointIntensity[LIGHT_NUM];
    //float g_vPointRange[LIGHT_NUM];
    //float g_vPointAttenuation[LIGHT_NUM];
    //float3 g_vPointColor[LIGHT_NUM];
    
    
    float4 g_vPointColor[LIGHT_NUM];//xyz�͐F�Aw�͌��̋���
    float4 g_vPoint[LIGHT_NUM];//x�͌��͈̔�y�͌����x,zw�̓_�~�[
    
    //float2 dummy;
}

//
//register7�̓A�j���[�V�����R���X�^���g�o�b�t�@