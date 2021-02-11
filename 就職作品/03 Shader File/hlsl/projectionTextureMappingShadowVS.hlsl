#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


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
    uint4 BoneIndex : BONEINDEX,
    float4 BoneWeight : BONEWEIGHT)
{
    VS_OUT output = (VS_OUT) 0;

    
    float4 position;
    matrix mat;
    uint4 index;
    float4 weight;
    
    position = float4(0, 0, 0, 0);
    
    
    weight = BoneWeight;
    index = BoneIndex;
    
    //アニメーションマトリクス計算----------------
    //
    mat = GetAnimMatrix(index.x);
    position += mul(Pos, mat) * weight.x;
    //
    mat = GetAnimMatrix(index.y);
    position += mul(Pos, mat) * weight.y;
    //
    mat = GetAnimMatrix(index.z);
    position += mul(Pos, mat) * weight.z;
    //
    mat = GetAnimMatrix(index.w);
    position += mul(Pos, mat) * weight.w;
    
    
    int lightIndex = g_vPointLight.index;
    matrix wlvp;
    wlvp = mul(g_mWorld, g_vPointLightViewProj[lightIndex]);
    
    
    output.Pos = mul(position,wlvp);
    output.Tex = Tex;
	

    return output;
}