#include "ConstantBuffer.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

AppendStructuredBuffer<uint> indexList : register(u0);

cbuffer CullingBuffer : register(b9)
{
    uint g_uInstanceCount;
    
    float3 dummy_b0;
    
    float4 g_fCullingPos[8];
}




[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    uint index = DTid.x;
    
    if (index > g_uInstanceCount - 1)//0からカウントするため
        return;

    
    matrix world = WorldMatrixBuffer[index];

   
    
    //視錐台カリング
    for (uint i = 0; i < 8;i++)
    {
        
        float3 pos = float3(world._41, world._42, world._43);
        
        float3 cullpos = mul(g_fCullingPos[i].xyz, (float3x3)world);

        pos += cullpos;
        
        float3 v, v1, v2,v3,v4, n1,n2;
    
        v = pos - g_vEyePos.xyz;
        v = normalize(v);
    
        //左面
        v1 = g_vWordldCameraPos[0].xyz - g_vEyePos.xyz;
        v2 = g_vWordldCameraPos[2].xyz - g_vEyePos.xyz;
        n1 = cross(v1, v2);
        n1 = normalize(n1);
    
            
    
        //右面
        v3 = g_vWordldCameraPos[3].xyz - g_vEyePos.xyz;
        v4 = g_vWordldCameraPos[1].xyz - g_vEyePos.xyz;
        n2 = cross(v3, v4);
        n2 = normalize(n2);
    
        
        if (dot(n1, v) > 0.0f && dot(n2, v) > 0.0f)
        {
            indexList.Append(index);
            return;
        }
        
    }
    

}