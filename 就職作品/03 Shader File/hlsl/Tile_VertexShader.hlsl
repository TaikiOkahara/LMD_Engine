#include "FBX_Geometry.hlsl"


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

////　ポイントライト
//cbuffer PointLightBuffer : register(b4)
//{
//	float4  g_vLightPos[10];		//	座標

//	float4	g_fIntensity_point[10];	//　明るさ
//	float4	g_Range[10];			//　範囲
//	float4	g_AttRate[10];			//　減衰率

//	float4	g_fNumLight;			//　ライト総数

//}

//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b5)
{
	float4 g_vLight;//ライトの座標
	//float4 g_fIntensity;//明るさ	
	
	//matrix g_mLightView;
}



VS_OUT main(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
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