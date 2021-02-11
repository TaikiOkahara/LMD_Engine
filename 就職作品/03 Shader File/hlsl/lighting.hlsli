#ifndef INCLUDE_DIRECTIONALLIGHT
#define INCLUDE_DIRECTIONALLIGHT

#define NearClip (0.1f)
#define FarClip (1000.0f)

#define PI (3.14159265)


// �T���v�����O��
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

// �u���[�t�B���^�[���̃e�N�Z���̃I�t�Z�b�g�z��
// �T���v�����O��
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


//���K��Lambert
float3 NormalizedLambert(float3 diffuseColor)
{
    return diffuseColor / PI;
}
//�t���l��
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

//���ʔ���BRDF
float3 BRDF(float3 albedo, float metallic, float roughness, float3 N, float3 V, float3 L, float3 H)
{
    float EPSILON = 0.001;
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic); //������ł�0.04�͔��˂���
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
    //NL = NL * 0.8f + 0.2f;//   �Œ�l�␳
   
    
    float3 Eye = g_vCameraPos.xyz - worldPos;
    Eye = normalize(Eye);

    float3 Reflect = reflect(vLightVector, normal.xyz);
    Reflect = normalize(Reflect);
	
    
    
    //AO�ݒ�
    float ao = g_fAmbientOcclusion.x;
   
    if(normal.w ==2.0f)
        return diffuse;
    
    color.rgb *= diffuse.xyz * NL * g_vLightColor.xyz; //���C�e�B���O���s��
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
  // �`��s�N�Z���̖@���}�b�v + �[�x�}�b�v���擾
    float3 dispNormalMap = g_texNormal.Sample(samplerState, tex);
    float2 depth = motionDepth.Sample(samplerState, tex).ba;
    // �@���x�N�g�����쐬����
    float3 dispNormal = dispNormalMap.xyz;

    // �`��s�N�Z���̃e�N�Z�����W����N���b�v��ԏ�̍��W���v�Z
    float4 dispClipPos;
    dispClipPos.xy = (tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f)) * depth.y;
    dispClipPos.zw = depth.xy;
       
        // �J������ԏ�ł̍��W���v�Z
    float4 dispViewPos = mul(dispClipPos, g_mInvProj);

        
    float normAO = 0;
    float depthAO = 0;
                
        
    float g_HemRadius = g_fAmbientOcclusion.y; //0.2
    float g_Zfar = g_fAmbientOcclusion.z;
    float g_AOPower = g_fAmbientOcclusion.w;
    
    for (uint i = 0; i < SPHERE_COUNT; i++)
    {
        // ���C�̕����x�N�g��
        float3 ray = SphereArray16[i].xyz * g_HemRadius;

        // ���C�̕����x�N�g����`��s�N�Z���̖@�������̔������Ɏ��܂�悤�ɕ�����ϊ�����
        ray = sign(dot(normal.xyz, ray)) * ray;
      
        // ���̓s�N�Z���̍��W
        float4 envPos;
        // ���C�����Ɉړ�
        envPos.xyz = dispViewPos.xyz + ray;
        // �N���b�v��ԏ�ɍs��ϊ�
        envPos = mul(float4(envPos.xyz, 1), g_mProj);
        // �X�N���[����Ԃɕϊ�
        envPos.xy = envPos.xy / envPos.w * float2(0.5f, -0.5f) + 0.5f;

        // ���̓s�N�Z���̃T���v�����O
        float4 envNormalMap = g_texNormal.Sample(samplerState, envPos.xy);

        // �@���x�N�g�����쐬����
        float3 envNormal = envNormalMap.xyz;

        // ���ς̊p�x���傫���Ȃ�قǊ����Օ��W�����傫���Ȃ�悤�Ɍv�Z����
        float n = dot(dispNormal, envNormal) * 0.5f + 0.5f;
        // �G�b�W���ʂɂȂ��Ă��镔���͎Օ�����Ȃ��悤�ɂ���
        n += step(dispNormalMap.z, envNormalMap.z);
        normAO += min(n, 1);
      
        // �[�x�l�̋������傫���قǊ����Օ��̉e���͂��������Ȃ�悤�ɂ���
        depthAO += abs(dispNormalMap.z - envNormalMap.z) / g_Zfar;
                
    }
    
    color = normAO / (float) SPHERE_COUNT + depthAO;
    // �A����������
    color = pow(abs(color), g_AOPower);
    
    return color;
}

float LinearizeDepth(float depth, float near, float far)
{
    return (2.0 * near) / (far + near - depth * (far - near));
}

#endif // INCLUDE_DIRECTIONALLIGHT