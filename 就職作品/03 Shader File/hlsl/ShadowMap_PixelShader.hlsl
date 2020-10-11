#include "../03 Shader File/FBX_Geometry.hlsl"


//
//深度テクスチャー用ピクセルシェーダー
//
float4  PS_Depth(VS_OUTPUT_DEPTH input) : SV_Target
{
	float4 color = input.Depth.z / input.Depth.w;
	return color;
}