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
//
//
VS_OUT_OUTLINE VS_OutLine(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
	float4 Tangent	: TANGENT)
{
	VS_OUT_OUTLINE output = (VS_OUT_OUTLINE)0;

	//アウトラインのための拡大サンプル
	matrix Exp;
	Exp._11 = 1.1f;
	Exp._12 = 0.0f;
	Exp._13 = 0.0f;
	Exp._14 = 0.0f;

	Exp._21 = 0.0f;
	Exp._22 = 1.1f;
	Exp._23 = 0.0f;
	Exp._24 = 0.0f;

	Exp._31 = 0.0f;
	Exp._32 = 0.0f;
	Exp._33 = 1.1f;
	Exp._34 = 0.0f;

	Exp._41 = 0.0f;
	Exp._42 = 0.0f;
	Exp._43 = 0.0f;
	Exp._44 = 1.0f;



	matrix ExpWVP = mul(Exp, g_mWorld);

	ExpWVP = mul(ExpWVP, g_mView);
	ExpWVP = mul(ExpWVP, g_mProj);

	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	float4 Outline = Pos + float4(Normal.xyz, 0) * 3;
	output.Pos = mul(Outline, WVP);
	//output.Tex = Tex;

	return output;
}