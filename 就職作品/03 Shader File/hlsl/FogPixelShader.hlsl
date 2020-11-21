#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);
Texture2D g_texPointLight : register(t3);
Texture2D g_texMotion : register(t4);

Texture2D g_texFog : register(t5);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
    //フォグの揺らぎテクスチャーのテクセルはワールド行列を適応した頂点座標の[ X, Z ]を使用する。
   //そのままでは使用できないのでm_TexScale変数値によって調整する。
   //またテクセル座標をm_Texel変数値によって移動させ、２つの色情報の平均値を最終的なフォグカラーとする
    
    
    if(g_fFogEnable == 0) 
        return float4(0, 0, 0, 1);
    
    
    
    //float3 motion = g_texMotion.Sample(g_samLinear, input.Tex).xyz;
    
    //float4 color = g_texColor.Sample(g_samLinear, input.Tex);
    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
    
    //if (length(motion) != 0)
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    //float2 uv = input.Tex;
    
    
    
    
    float texScale = g_fFogData.x;
    float maxHeight = g_fFogData.y;
    float minHeight = g_fFogData.z;
    
    
    float FogColor = (g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.xy).r +
                      g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.zw).r * 0.2f) * 0.5f; //0.2fはフォグに隙間を持たせる閾値
    
    
    //上下に波立たせるために高さを少しいじる
    float randomHeight = g_texFog.Sample(g_samLinear, g_fFogOffset.xy).r + g_texFog.Sample(g_samLinear, g_fFogOffset.xz).r;
    randomHeight *= 0.5f;
    
   //頂点の高さによってα値を計算
  
    float Alpha = saturate((position.y + randomHeight - minHeight) / (maxHeight - minHeight));
    
    
   
   //フォグの揺らぎ情報によりα値を調整
    Alpha = 1.0f - (1.0f - Alpha) * FogColor;
   
   //オブジェクトの色情報とフォグカラーを線形合成
    float4 fogColor = float4(g_fFogColor, 1);
    float4 Out = fogColor * (1.0f - Alpha);

    return Out;
}