#include "Geometry.hlsl"


Texture2D g_texDif		: register(t0);//Diffuseテクスチャ
Texture2D g_texNor		: register(t1);//Normalテクスチャ


SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	//カラーテクスチャーへ出力 
    Out.vColor = g_texDif.Sample(g_samLinear, input.Tex);//+float4(0.01, 0.01, 0.01, 0);
  
	
	//座標テクスチャ―へ出力
    Out.vPosition = input.WorldPos;
	
	//ワールド法線テクスチャーへ出力

    float4 bump;
	bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (bump.z * input.WorldNormal);
   
    float4 normal;
    normal.x = -bumpNormal.x;
    normal.y = bumpNormal.y;
    normal.z = bumpNormal.z;
    normal.w = 0.0f;
	
    normal = normalize(normal);
    Out.vNormal = normal;
	

	return Out;
}