#include "FBX_Geometry.hlsl"

cbuffer WorldBuffer : register(b0){
	matrix g_mWorld;
	matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1) {
	matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2){
	matrix g_mProj;
}

// �}�e���A���o�b�t�@
struct MATERIAL{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit���E�p
};

////�@�|�C���g���C�g
//cbuffer PointLightBuffer : register(b4){
//	float4  g_vLightPos[10];		//	���W

//	float4	g_fIntensity_point[10];	//�@���邳
//	float4	g_Range[10];			//�@�͈�
//	float4	g_AttRate[10];			//�@������

//	float4	g_fNumLight;			//�@���C�g����

//	//float Dummy;
//}

//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b5){
	float4 g_vLight;//���C�g�̍��W
	//float4 g_fIntensity;//���邳	
	
	//matrix g_mLightView;
}



VS_OUT main(
	float4 Pos		: POSITION0,
	float4 Normal	: NORMAL0,
	float2 Tex		: TEXCOORD0,
	float4 Tangent	: TANGENT,
	float4 Binormal	: BINORMAL)
{
	VS_OUT output = (VS_OUT)0;


	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.WorldNormal = normalize(mul(Normal.xyz, (float3x3)g_mWorld));
	output.Pos = mul(Pos, WVP);
	output.WorldPos = mul(Pos, g_mWorld);
    output.WorldTangent = mul(Tangent.xyz, (float3x3)g_mWorld);
    output.WorldBinormal = mul(Binormal.xyz, (float3x3)g_mWorld);
	
	output.Tex = Tex;
	output.LightVector = normalize(g_vLight).xyz;
	//output.ShadowPos = Pos;


	return output;

}

