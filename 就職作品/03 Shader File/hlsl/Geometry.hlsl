
//バーテックスバッファー出力
struct VS_OUT
{
	float4 Pos			: SV_POSITION;
	float4 WorldPos		: POSITION;
	float3 WorldNormal	: NORMAL;
    float3 WorldTangent : TANGENT;
    float3 WorldBinormal: BINORMAL;

	float2 Tex			: TEXCOORD0;

	float3 EyeVector	: TEXCOORD1;
	float3 MotionDir	: TEXCOORD2;
	float  Depth		: TEXCOORD3;
};
//ピクセルシェーダー出力
struct PS_OUT
{
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
	float4 vPointLight	: SV_Target3;
	float4 vMotion		: SV_Target4;
	float  vDepth		: SV_Target5;
};

struct VS_LIGHT_OUT
{
    float4 Pos			: SV_POSITION;
    float2 Tex          : TEXCOORD0;
    float3 LightPos		: TEXCOORD1;
    float3 LightRange	: TEXCOORD2;
    uint   LightIndex	: TEXCOORD3;
    float4 WorldPos : POSITION;
    float3 WorldNormal : NORMAL;
};

