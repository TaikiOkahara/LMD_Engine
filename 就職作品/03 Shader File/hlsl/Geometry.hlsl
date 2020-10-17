
//�o�[�e�b�N�X�o�b�t�@�[�o��
struct VS_OUT
{
	float4 Pos			: SV_POSITION;
	float4 WorldPos		: POSITION;
	float3 WorldNormal	: NORMAL;
    float3 WorldTangent : TANGENT;
    float3 WorldBinormal: BINORMAL;
	float2 Tex			: TEXCOORD0;
	//float3 LightVector  : TEXCOORD1;
	//float4 LightTexCoord: TEXCOORD2;
	//float4 LightViewPos : TEXCOORD3;
	

	float3 EyeVector	: TEXCOORD5;
};
//�s�N�Z���V�F�[�_�[�o��
struct PS_OUT
{
	float4 vColor		: SV_Target0;
	float4 vNormal		: SV_Target1;
	float4 vPosition	: SV_Target2;
};

struct VS_LIGHT_OUT
{
    float4 Pos : SV_POSITION;
};

//struct VS_LINE_OUT
//{
//    float4 Pos		: SV_POSITION;
//    float4 WorldPos : TEXCOORD0;
//};


//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
//struct VS_OUTPUT_DEPTH//�[�x�e�N�X�`���[�����_�[�p
//{
//	float4 Pos		: SV_POSITION;
//	float4 Depth	: POSITION;
//};



//�A�E�g���C��
//struct VS_OUT_OUTLINE
//{
//	float4 Pos		: SV_POSITION;
//};