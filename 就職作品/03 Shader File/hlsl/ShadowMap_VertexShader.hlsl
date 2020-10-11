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

//　ポイントライト
cbuffer PointLightBuffer : register(b4)
{
	float4  g_vLightPos[10];		//	座標

	float4	g_fIntensity_point[10];	//　明るさ
	float4	g_Range[10];			//　範囲
	float4	g_AttRate[10];			//　減衰率

	float4	g_fNumLight;			//　ライト総数

	//float Dummy;
}

//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b5)
{
	float4 g_vLight;//ライトの座標
	float4 g_fIntensity;//明るさ	
	//matrix g_mWLP;
	//matrix g_mWLPT;
	matrix g_mLightView;
}

cbuffer MaterialBuffer : register(b6)
{
	float4 g_Ambient;//アンビエント光
	float4 g_Diffuse;//拡散反射(色）
	float4 g_Specular;//鏡面反射
}


//
//深度テクスチャー用バーテックスシェーダー
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