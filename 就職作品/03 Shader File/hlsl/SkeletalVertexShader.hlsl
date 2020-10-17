#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


// マテリアルバッファ
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy; //16bit境界用
};


cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[256];
}


matrix GetAnimMatrix(uint index)
{
    return g_Animation[index];
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
    normal = float4(0, 0, 0, 0);
    binormal = float4(0, 0, 0, 0);
    tangent = float4(0, 0, 0, 0);
    
    weight = BoneWeight;
    index = BoneIndex;
    
    //アニメーションマトリクス計算----------------
    //
    mat = GetAnimMatrix(index.x);
    position = mul(Pos, mat * weight.x);
    normal = mul(Normal, mat * weight.x);
    binormal = mul(Binormal, mat * weight.x);
    tangent = mul(Tangent, mat * weight.x);
    //
    mat = GetAnimMatrix(index.y);
    position += mul(Pos, mat * weight.y);
    normal += mul(Normal, mat * weight.y);
    binormal += mul(Binormal, mat * weight.y);
    tangent += mul(Tangent, mat * weight.y);
    //
    mat = GetAnimMatrix(index.z);
    position += mul(Pos, mat * weight.z);
    normal += mul(Normal, mat * weight.z);
    binormal += mul(Binormal, mat * weight.z);
    tangent += mul(Tangent, mat * weight.z);
    //
    mat = GetAnimMatrix(index.w);
    position += mul(Pos, mat * weight.w);
    normal += mul(Normal, mat * weight.w);
    binormal += mul(Binormal, mat * weight.w);
    tangent += mul(Tangent, mat * weight.w);
    
    //--------------------------------------------
    
    
    matrix WVP;
    WVP = mul(g_mWorld, g_mView);
    WVP = mul(WVP, g_mProj);
    
    
    output.Pos = mul(position, WVP);
    output.WorldNormal = mul(normal.xyz, (float3x3)g_mWorld);
    
    output.WorldPos = mul(position, g_mWorld);
    output.WorldTangent = mul(tangent.xyz, (float3x3)g_mWorld);
    output.WorldBinormal = mul(binormal.xyz, (float3x3) g_mWorld);
	
    output.Tex = Tex;
	//output.ShadowPos = Pos;


    return output;

}

