#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

PS_PL_OUT main(VS_OUT input)
{
    PS_PL_OUT Out = (PS_PL_OUT) 0;
 
    Out.PointLight = float4(0, 0, 0, 1);
    
    return Out;
}