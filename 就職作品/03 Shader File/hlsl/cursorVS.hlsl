#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_OUT main(
	float4 Pos		: POSITION0,
	float4 Normal	: NORMAL0,
	float2 Tex		: TEXCOORD0,
	float4 Tangent	: TANGENT,
	float4 Binormal	: BINORMAL)
{
	VS_OUT output = (VS_OUT)0;


	matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);

	
	output.WorldNormal = normalize(mul(Normal.xyz, (float3x3)g_mWorld));
    output.Pos = mul(Pos, wvp);
	output.WorldPos = mul(Pos, g_mWorld);
	
	return output;
}

