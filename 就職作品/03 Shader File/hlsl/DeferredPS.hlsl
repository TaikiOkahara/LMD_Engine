#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"
#include "lighting.hlsli"

//GBuffer
Texture2D g_texColor	: register(t0);
Texture2D g_texNormal	: register(t1);
Texture2D g_texPosition	: register(t2);
Texture2D g_texPointLight: register(t3);
Texture2D g_texMotion	: register(t4);
Texture2D g_texDepthPBR : register(t5);
Texture2D g_texShadow : register(t6);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);



float4 main(VS_OUT input) : SV_Target
{
  
    float4 Color = float4(0, 0, 0, 1);
    
    
    float4 diffuse = g_texColor.Sample(g_samDeferredLinear, input.Tex);
    float4 normal = g_texNormal.Sample(g_samDeferredLinear, input.Tex);
    float4 position = g_texPosition.Sample(g_samDeferredLinear, input.Tex);
    float4 pointlight = g_texPointLight.Sample(g_samDeferredLinear, input.Tex);
    float2 motion = g_texMotion.Sample(g_samDeferredLinear, input.Tex).rg;
    float4 depthRMA = g_texDepthPBR.Sample(g_samDeferredLinear, input.Tex);
    float4 shadow = g_texShadow.Sample(g_samDeferredLinear, input.Tex);
  
    if (g_fGBufferEnable)
    {
        uint type = g_fDeferred.x;
        
        if (type == 0)
        {
            Color = diffuse;
            return Color;
        }
    
        if (type == 1)
        {
            Color = normal;
            return Color;
        }
    
        if (type == 2)
        {
            Color = position;
            return Color;
        }
    
        if (type == 3)
        {
            Color = pointlight;
            return Color;
        }
    
        if (type == 4)
        {
            Color = float4(motion.x, motion.y, 0, 1);
            return Color;
        }

    
        if (type == 5)
        {
            float depth = depthRMA.r;
        
            Color = float4(depth, depth, depth, 1);
            return Color;
        }
    
        if (type == 6)
        {
            float roughness = depthRMA.g;
        
            Color = float4(roughness, roughness, roughness, 1);
            return Color;
        
        }
    
        if (type == 7)
        {
            float metallic = depthRMA.b;
        
            Color = float4(metallic, metallic, metallic, 1);
            return Color;
        
        }
    
        if (type == 8)
        {
            float ao = depthRMA.a;
            Color = float4(ao, ao, ao, 1);
            return Color;
        
        }
    
        if (type == 9)
        {
            //投影テクスチャシャドウマップ
            float shadow = 1.0f - pointlight.a;
            Color = float4(shadow, shadow, shadow, 1);
            return Color;
        }
    
        if (type == 10)
        {
            //シャドウ
            float4 col = shadow;
            col.a = 1;
            Color = col;
            return Color;
        }
    }
    
    
    //ここにすべてのポストエフェクト処理関数を入れる
    //仮
    Color = DirectionalLighting(diffuse, diffuse, normal, position.xyz);
    Color = PointLighting(Color, pointlight.xyz);
    
    
       
    
    return Color;
}
