#include "Geometry.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

cbuffer ViewBuffer : register(b1) {
	matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2){
	matrix g_mProj;
}

// マテリアルバッファ
struct MATERIAL{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
};



//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b5){
	float4 g_vLight;//ライトの座標
	//float4 g_fIntensity;//明るさ	
	
	//matrix g_mLightView;
}



VS_OUT main(
	float4	Pos		: POSITION0,
	float3	Normal	: NORMAL0,
	float2	Tex		: TEXCOORD0,
	float4	Tangent	: TANGENT,
	float4	Binormal: BINORMAL,
	uint	instID	: SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;

    matrix world = WorldMatrixBuffer[instID];

	matrix WVP;
    WVP = mul(world, g_mView);
	WVP = mul(WVP, g_mProj);

    output.WorldNormal = mul(Normal, (float3x3) world);
	output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, world);
    output.WorldTangent = mul(Tangent.xyz, (float3x3)world);
    output.WorldBinormal = mul(Binormal.xyz, (float3x3)world);
	
	output.Tex = Tex;
	output.LightVector = normalize(g_vLight).xyz;
	//output.ShadowPos = Pos;

	return output;

}

