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

cbuffer CameraBuffer : register(b3)
{
    float4 g_vEyePos;
    
    float4 g_vWordldCameraPos[4];//ワールド座標の画面端4頂点
}


//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b4)
{
    float4 g_vDirectionalLightDir; //ライトの座標
    float4 g_vDirectionalLightPos; //ライトの座標
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
    bool g_fFogEnable;
    
    
}

cbuffer AnimationBuffer : register(b7)
{
    matrix g_Animation[ANIMATION_MATRIX_MAX];
}

cbuffer ToggleBuffer : register(b8)
{
    bool g_FrustumCullingEnable;
    
    float3 dummy_b8;
}