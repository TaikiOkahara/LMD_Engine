#include "ConstantBuffer.hlsl"

StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

AppendStructuredBuffer<uint> indexList : register(u0);

cbuffer CullingBuffer : register(b9)
{
    uint g_uInstanceCount;
    
    float3 g_fCulldummy;
    
    float4 g_fCullingPos[8];
    
    float4 g_vCullingCameraPoint[4];//���[���h���W�̉�ʒ[4���_�A������J�����O�ŗ��p
}




[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    uint index = DTid.x;
    
    if (index > g_uInstanceCount - 1)
        return;

    
    matrix world = WorldMatrixBuffer[index];

   
    float3 pos = float3(world._41, world._42, world._43);
    
    //������J�����O
    for (uint i = 0; i < 8;i++)
    {
        
        
        float3 cullpos = mul(g_fCullingPos[i].xyz, (float3x3)world);

        pos += cullpos;
        
        float3 v, v1, v2,v3,v4, n1,n2;
    
        v = pos - g_vCullingCameraPos.xyz;
        v = normalize(v);
    
        //����
        v1 = g_vCullingCameraPoint[0].xyz - g_vCullingCameraPos.xyz;
        v2 = g_vCullingCameraPoint[2].xyz - g_vCullingCameraPos.xyz;
        n1 = cross(v1, v2);
        n1 = normalize(n1);
    
            
    
        //�E��
        v3 = g_vCullingCameraPoint[3].xyz - g_vCullingCameraPos.xyz;
        v4 = g_vCullingCameraPoint[1].xyz - g_vCullingCameraPos.xyz;
        n2 = cross(v3, v4);
        n2 = normalize(n2);
    
        
        if (dot(n1, v) > 0.0f && dot(n2, v) > 0.0f)
        {
            indexList.Append(index);
            return;
        }
        
    }
    

}