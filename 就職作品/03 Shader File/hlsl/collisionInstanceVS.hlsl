#include "geometry.hlsl"
#include "constantBuffer.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);
StructuredBuffer<uint> IndexBuffer : register(t1);

VS_OUT main(
float4 Pos : POSITION,
uint instID : SV_InstanceID)
{
    VS_OUT output = (VS_OUT) 0;

    uint index = IndexBuffer[instID];
    
    matrix world = WorldMatrixBuffer[index];

    matrix WVP;
    WVP = mul(world, g_mView);
    WVP = mul(WVP, g_mProj);

    output.Pos = mul(Pos, WVP);
    output.WorldPos = mul(Pos, world);
    
    return output;
}

