#ifndef INCLUDE_DIRECTIONALLIGHT
#define INCLUDE_DIRECTIONALLIGHT

#define NearClip (0.1f)
#define FarClip (1000.0f)

#define PI (3.14159265)


// サンプリング数
#define SPHERE_COUNT 16
static float3 SphereArray16[SPHERE_COUNT] =
{
    float3(0.53812504, 0.18565957, -0.43192)
      , float3(0.13790712, 0.24864247, 0.44301823)
      , float3(0.33715037, 0.56794053, -0.005789503)
      , float3(-0.6999805, -0.04511441, -0.0019965635)
      , float3(0.06896307, -0.15983082, -0.85477847)
      , float3(0.056099437, 0.006954967, -0.1843352)
      , float3(-0.014653638, 0.14027752, 0.0762037)
      , float3(0.010019933, -0.1924225, -0.034443386)
      , float3(-0.35775623, -0.5301969, -0.43581226)
      , float3(-0.3169221, 0.106360726, 0.015860917)
      , float3(0.010350345, -0.58698344, 0.0046293875)
      , float3(-0.08972908, -0.49408212, 0.3287904)
      , float3(0.7119986, -0.0154690035, -0.09183723)
      , float3(-0.053382345, 0.059675813, -0.5411899)
      , float3(0.035267662, -0.063188605, 0.54602677)
      , float3(-0.47761092, 0.2847911, -0.0271716)
};

// ブラーフィルター時のテクセルのオフセット配列
// サンプリング数
#define BLUROFFSET_COUNT 16
static float2 BlurOffset16[BLUROFFSET_COUNT] =
{
    float2(1, 1)
      , float2(-1, 1)
      , float2(-1, -1)
      , float2(1, -1)
      , float2(3, 1)
      , float2(3, 3)
      , float2(1, 3)
      , float2(-1, 3)
      , float2(-3, 3)
      , float2(-3, 1)
      , float2(-3, -1)
      , float2(-3, -3)
      , float2(-1, -3)
      , float2(1, -3)
      , float2(3, -3)
      , float2(3, -1)
};


#include "ConstantBuffer.hlsl"


//正規化Lambert
float3 NormalizedLambert(float3 diffuseColor)
{
    return diffuseColor / PI;
}
//フレネル
float3 F_Schlick(float3 F0, float3 H, float3 V)
{
    return (F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0), 5.0));
}

float D_GGX(float a, float NoH)
{
    float a2 = a * a;
    float NoH2 = NoH * NoH;
    float d = NoH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_SchlickGGX(float a, float NoV)
{
    float EPSILON = 0.001;
    float k = a * a / 2 + EPSILON;
    float up = NoV;
    float down = NoV * (1 - k) + k;
    return up / down;
}

float G_Smith(float a, float NoV, float NoL)
{
    float ggx0 = G_SchlickGGX(1, NoV);
    float ggx1 = G_SchlickGGX(1, NoL);
    return ggx0 * ggx1;
}

//鏡面反射BRDF
float3 BRDF(float3 albedo, float metallic, float roughness, float3 N, float3 V, float3 L, float3 H)
{
    float EPSILON = 0.001;
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic); //非金属でも0.04は反射する
    float NoH = max(dot(N, H), 0);
    float NoV = max(dot(N, V), 0);
    float NoL = max(dot(N, L), 0);
    float a = roughness * roughness;

  // specular
    float3 F = F_Schlick(F0, H, V);
    float D = D_GGX(a, NoH);
    float G = G_Smith(a, NoV, NoL);
    float3 up = F * D * G;
    float down = max(4.0 * NoV * NoL, EPSILON);
    float3 specular = up / down;

  // diffuse
    float3 diffuse = NormalizedLambert(albedo);

    float3 kD = float3(1.0, 1.0, 1.0) - F;
    kD *= 1.0 - metallic;
    return kD * diffuse + specular;
}

