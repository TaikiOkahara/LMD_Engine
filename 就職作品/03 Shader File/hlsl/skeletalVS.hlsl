#include "geometry.hlsl"
#include "constantBuffer.hlsl"






matrix GetAnimMatrix(uint index)
{
    return g_Animation[index];
}


VS_OUT main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD0,
	float3 Normal : NORMAL,
	float3 Tangent : TANGENT,
	float3 Binormal : BINORMAL,
    uint4  BoneIndex : BONEINDEX,
    float4 BoneWeight : BONEWEIGHT)
{
    VS_OUT output = (VS_OUT) 0;


    

    float4 position,normal,binormal,tangent;
    matrix mat;
    uint4 index;
    float4 weight;
    
    position = float4(0,0,0,0);
    normal = float4(0,0,0,0);
    binormal = float4(0,0,0,0);
    tangent = float4(0,0,0,0);
    
    weight = BoneWeight;
    index = BoneIndex;
    
    //アニメーションマトリクス計算----------------
    //
    mat = GetAnimMatrix(index.x);
    position += mul(Pos, mat) * weight.x;
    normal += mul(float4(Normal.xyz,0), mat) * weight.x;
    binormal += mul(float4(Binormal.xyz, 0), mat) * weight.x;
    tangent += mul(float4(Tangent.xyz, 0), mat) * weight.x;
    //
    mat = GetAnimMatrix(index.y);
    position += mul(Pos, mat) * weight.y;
    normal += mul(float4(Normal.xyz, 0), mat) * weight.y;
    binormal += mul(float4(Binormal.xyz, 0), mat) * weight.y;
    tangent += mul(float4(Tangent.xyz, 0), mat) * weight.y;
    //
    mat = GetAnimMatrix(index.z);
    position += mul(Pos, mat) * weight.z;
    normal += mul(float4(Normal.xyz, 0), mat) * weight.z;
    binormal += mul(float4(Binormal.xyz, 0), mat) * weight.z;
    tangent += mul(float4(Tangent.xyz, 0), mat) * weight.z;
    //
    mat = GetAnimMatrix(index.w);
    position += mul(Pos, mat) * weight.w;
    normal += mul(float4(Normal.xyz, 0), mat) * weight.w;
    binormal += mul(float4(Binormal.xyz, 0), mat) * weight.w;
    tangent += mul(float4(Tangent.xyz, 0), mat) * weight.w;
    
    //--------------------------------------------
  
    
    matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);
    
    matrix oldWvp;
    oldWvp = mul(g_mWorld, g_mOldView);
    oldWvp = mul(oldWvp, g_mOldProj);
    
    
    output.Pos = mul(position, wvp);
    output.WorldPos = mul(position, g_mWorld);
    output.WorldNormal = normalize(mul(normal.xyz, (float3x3) g_mWorld));
    output.WorldTangent = normalize(mul(tangent.xyz, (float3x3) g_mWorld));
    output.WorldBinormal = normalize(mul(binormal.xyz, (float3x3) g_mWorld));
    output.Tex = Tex;
	
    output.DepthZW = output.Pos.zw;
    
    float4 curPos = mul(Pos, wvp);
    float4 lastPos = mul(Pos, oldWvp);

	
    float2 velocity = (curPos.xy / curPos.w) - (lastPos.xy / lastPos.w);
	
    output.Velocity = velocity;

    return output;

}

