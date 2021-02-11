#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

PS_PL_OUT main(VS_OUT input)
{
    PS_PL_OUT Out = (PS_PL_OUT) 0;
 
    int index = g_vPointLight.index;
    
    
    if(index == 0)
        Out.PointLight = float4(1, 0, 0, 1);
    else if (index == 1)
        Out.PointLight = float4(0, 1, 0, 1);
    else if (index == 2)
        Out.PointLight = float4(0, 0, 1, 1);
 
    
    return Out;
}