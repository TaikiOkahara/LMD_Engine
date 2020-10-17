#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

VS_OUT main(
	float4 Pos		: POSITION0,
	float4 Normal	: NORMAL0,
	float2 Tex		: TEXCOORD0,
	float4 Tangent	: TANGENT,
	float4 Binormal : BINORMAL)
{
	VS_OUT output = (VS_OUT)0;


	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.Pos = mul(Pos, WVP);
	
	return output;

}

