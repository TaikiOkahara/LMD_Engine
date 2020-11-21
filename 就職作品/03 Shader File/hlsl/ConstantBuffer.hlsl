#define ANIMATION_MATRIX_MAX (64)
#define LIGHT_NUM (128)




cbuffer WorldBuffer : register(b0)
{
    matrix g_mWorld;
    matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1)
{
    matrix g_mView;
    matrix g_mOldView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix g_mProj;
    matrix g_mOldProj;
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
    float4 g_fDeferred;//x�Ń����_�����O�o�b�t�@�ύX�i�Z�b�g�ԍ��Ƌ��ʁj
    
    
    //Fog
    float4 g_fFogOffset; //�t�H�O�̗h�炬�e�N�X�`���[�̃e�N�Z���̃I�t�Z�b�g�l x 2 
    float4 g_fFogData; //x :�e�N�Z���̃X�P�[�����O�l y :���_�̍����̍ő�l z : ���_�̍����̍ŏ��l w : �_�~�[
  
    float3 g_fFogColor;
    float g_fFogEnable;
    
    
}

cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[ANIMATION_MATRIX_MAX];
}