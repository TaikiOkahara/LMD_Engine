#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_OUT main(
	float4 Pos		: POSITION0,
	float4 Normal	: NORMAL0,
	float2 Tex		: TEXCOORD0,
	float4 Tangent	: TANGENT,
	float4 Binormal	: BINORMAL)
{
	VS_OUT output = (VS_OUT)0;


	matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);

	
    matrix oldWvp;
    oldWvp = mul(g_mWorld, g_mOldView);
    oldWvp = mul(oldWvp, g_mOldProj);
	
	
	output.WorldNormal = normalize(mul(Normal.xyz, (float3x3)g_mWorld));
    output.Pos = mul(Pos, wvp);
	output.WorldPos = mul(Pos, g_mWorld);
    output.WorldTangent = normalize(mul(Tangent.xyz, (float3x3) g_mWorld));
    output.WorldBinormal = normalize(mul(Binormal.xyz, (float3x3) g_mWorld));
	output.Tex = Tex;
	
    output.DepthZW = output.Pos.zw;
	
	
    float4 curPos = mul(Pos, wvp);
    float4 lastPos = mul(Pos, oldWvp);

	
    float2 velocity = (curPos.xy / curPos.w) - (lastPos.xy / lastPos.w);
	
    output.Velocity = velocity;
 
	
	return output;
}

