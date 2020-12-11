
StructuredBuffer<matrix> WorldMatrixBuffer : register(t0);

AppendStructuredBuffer<uint> indexList : register(u0);

cbuffer CullingBuffer : register(b0)
{
    uint g_uInstanceCount;
    
    float3 dummy;
    
    //matrix cameraMatrix;
    float4 g_fCullingCamerapos[4];
    
    float4 g_fCullingPos[8];
}

cbuffer ViewBuffer : register(b1)
{
    matrix g_mView;
    matrix g_mOldView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix g_mProj;
    matrix g_mOldProj;
}

cbuffer EyeBuffer : register(b3)
{
    float4 g_vEye;
}




[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    uint index = DTid.x;
    
    if (index > g_uInstanceCount - 1)//0からカウントするため
        return;
    
    
   
   
    
    //視錐台カリングはここで行う
    matrix world = WorldMatrixBuffer[index];
    
    
    
    
   
    
    for (uint i = 0; i < 8;i++)
    {
        
        float3 pos = float3(world._41, world._42, world._43);
        
        float3 cullpos = mul(g_fCullingPos[i].xyz, (float3x3)world);

        pos += cullpos;
        
        float3 v, v1, v2,v3,v4, n1,n2;
    
        v = pos - g_vEye.xyz;
        v = normalize(v);
    
        //左面
        v1 = g_fCullingCamerapos[0].xyz - g_vEye.xyz;
        v2 = g_fCullingCamerapos[2].xyz - g_vEye.xyz;
        n1 = cross(v1, v2);
        n1 = normalize(n1);
    
            
    
        //右面
        v3 = g_fCullingCamerapos[3].xyz - g_vEye.xyz;
        v4 = g_fCullingCamerapos[1].xyz - g_vEye.xyz;
        n2 = cross(v3, v4);
        n2 = normalize(n2);
    
        
        if (dot(n1, v) > 0.0f && dot(n2, v) > 0.0f)
        {
            indexList.Append(index);
            return;
        }
        
    }
    
   
    
    
    
    
    
    
}