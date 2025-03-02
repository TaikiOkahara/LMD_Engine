#define ANIMATION_MATRIX_MAX (64)
#define LIGHT_NUM (6)


#ifndef CONSTANT_BUFFER
#define CONSTANT_BUFFER



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
    matrix g_mInvProj;
}

cbuffer CameraBuffer : register(b3)
{
    float4 g_vCameraPos;
    float4 g_vCullingCameraPos;
}


//　ディレクショナルライト
cbuffer DirectionalLightBuffer : register(b4)
{
    float4 g_vDirectionalLightDir;
    float4 g_vDirectionalLightPos;
	float4 g_vLightColor;
    matrix g_vDirectionalLightVP;
}


struct POINT
{
    float3 color;
    float intensity;
    float3 calc;
    float size;
    float3 pos;
    uint index;
};

cbuffer PointLightBuffer : register(b5)
{
    POINT g_vPointLight;//[LIGHT_NUM];
    matrix g_vPointLightViewProj[8];
    float4 g_vPointLightPlayerPos;
}


cbuffer EffectBuffer : register(b6)
{
    float g_fDeferred;//xでレンダリングバッファ変更（セット番号と共通
    bool g_fShadow;//影描画
    float effDummy;
    bool g_fGBufferEnable;//GBufferを表示するかどうか
    
    //Fog
    float4 g_fFogOffset; //フォグの揺らぎテクスチャーのテクセルのオフセット値 x 2 
    float4 g_fFogData; //x :テクセルのスケーリング値 y :頂点の高さの最大値 z : 頂点の高さの最小値 w : ダミー
  
    float3 g_fFogColor;
    bool g_fFogEnable;
    
    float4 g_fAmbientOcclusion;//x：有無、y：環境光遮蔽の判定用の半球の半径、z：Zfar、w：影の強度
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

#endif  // CONSTANT_BUFFER