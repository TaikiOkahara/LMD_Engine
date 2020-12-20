#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

//6面テクスチャ
Texture2D g_tex_f : register(t0); 
Texture2D g_tex_b : register(t1);
Texture2D g_tex_u : register(t2);
Texture2D g_tex_d : register(t3);
Texture2D g_tex_r : register(t4);
Texture2D g_tex_l : register(t5);

SamplerState g_samLinear : register(s0);



PS_OUT main(VS_CUBEMAP_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

    
    
    if (input.TexNum == 0)
    {
        Out.vColor = g_tex_f.Sample(g_samLinear, input.Tex);
    }
    if (input.TexNum == 1)
    {
        Out.vColor = g_tex_b.Sample(g_samLinear, input.Tex);
    }
    if (input.TexNum == 2)
    {
        Out.vColor = g_tex_u.Sample(g_samLinear, input.Tex);
    }
    if (input.TexNum == 3)
    {
        Out.vColor = g_tex_d.Sample(g_samLinear, input.Tex);
    }
    if (input.TexNum == 4)
    {
        Out.vColor = g_tex_r.Sample(g_samLinear, input.Tex);
    }
    if (input.TexNum == 5)
    {
        Out.vColor = g_tex_l.Sample(g_samLinear, input.Tex);
    }
        
    
    
    
    
    
    
    Out.vNormal = float4(0, 0, 0, 2);
    
    
	//座標テクスチャ―へ出力
   // Out.vPosition = input.WorldPos;
	
	////ワールド法線テクスチャーへ出力

 //   float4 bump;
 //   bump = g_texNor.Sample(g_samLinear, input.Tex);
 //   bump = (bump * 2.0f) - 1.0f;
	
 //   float3 bumpNormal;
 //   bumpNormal = (-bump.x * input.WorldTangent) + (-bump.y * input.WorldBinormal) + (-bump.z * input.WorldNormal);
 //   bumpNormal = normalize(bumpNormal);
    
    
 //   Out.vNormal = float4(bumpNormal, 0);
	
 //   Out.vMotion = input.Velocity;
    
    
 //   float metallic = g_texMRA.Sample(g_samLinear, input.Tex).r;
 //   float roughness = g_texMRA.Sample(g_samLinear, input.Tex).g;
 //   float ambientOcclusion = g_texMRA.Sample(g_samLinear, input.Tex).b;
    
 //   Out.vDepthPBR = float4(input.Depth, roughness, metallic, ambientOcclusion);
    
    return Out;
}