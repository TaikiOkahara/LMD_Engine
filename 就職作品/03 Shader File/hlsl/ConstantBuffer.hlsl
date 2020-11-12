#define ANIMATION_MATRIX_MAX (256)
#define LIGHT_NUM (128)




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


struct POINT
{
    float3 color;
    float intensity;
    float3 calc;
    float specular;
};

cbuffer PointLightBuffer : register(b5)
{
    POINT g_vPointLight[LIGHT_NUM];
}


cbuffer EffectBuffer : register(b6)
{
  
    float2 g_fFogOffset0; //フォグの揺らぎテクスチャーのテクセルのオフセット値
    float2 g_fFogOffset1; //フォグの揺らぎテクスチャーのテクセルのオフセット値
    float g_fFogTexScale; //テクセルのスケーリング値。
    float g_fFogMaxHeight; //頂点の高さの最大値。頂点の高さがこの値より大きくなるとフォグがかからなくなる。
    float g_fFogMinHeight; //頂点の高さの最小値。頂点の高さがこの値より小さくなるとフォグカラーで塗りつぶされる。
    float g_fFogDummy0;
    
    float3 g_fFogColor;
    float g_fFogDummy1;
}

cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[ANIMATION_MATRIX_MAX];
}