#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texColor : register(t0);
Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

Texture2D g_texFog : register(t3);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
    //フォグの揺らぎテクスチャーのテクセルはワールド行列を適応した頂点座標の[ X, Z ]を使用する。
   //そのままでは使用できないのでm_TexScale変数値によって調整する。
   //またテクセル座標をm_Texel変数値によって移動させ、２つの色情報の平均値を最終的なフォグカラーとする
    float4 color = g_texColor.Sample(g_samLinear, input.Tex);
    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
    
    //return (position.y < 1.0f) ? float4(1, 1, 1, 1) : float4(0, 0, 0, 1);
   
    //float m_TexScale = 0.01f; 
    //float2 m_Texel[2] = {1,1,1,1}; 
    //float m_MaxHeight = 10.0f; 
    //float m_MinHeight = 2.0f; 
    
    float FogColor = (g_texFog.Sample(g_samLinear, position.xz * g_fFogTexScale + g_fFogOffset0).r +
                      g_texFog.Sample(g_samLinear, position.xz * g_fFogTexScale + g_fFogOffset1).r) * 0.5f;
   
    //上下に波立たせるために高さを少しいじる
    float randomHeight = g_texFog.Sample(g_samLinear, input.Tex).r;
    
   //頂点の高さによってα値を計算する。
   //頂点座標 >= m_MaxHeight のとき 1.0f
   //頂点座標 <= m_MinHeight のとき 0.0f
    float Alpha = saturate((position.y - g_fFogMinHeight) / (g_fFogMaxHeight - g_fFogMinHeight));
    
    
   
   //フォグの揺らぎ情報によりα値を調整する。
    Alpha = 1.0f - (1.0f - Alpha) * FogColor;
   
   //オブジェクトの色情報とフォグカラーを線形合成する
    float4 fogColor = float4(g_fFogColor, 1);
    float4 Out = fogColor * (1.0f - Alpha);
    //float4 Out = In.Col * tex2D(tex0, In.Tex) * Alpha + m_FogColor * (1.0f - Alpha);

    return Out;
}