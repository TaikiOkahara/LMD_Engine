#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"
#include "lighting.hlsli"

//GBuffer
Texture2D g_texColor	: register(t0);
Texture2D g_texNormal	: register(t1);
Texture2D g_texPosition	: register(t2);
Texture2D g_texPointLight: register(t3);
Texture2D g_texMotionDepth	: register(t4);
Texture2D g_texPBR : register(t5);
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
    float4 motionDepth = g_texMotionDepth.Sample(g_samDeferredLinear, input.Tex);
    float4 RMA = g_texPBR.Sample(g_samDeferredLinear, input.Tex);
    float4 shadow = g_texShadow.Sample(g_samDeferredLinear, input.Tex);
  
    float4 returnAO;
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
            Color = float4(motionDepth.x, motionDepth.y, 0, 1);
            return Color;
        }

    
        if (type == 5)
        {
            float depth = motionDepth.z / motionDepth.w;
            float linearZ = LinearizeDepth(depth, NearClip, FarClip);
            Color = float4(linearZ, linearZ, linearZ, 1);
            return Color;
            
        }
    
        if (type == 6)
        {
            float roughness = RMA.r;
        
            Color = float4(roughness, roughness, roughness, 1);
            return Color;
           
        }
    
        if (type == 7)
        {
            float metallic = RMA.g;
        
            Color = float4(metallic, metallic, metallic, 1);
            return Color;
        
        }
    
        
        
        if (type == 8)
        {
          
            Color = SSAO(normal, g_texNormal, g_texMotionDepth, g_samDeferredLinear, input.Tex);
            return Color;
        }
    
        if (type == 9)
        {
            //投影テクスチャシャドウマップ
            Color = float4(shadow.rgb,1);
            return Color;
        }
    
        if (type == 10)
        {
            //シャドウ
            Color = float4(pointlight.a, 0, 0, 1);
            return Color;
        }
    }
    
    
    
    
    //ここにすべてのポストエフェクト処理関数を入れる
    Color += DirectionalLighting(diffuse, diffuse, normal, position.xyz);
    Color += PointLighting(Color, pointlight.xyz);
    
    //投影テクスチャマッピング
    if (g_fShadow)
    {
        float shadowTex = 1.0f - g_texPointLight.Sample(g_samDeferredLinear, input.Tex).a;
        Color.rgb *= shadowTex;
    }
    
    if (g_fAmbientOcclusion.x && normal.w != 2)
    {
        Color *= SSAO(normal, g_texNormal, g_texMotionDepth, g_samDeferredLinear, input.Tex);
    }
      
     
       
    
    return Color;
}
