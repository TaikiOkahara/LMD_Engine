#include "Geometry.hlsl"



VS_OUT main(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
	float4 Tangent	: TANGENT)
{
    VS_OUT output = (VS_OUT) 0;

    output.Pos = Pos;
    output.Tex = Tex;
	
	

    return output;
}