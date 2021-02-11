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
	float2 DepthZW		: TEXCOORD3;
};
struct PS_OUT
{
    //GBuffer
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
	float4 vPointLight	: SV_Target3;
	float4 vMotionDepth : SV_Target4;
	float4 vPBR	        : SV_Target5;
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
    float3 WorldPos             : TEXCOORD0;
    float3 CenterPos            : TEXCOORD1;
    float2 Tex                  : TEXCOORD2;
    
    float4 ProjectorSpacePos_1    : TEXCOORD3;
    float4 ProjectorSpacePos_2    : TEXCOORD4;
    float4 ProjectorSpacePos_3    : TEXCOORD5;
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