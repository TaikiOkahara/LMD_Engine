#include "Geometry.hlsl"
#include "ConstantBuffer.hlsl"

Texture2D g_texCol : register(t0);
Texture2D g_Nor : register(t1);
Texture2D g_MRA : register(t2);
Texture2D g_texPointLight : register(t3);

SamplerState g_samLinear : register(s0);
SamplerState g_samDeferredLinear : register(s1);

float4 main(VS_SHADOW_OUT input) : SV_Target
{
    input.ProjectorSpacePos.xyz /= input.ProjectorSpacePos.w;
    float2 uv = input.ProjectorSpacePos.xy;
    

    float projectTex = g_texPointLight.Sample(g_samDeferredLinear, uv).a;
    
    // カメラの範囲外を適用しない処理
    {
        //float3 isOut = step((input.ProjectorSpacePos - 0.5) * sign(input.ProjectorSpacePos), 0.5);
        //float alpha = isOut.x * isOut.y * isOut.z;
        //            // プロジェクターから見て裏側の面には適用しない
        //alpha *= step(-dot(lerp(-g_vDirectionalLightPos.xyz, g_vDirectionalLightPos.xyz - input.WorldPos, g_vDirectionalLightPos.w), i.worldNormal), 0);
        //return lerp(1, projectorTex, alpha);
        
        //return g_texPointLight.Sample(g_samLinear, input.Tex);
        
        //float shadow = 1.0f - projectTex.a;
        
        //projectTex.w = 1;
    }
   
    
    float dist = 1 - (distance(input.WorldPos, input.CenterPos) / 2);
    
    projectTex *= dist;
    return float4(projectTex, 0, 0, 1); //float4(1, 1, 1, 1);

}
