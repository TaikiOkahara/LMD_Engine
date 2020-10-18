#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_LIGHT_OUT main(
	float4 Pos		: POSITION0,
	float4 Normal	: NORMAL0,
	float2 Tex		: TEXCOORD0,
	float4 Tangent	: TANGENT,
	float4 Binormal : BINORMAL)
{
    VS_LIGHT_OUT output = (VS_LIGHT_OUT) 0;

    

	matrix WVP;
    WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.Pos = mul(Pos, WVP);
    output.LightPos = float3(g_mWorld._41, g_mWorld._42, g_mWorld._43); //マトリクスから位置座標を取得
    output.LightRange = float3(g_mWorld._11/100, g_mWorld._22/100, g_mWorld._33/100);
	
	return output;

}
