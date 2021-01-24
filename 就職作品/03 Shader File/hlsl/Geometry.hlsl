#define PI (3.14159265)


struct VS_OUT
{
	float4 Pos			: SV_POSITION;
	float4 WorldPos		: POSITION;
	float3 WorldNormal	: NORMAL;
    float3 WorldTangent : TANGENT;
    float3 WorldBinormal: BINORMAL;

	float2 Tex			: TEXCOORD0;

	float3 EyeVector	: TEXCOORD1;
	float2 Velocity		: TEXCOORD2;
	float  Depth		: TEXCOORD3;
};
struct PS_OUT
{
    //GBuffer
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
	float4 vPointLight	: SV_Target3;
	float2 vMotion		: SV_Target4;
	float4 vDepthPBR	: SV_Target5;
	float4 vShadow  	: SV_Target6;
};

//PointLight
struct VS_PL_OUT
{
    float4 Pos			: SV_POSITION;
    float2 Tex          : TEXCOORD0;
    
    float3 WorldNormal	: NORMAL;
};
struct PS_PL_OUT
{
    float4 PointLight  : SV_Target0;
};


struct VS_SHADOW_OUT
{
    float4 Pos                  : SV_POSITION;
    float4 ProjectorSpacePos    : TEXCOORD0;
    float3 WorldPos             : TEXCOORD1;
    float3 CenterPos            : TEXCOORD2;
    float2 Tex                  : TEXCOORD3;
};



struct VS_CUBEMAP_OUT
{
    float4 Pos      : SV_POSITION;
    float3 Normal   : NORMAL;
    float2 Tex      : TEXCOORD0;
    uint   TexNum   : TEXCOORD1;
};

struct VS_DLIGHT_OUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;

};

//ê≥ãKâªLambert
float3 NormalizedLambert(float3 diffuseColor)
{
    return diffuseColor / PI;
}
//ÉtÉåÉlÉã
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

//ãæñ îΩéÀBRDF
float3 BRDF(float3 albedo, float metallic, float roughness, float3 N, float3 V, float3 L, float3 H)
{
    float EPSILON = 0.001;
    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);//îÒã‡ëÆÇ≈Ç‡0.04ÇÕîΩéÀÇ∑ÇÈ
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
