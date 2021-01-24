#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_OUT main(
float4 Pos : POSITION)
{
    VS_OUT output = (VS_OUT) 0;

    matrix WVP;
    WVP = mul(g_mWorld, g_mView);
    WVP = mul(WVP, g_mProj);

    output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, g_mWorld);
    
    return output;
}