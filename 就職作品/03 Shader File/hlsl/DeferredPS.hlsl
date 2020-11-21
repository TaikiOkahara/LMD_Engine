#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor	: register(t0);
Texture2D g_texNormal	: register(t1);
Texture2D g_texPosition	: register(t2);
Texture2D g_texPointLight: register(t3);
Texture2D g_texMotion	: register(t4);
Texture2D g_texDepth	: register(t5);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);



float4 main(VS_OUT input) : SV_Target
{
   
    uint type = g_fDeferred.x;
    
    float4 Color;
    
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
    
    
    //Color = (type == 0) ? g_texColor.Sample(g_samDeferredLinear, input.Tex) : 0;
    //Color = (type == 1) ? g_texNormal.Sample(g_samDeferredLinear, input.Tex) : 0;
    //Color = (type == 2) ? g_texPosition.Sample(g_samDeferredLinear, input.Tex) : 0;
    //Color = (type == 3) ? g_texPointLight.Sample(g_samDeferredLinear, input.Tex) : 0;
    //Color = (type == 4) ? g_texMotion.Sample(g_samDeferredLinear, input.Tex) : 0;
    
    if(type == 5)
    {
        float depth = g_texDepth.Sample(g_samDeferredLinear, input.Tex);
        
        Color = float4(depth, depth, depth, 1);
    }
    
    
    
    
    
    
    //PointLight‚ÌŒ‹‰Ê‚¾‚¯‚ð•`‰æ    
    //Color = g_texDepth.Sample(g_samDeferredLinear, input.Tex).r;
    //float4 Color = g_texMotion.Sample(g_samDeferredLinear, input.Tex);
    
    return Color;
}
