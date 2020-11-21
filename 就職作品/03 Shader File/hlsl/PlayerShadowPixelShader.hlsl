#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"



PS_OUT main(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	
    float3 lightPos = float3(g_mWIT._41, g_mWIT._42, g_mWIT._43);
    float dist = length(input.WorldPos.xyz - lightPos);
    dist = abs(dist);
    float lightRange = g_mWIT._31;
    
    
    float att = max(0, 1.0f - (dist / lightRange));
	//カラーテクスチャーへ出力 
    Out.vColor = float4(1, 0, 0, 1);//float4(0.5, 0.5, 0.5, 0.5f);
	
    //if (dist < lightRange)
    //{
    //    Out.vColor = float4(0, 0, 0, 0);
    //}

	
	//座標テクスチャ―へ出力
    //Out.vPosition = input.WorldPos;
	
	//ワールド法線テクスチャーへ出力

 //   float4 bump;
	//bump = g_texNor.Sample(g_samLinear, input.Tex);
 //   bump = (bump * 2.0f) - 1.0f;
	
 //   float3 bumpNormal;
 //   bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (bump.z * input.WorldNormal);
   
    //float4 normal;
    //normal.x = -bumpNormal.x;
    //normal.y = bumpNormal.y;
    //normal.z = bumpNormal.z;
    //normal.w = 0.0f;
	
    //normal = normalize(normal);
    //Out.vNormal = normal;
	
    //Out.vMotion = float4(input.MotionDir, 1);

	return Out;
}