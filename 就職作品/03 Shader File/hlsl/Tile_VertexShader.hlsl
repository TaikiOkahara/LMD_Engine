#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


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