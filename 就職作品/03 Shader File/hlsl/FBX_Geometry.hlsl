
//バーテックスバッファー出力
struct VS_OUT
{
	float4 Pos			: SV_POSITION;
	float4 WorldPos		: POSITION;
	float3 WorldNormal	: NORMAL;
    float3 WorldTangent : TANGENT;
    float3 WorldBinormal: BINORMAL;
	float2 Tex			: TEXCOORD0;
	float3 LightVector  : TEXCOORD1;
	float4 LightTexCoord: TEXCOORD2;
	float4 LightViewPos : TEXCOORD3;
	//float4 OutLinePos   : TEXCOORD4;

	//float4 ShadowPos	: TEXCOORD4;



	float3 EyeVector	: TEXCOORD5;
};
//ピクセルシェーダー出力
struct PS_OUT
{
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
	//float4 vShadowMap	: SV_Target3;
	//float4 vOutLine		: SV_Target3;
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT_DEPTH//深度テクスチャーレンダー用
{
	float4 Pos		: SV_POSITION;
	float4 Depth	: POSITION;
};

//struct PS_OUTPUT_DEPTH
//{
//	float4 vShadowMap	: SV_Target;
//};


//アウトライン
struct VS_OUT_OUTLINE
{
	float4 Pos		: SV_POSITION;
};