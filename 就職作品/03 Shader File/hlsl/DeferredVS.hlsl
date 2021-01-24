#include "Geometry.hlsl"



VS_OUT main(
	float4 Pos		: POSITION,
    float2 Tex		: TEXCOORD)
{
    VS_OUT output = (VS_OUT) 0;

    output.Pos = Pos;
    output.Tex = Tex;
	
    return output;
}