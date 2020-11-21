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

    uint index = IndexBuffer[instID];
    
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
    //output.Depth = mul(g_mView,output.WorldPos).z;
    
    float4 curPos = mul(Pos, wvp);
    float4 lastPos = mul(Pos, oldWvp);

	
    float2 velocity = (curPos.xy / curPos.w) - (lastPos.xy / lastPos.w);
	
	// The velocity is now between (-2,2) so divide by 2 to get it to (-1,1)
    velocity /= 2.0f;
	
    output.MotionDir = float3(velocity, 0);
    //float3 motionDir = output.Pos.xyz - output.MotionDir;
    //output.MotionDir = motionDir;

	return output;

}

