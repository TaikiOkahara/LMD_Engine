#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


Texture2D g_texNormal : register(t1);
Texture2D g_texPosition : register(t2);

Texture2D g_texFog : register(t7);


SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);


float4 main(VS_OUT input) : SV_Target
{
    //フォグの揺らぎテクスチャーのテクセルはワールド行列を適応した頂点座標の[ X, Z ]を使用する。
   //またテクセル座標をm_Texel変数値によって移動させ、２つの色情報の平均値を最終的なフォグカラーとする
    
    
    if(g_fFogEnable == 0) 
        discard;
    
    float4 normal = g_texNormal.Sample(g_samLinear, input.Tex);
    if(normal.w == 2)
        discard;
    

    float4 position = g_texPosition.Sample(g_samLinear, input.Tex);
   
    float texScale = g_fFogData.x;
    float maxHeight = g_fFogData.y;
    float minHeight = g_fFogData.z;
    
    
    if (position.y >= maxHeight)
        discard;
    
        
        
    float fogColor = (g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.xy).r +
                      g_texFog.Sample(g_samLinear, position.xz * texScale + g_fFogOffset.zw).r * 0.2f) * 0.5f; //0.2fはフォグに隙間を持たせる閾値
    
    
    //上下に波立たせるために高さを少しいじる
    float randomHeight = g_texFog.Sample(g_samLinear, g_fFogOffset.xy).r + g_texFog.Sample(g_samLinear, g_fFogOffset.xz).r;
    randomHeight *= 0.5f;
    
   //頂点の高さによってα値を計算
  
    float alpha = saturate((position.y + randomHeight - minHeight) / (maxHeight - minHeight));
    
    
   
   //フォグの揺らぎ情報によりα値を調整
    alpha = 1.0f - (1.0f - alpha) * fogColor;
   
   //オブジェクトの色情報とフォグカラーを線形合成
    float4 Out = float4(g_fFogColor, 1);
    Out = Out * (1.0f - alpha);

    
    
    
    
    return Out;
}