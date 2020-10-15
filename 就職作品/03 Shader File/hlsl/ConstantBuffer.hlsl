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

//　ポイントライト
cbuffer PointLightBuffer : register(b4)
{
    float4 g_vPointLight[10];
}


//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b5)
{
    float4 g_vLight; //ライトの座標
	//float4 g_fIntensity;//明るさ	
	
	//matrix g_mLightView;
}

//
//register7はアニメーションコンスタントバッファ