#include "Geometry.hlsl"

cbuffer WorldBuffer : register(b0)
{
    matrix g_mWorld;
    matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1)
{
    matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix g_mProj;
}


VS_OUT main(
float4 Pos : POSITION,
float2 Tex : TEXCOORD0)
{
    VS_OUT output = (VS_OUT) 0;

    matrix WVP;
    WVP = mul(g_mWorld, g_mView);
    WVP = mul(WVP, g_mProj);

    output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, g_mWorld);
    
    return output;
}

