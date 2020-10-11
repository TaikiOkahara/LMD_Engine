#include "../03 Shader File/FBX_Geometry.hlsl"

cbuffer WorldBuffer : register(b0) {
	matrix g_mWorld;
	matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1) {
	matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2) {
	matrix g_mProj;
}


cbuffer EyeBuffer : register(b3) {
	float4 g_vEye;
}

//�@�|�C���g���C�g
cbuffer PointLightBuffer : register(b4)
{
	float4  g_vLightPos[10];		//	���W

	float4	g_fIntensity_point[10];	//�@���邳
	float4	g_Range[10];			//�@�͈�
	float4	g_AttRate[10];			//�@������

	float4	g_fNumLight;			//�@���C�g����

	//float Dummy;
}

//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b5)
{
	float4 g_vLight;//���C�g�̍��W
	float4 g_fIntensity;//���邳	
	//matrix g_mWLP;
	//matrix g_mWLPT;
	matrix g_mLightView;
}

cbuffer MaterialBuffer : register(b6)
{
	float4 g_Ambient;//�A���r�G���g��
	float4 g_Diffuse;//�g�U����(�F�j
	float4 g_Specular;//���ʔ���
}


//
//�[�x�e�N�X�`���[�p�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT_DEPTH VS_Depth(
	float4 Pos		: POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;


	matrix WLP;
	WLP = mul(g_mWorld, g_mLightView);
	WLP = mul(WLP, g_mProj);

	output.Pos = mul(Pos, WLP);
	output.Depth = output.Pos;

	return output;
}