#include "Geometry.hlsl"


Texture2D g_tex			: register(t0);
Texture2D g_texNor		: register(t1);
//GBuffer
Texture2D g_texColor	: register(t2);
Texture2D g_texNormal	: register(t3);
Texture2D g_texPosition	: register(t4);


SamplerState g_samLinear : register(s0);



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	//カラーテクスチャーへ出力 
	Out.vColor = g_tex.Sample(g_samLinear, input.Tex) + float4(0.01, 0.01, 0.01, 0);

	//座標テクスチャ―へ出力
    Out.vPosition = input.WorldPos;
	
	
	//ワールド法線テクスチャーへ出力
    float4 bump;
    bump = g_texNor.Sample(g_samLinear, input.Tex);
    bump = (bump * 2.0f) - 1.0f;
	
    float3 bumpNormal;
    bumpNormal = (bump.x * input.WorldTangent) + (bump.y * input.WorldBinormal) + (bump.z * input.WorldNormal);
    bumpNormal = normalize(bumpNormal);
	
    Out.vNormal = normalize(float4(bumpNormal, 0));
    //Out.vNormal = normalize(float4(bump.xyz, 0)); //光反射しすぎて見づらかったからバンプマッピングだけにした
	
	//Out.vNormal = normalize(float4(input.WorldNormal, 0));


	return Out;
}
