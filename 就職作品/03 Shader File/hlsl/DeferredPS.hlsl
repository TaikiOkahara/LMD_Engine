#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


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
   
    uint type = g_fDeferred.x;
    
    float4 Color = float4(0, 0, 0, 1);
    
    if (type == 0)
    {
        Color = g_texColor.Sample(g_samDeferredLinear, input.Tex);
        return Color;
    }
    
    if (type == 1)
    {
        Color = g_texNormal.Sample(g_samDeferredLinear, input.Tex);
        return Color;
    }
    
    if (type == 2)
    {
        Color = g_texPosition.Sample(g_samDeferredLinear, input.Tex);
        return Color;
    }
    
    if (type == 3)
    {
        Color = g_texPointLight.Sample(g_samDeferredLinear, input.Tex);
        return Color;
    }
    
    if (type == 4)
    {
        Color = g_texMotion.Sample(g_samDeferredLinear, input.Tex);
        return Color;
    }

    
    if(type == 5)
    {
        float depth = g_texDepthPBR.Sample(g_samDeferredLinear, input.Tex).r;
        
        Color = float4(depth, depth, depth, 1);
        return Color;
    }
    
    if (type == 6)
    {
        float roughness = g_texDepthPBR.Sample(g_samDeferredLinear, input.Tex).g;
        
        Color = float4(roughness, roughness, roughness, 1);
        return Color;
        
    }
    
    if (type == 7)
    {
        float metallic = g_texDepthPBR.Sample(g_samDeferredLinear, input.Tex).b;
        
        Color = float4(metallic, metallic, metallic, 1);
        return Color;
        
    }
    
    if (type == 8)
    {
        float ao = g_texDepthPBR.Sample(g_samDeferredLinear, input.Tex).a;
        Color = float4(ao, ao, ao, 1);
        return Color;
        
    }
    
    if (type == 9)
    {
        
        //input.Tex = mul(input.Tex, g_vDirectionalLightVP);
        
        
        //���e�e�N�X�`���V���h�E�}�b�v
        float shadow = 1.0f - g_texPointLight.Sample(g_samDeferredLinear, input.Tex).a;
        Color = float4(shadow, shadow, shadow, 1);
        return Color;
        
    }
    
    if (type == 10)
    {
        //�V���h�E
        float4 shadow = g_texShadow.Sample(g_samDeferredLinear, input.Tex);
        shadow.a = 1;
        Color = shadow;
        return Color;
        
    }
    
       
    return Color;
}
