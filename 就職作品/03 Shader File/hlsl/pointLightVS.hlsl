#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_PL_OUT main(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
	float4 Tangent	: TANGENT,
	float4 Binormal : BINORMAL)
{
    VS_PL_OUT output = (VS_PL_OUT) 0;

	matrix WVP;
    WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.Pos = mul(Pos, WVP);
    output.WorldNormal = mul(Normal, g_mWorld).xyz;
    output.Tex = Tex;
	
	return output;
}
