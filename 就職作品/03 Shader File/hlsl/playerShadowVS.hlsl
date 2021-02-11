#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



VS_SHADOW_OUT main(
	float4 Pos      : POSITION0,
	float4 Normal   : NORMAL0,
	float2 Tex      : TEXCOORD0,
	float4 Tangent  : TANGENT,
	float4 Binormal : BINORMAL)
{
    VS_SHADOW_OUT output = (VS_SHADOW_OUT) 0;


    matrix wvp;
    wvp = mul(g_mWorld, g_mView);
    wvp = mul(wvp, g_mProj);
	
    output.Pos = mul(Pos, wvp);
    output.WorldPos = mul(Pos, g_mWorld);
    output.CenterPos = float3(g_mWorld._41, g_mWorld._42, g_mWorld._43);
    output.Tex = Tex;
	
    int index = g_vPointLight.index;
    output.ProjectorSpacePos_1 = mul(Pos, mul(g_mWorld, g_vPointLightViewProj[0]));
    output.ProjectorSpacePos_2 = mul(Pos, mul(g_mWorld, g_vPointLightViewProj[1]));
    output.ProjectorSpacePos_3 = mul(Pos, mul(g_mWorld, g_vPointLightViewProj[2]));
    
    //射影テクスチャマッピング
    matrix mt;
    mt._11_12_13_14 = float4(0.5f, 0.0f, 0.0f, 0.0f);
    mt._21_22_23_24 = float4(0.0f, -0.5f, 0.0f, 0.0f);
    mt._31_32_33_34 = float4(0.0f, 0.0f, 1.0f, 0.0f);
    mt._41_42_43_44 = float4(0.5f, 0.5f, 0.0f, 1.0f);
    
    output.ProjectorSpacePos_1 = mul(output.ProjectorSpacePos_1, mt);
    output.ProjectorSpacePos_2 = mul(output.ProjectorSpacePos_2, mt);
    output.ProjectorSpacePos_3 = mul(output.ProjectorSpacePos_3, mt);
    
    return output;

}

