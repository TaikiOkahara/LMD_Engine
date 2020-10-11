#include "Geometry.hlsl"


//GBuffer
Texture2D g_texColor : register(t2);
Texture2D g_texNormal : register(t3);
Texture2D g_texPosition : register(t4);


PS_OUT main(VS_OUT input)
{
    PS_OUT Out = (PS_OUT) 0;

	//カラーテクスチャーへ出力 
    Out.vColor = float4(0, 1, 0, 1);
    Out.vNormal = float4(0,0,-1,2); //wが２ならライティングを行わない（天才）
    Out.vPosition = input.WorldPos;
    
    return Out;
}