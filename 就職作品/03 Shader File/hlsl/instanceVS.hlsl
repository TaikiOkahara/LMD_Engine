#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

StructuredBuffer<uint> IndexBuffer : register(t1);



VS_OUT main(
	float4	Pos		: POSITION0,
	float3	Normal	: NORMAL0,
	float2	Tex		: TEXCOORD0,
	float4	Tangent	: TANGENT,
	float4	Binormal: BINORMAL,
	uint	instID	: SV_InstanceID)
{
	VS_OUT output = (VS_OUT)0;

    uint index;
    
    index = (g_FrustumCullingEnable == 0) ? instID : IndexBuffer[instID];
        
    
    matrix world = WorldMatrixBuffer[index];

    matrix wvp;
    wvp = mul(world, g_mView);
    wvp = mul(wvp, g_mProj);

	
    matrix oldWvp;
    oldWvp = mul(world, g_mOldView);
    oldWvp = mul(oldWvp, g_mOldProj);

    output.WorldNormal = mul(Normal, (float3x3) world);
	output.Pos = mul(Pos, wvp);
    output.WorldPos = mul(Pos, world);
    output.WorldTangent = mul(Tangent.xyz, (float3x3)world);
    output.WorldBinormal = mul(Binormal.xyz, (float3x3)world);
	
	output.Tex = Tex;
	
    output.Depth = output.Pos.z / output.Pos.w;
    
    float4 curPos = mul(Pos, wvp);
    float4 lastPos = mul(Pos, oldWvp);

	
    float2 velocity = (curPos.xy / curPos.w) - (lastPos.xy / lastPos.w);
	
    output.Velocity = velocity;
 
	return output;

}