float DistanceAttenuation(float distance, float lightRange)
{
    float EPSILON = 0.01;
    float att = 1.0 / (distance * distance + EPSILON);
    float smoothatt = 1 - pow(distance / lightRange, 4.0);
    smoothatt = max(smoothatt, 0.0);
    smoothatt = smoothatt * smoothatt;
    return att * smoothatt;
}



float4 DirectionalLighting(float4 color,float4 diffuse, float4 normal, float3 worldPos)
{
   if(normal.w == 2)
        return color;
    
    float3 vLightVector = normalize(g_vDirectionalLightDir).xyz;
    float NL = saturate(-dot(normal.xyz, vLightVector));
    //NL = NL * 0.8f + 0.2f;//   最低値補正
   
    
    float3 Eye = g_vCameraPos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    //AO設定
    float ao = g_fAmbientOcclusion.x;
   
    if(normal.w ==2.0f)
        return diffuse;
    
    color.rgb *= diffuse.xyz * NL * g_vLightColor.xyz; //ライティングを行か
    color.a = diffuse.a;
 
    
    return color;
   
}

float4 PointLighting(float4 color,float3 pointlight)
{
    color.rgb += pointlight;
    return color;
}

float4 SSAO(float4 normal,Texture2D g_texNormal,Texture2D motionDepth,SamplerState samplerState,float2 tex)
{
  
   float4 color;
  // 描画ピクセルの法線マップ + 深度マップを取得
    float3 dispNormalMap = g_texNormal.Sample(samplerState, tex);
    float2 depth = motionDepth.Sample(samplerState, tex).ba;
    // 法線ベクトルを作成する
    float3 dispNormal = dispNormalMap.xyz;

    // 描画ピクセルのテクセル座標からクリップ空間上の座標を計算
    float4 dispClipPos;
    dispClipPos.xy = (tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f)) * depth.y;
    dispClipPos.zw = depth.xy;
       
        // カメラ空間上での座標を計算
    float4 dispViewPos = mul(dispClipPos, g_mInvProj);

        
    float normAO = 0;
    float depthAO = 0;
                
        
    float g_HemRadius = g_fAmbientOcclusion.y; //0.2
    float g_Zfar = g_fAmbientOcclusion.z;
    float g_AOPower = g_fAmbientOcclusion.w;
    
    for (uint i = 0; i < SPHERE_COUNT; i++)
    {
        // レイの方向ベクトル
        float3 ray = SphereArray16[i].xyz * g_HemRadius;

        // レイの方向ベクトルを描画ピクセルの法線方向の半球内に収まるように方向を変換する
        ray = sign(dot(normal.xyz, ray)) * ray;
      
        // 周囲ピクセルの座標
        float4 envPos;
        // レイ方向に移動
        envPos.xyz = dispViewPos.xyz + ray;
        // クリップ空間上に行列変換
        envPos = mul(float4(envPos.xyz, 1), g_mProj);
        // スクリーン空間に変換
        envPos.xy = envPos.xy / envPos.w * float2(0.5f, -0.5f) + 0.5f;

        // 周囲ピクセルのサンプリング
        float4 envNormalMap = g_texNormal.Sample(samplerState, envPos.xy);

        // 法線ベクトルを作成する
        float3 envNormal = envNormalMap.xyz;

        // 内積の角度が大きくなるほど環境光遮蔽係数が大きくなるように計算する
        float n = dot(dispNormal, envNormal) * 0.5f + 0.5f;
        // エッジが凸になっている部分は遮蔽されないようにする
        n += step(dispNormalMap.z, envNormalMap.z);
        normAO += min(n, 1);
      
        // 深度値の距離が大きいほど環境光遮蔽の影響力が小さくなるようにする
        depthAO += abs(dispNormalMap.z - envNormalMap.z) / g_Zfar;
                
    }
    
    color = normAO / (float) SPHERE_COUNT + depthAO;
    // 陰を強調する
    color = pow(abs(color), g_AOPower);
    
    return color;
}

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

#endif // INCLUDE_DIRECTIONALLIGHT