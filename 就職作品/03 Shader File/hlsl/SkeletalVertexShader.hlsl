#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"






matrix GetAnimMatrix(uint index)
{
    return g_Animation[index];
  
    
    //インデックスが要素数最大になっていたら、そのボーンウェイトは影響がないため、正規化のマトリクスを返す
    //return (index == ANIMATION_MATRIX_MAX - 1) ? mat : g_Animation[index];
}


VS_OUT main(
	float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0,
	float4 Tangent : TANGENT,
	float4 Binormal : BINORMAL,
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
    normal += mul(Normal, mat * weight.x);
    binormal += mul(Binormal, mat * weight.x);
    tangent += mul(Tangent, mat * weight.x);
    //
    mat = GetAnimMatrix(index.y);
    position += mul(Pos, mat) * weight.y;
    normal += mul(Normal, mat) * weight.y;
    binormal += mul(Binormal, mat * weight.y);
    tangent += mul(Tangent, mat * weight.y);
    //
    mat = GetAnimMatrix(index.z);
    position += mul(Pos, mat) * weight.z;
    normal += mul(Normal, mat * weight.z);
    binormal += mul(Binormal, mat * weight.z);
    tangent += mul(Tangent, mat * weight.z);
    //
    mat = GetAnimMatrix(index.w);
    position += mul(Pos, mat) * weight.w;
    normal += mul(Normal, mat * weight.w);
    binormal += mul(Binormal, mat * weight.w);
    tangent += mul(Tangent, mat * weight.w);
    
    //--------------------------------------------
    //position = Pos;
    
    matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);
    
    matrix oldWvp;
    oldWvp = mul(g_mWorld, g_mOldView);
    oldWvp = mul(oldWvp, g_mOldProj);
    
    
    output.Pos = mul(position, wvp);
    output.WorldNormal = mul(normal.xyz, (float3x3)g_mWorld);  
    output.WorldPos = mul(position, g_mWorld);
    output.WorldTangent = mul(tangent.xyz, (float3x3)g_mWorld);
    output.WorldBinormal = mul(binormal.xyz, (float3x3) g_mWorld);
    output.Tex = Tex;
	
    output.Depth = output.Pos.z / output.Pos.w;
    
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

