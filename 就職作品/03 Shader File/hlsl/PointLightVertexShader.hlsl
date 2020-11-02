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

    output.LightIndex = g_mWorld._33;//indexはscaleのz値に入れいている

    matrix world = g_mWorld;
    world._33 = world._22 = world._11;//scaleはｘ座標に入っている値をyzにも反映させる

	matrix WVP;
    WVP = mul(world, g_mView);
	WVP = mul(WVP, g_mProj);

	output.Pos = mul(Pos, WVP);
    output.LightPos = float3(world._41, world._42, world._43); //マトリクスから位置座標を取得
    output.LightRange = float3(world._11, world._11, world._11);
	
	return output;

}
