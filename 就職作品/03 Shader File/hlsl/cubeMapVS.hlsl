#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_CUBEMAP_OUT main(
	float4 Pos : POSITION0,
	float4 Normal : NORMAL0,
	float2 Tex : TEXCOORD0,
	float4 Tangent : TANGENT,
	float4 Binormal : BINORMAL)
{
    VS_CUBEMAP_OUT output = (VS_CUBEMAP_OUT) 0;


    matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);

	
    output.Normal = Normal;
    output.Pos = mul(Pos, wvp);
    output.Tex = Tex;
	
    output.TexNum = Binormal.x;
    
    return output;
}

