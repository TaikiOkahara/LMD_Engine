cbuffer WorldBuffer : register(b0)
{
    matrix g_mWorld;
    matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1)
{
    matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix g_mProj;
}

cbuffer EyeBuffer : register(b3)
{
    float4 g_vEye;
}


//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b4)
{
    float4 g_vLight; //ライトの座標
	float4 g_vLightColor;//色	
	
	//matrix g_mLightView;
}

#define LIGHT_NUM (128)
cbuffer PointLightBuffer : register(b5)
{
    //float g_vPointIntensity[LIGHT_NUM];
    //float g_vPointRange[LIGHT_NUM];
    //float g_vPointAttenuation[LIGHT_NUM];
    //float3 g_vPointColor[LIGHT_NUM];
    
    
    float4 g_vPointColor[LIGHT_NUM];//xyzは色、wは光の強さ
    float4 g_vPoint[LIGHT_NUM]; //x：一定減衰係数　y：線形減衰係数　z：２次減衰係数 w：スペキュラ
    
    //float2 dummy;
}

//
//register7はアニメーションコンスタントバッファ