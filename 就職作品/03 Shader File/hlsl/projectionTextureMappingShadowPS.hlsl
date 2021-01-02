#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"


//GBuffer
Texture2D g_texColor	: register(t0);
Texture2D g_texNormal	: register(t1);
Texture2D g_texPosition	: register(t2);
Texture2D g_texDepthPBR: register(t3);


PS_PL_OUT main(VS_OUT input)
{
    PS_PL_OUT Out = (PS_PL_OUT) 0;
 
    Out.PointLight = float4(0, 0, 0, 1);
    
    return Out;
}