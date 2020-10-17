#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


// マテリアルバッファ
struct MATERIAL{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
};



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

