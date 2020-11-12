#define ANIMATION_MATRIX_MAX (256)
#define LIGHT_NUM (128)




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


struct POINT
{
    float3 color;
    float intensity;
    float3 calc;
    float specular;
};

cbuffer PointLightBuffer : register(b5)
{
    POINT g_vPointLight[LIGHT_NUM];
}


cbuffer EffectBuffer : register(b6)
{
  
    float2 g_fFogOffset0; //�t�H�O�̗h�炬�e�N�X�`���[�̃e�N�Z���̃I�t�Z�b�g�l
    float2 g_fFogOffset1; //�t�H�O�̗h�炬�e�N�X�`���[�̃e�N�Z���̃I�t�Z�b�g�l
    float g_fFogTexScale; //�e�N�Z���̃X�P�[�����O�l�B
    float g_fFogMaxHeight; //���_�̍����̍ő�l�B���_�̍��������̒l���傫���Ȃ�ƃt�H�O��������Ȃ��Ȃ�B
    float g_fFogMinHeight; //���_�̍����̍ŏ��l�B���_�̍��������̒l��菬�����Ȃ�ƃt�H�O�J���[�œh��Ԃ����B
    float g_fFogDummy0;
    
    float3 g_fFogColor;
    float g_fFogDummy1;
}

cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[ANIMATION_MATRIX_MAX];
}