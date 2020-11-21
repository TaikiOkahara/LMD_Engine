#define ANIMATION_MATRIX_MAX (64)
#define LIGHT_NUM (128)




cbuffer WorldBuffer : register(b0)
{
    matrix g_mWorld;
    matrix g_mWIT;
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


//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b4)
{
    float4 g_vLight; //ライトの座標
	float4 g_vLightColor;//色	
	
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
    float4 g_fDeferred;//xでレンダリングバッファ変更（セット番号と共通）
    
    
    //Fog
    float4 g_fFogOffset; //フォグの揺らぎテクスチャーのテクセルのオフセット値 x 2 
    float4 g_fFogData; //x :テクセルのスケーリング値 y :頂点の高さの最大値 z : 頂点の高さの最小値 w : ダミー
  
    float3 g_fFogColor;
    float g_fFogEnable;
    
    
}

cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[ANIMATION_MATRIX_MAX];
}