#include "Geometry.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);


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
uint instID : SV_InstanceID)
{
    VS_OUT output = (VS_OUT) 0;

    matrix world = WorldMatrixBuffer[instID];

    matrix WVP;
    WVP = mul(world, g_mView);
    WVP = mul(WVP, g_mProj);

    output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, world);
    
    return output;
}

