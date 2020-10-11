#include "../03 Shader File/FBX_Geometry.hlsl"


cbuffer WorldBuffer : register(b0) {
	matrix g_mWorld;
	matrix g_mWIT;
}

cbuffer ViewBuffer : register(b1) {
	matrix g_mView;
}

cbuffer ProjectionBuffer : register(b2) {
	matrix g_mProj;
}


cbuffer EyeBuffer : register(b3) {
	float4 g_vEye;
}

//�@�|�C���g���C�g
cbuffer PointLightBuffer : register(b4)
{
	float4  g_vLightPos[10];		//	���W

	float4	g_fIntensity_point[10];	//�@���邳
	float4	g_Range[10];			//�@�͈�
	float4	g_AttRate[10];			//�@������

	float4	g_fNumLight;			//�@���C�g����

	//float Dummy;
}

//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b5)
{
	float4 g_vLight;//���C�g�̍��W
	float4 g_fIntensity;//���邳	
	//matrix g_mWLP;
	//matrix g_mWLPT;
	matrix g_mLightView;
}

cbuffer MaterialBuffer : register(b6)
{
	float4 g_Ambient;//�A���r�G���g��
	float4 g_Diffuse;//�g�U����(�F�j
	float4 g_Specular;//���ʔ���
}

cbuffer ClusterBuffer : register(b7)
{
	matrix g_Cluster[127];
	
}


VS_OUT VS_FBX(
	float4 Pos		: POSITION,
	float4 Normal	: NORMAL,
	float2 Tex		: TEXCOORD,
	float4 Tangent	: TANGENT)
{
	VS_OUT output = (VS_OUT)0;

	//�A�j���[�V�����̏ꍇ�����Ŏp�����䂷��



	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.WorldNormal = normalize(mul(Normal.xyz, (float3x3)g_mWorld));
	output.Pos = mul(Pos, WVP);
	output.WorldPos = mul(Pos, g_mWorld);
	output.Tex = Tex;
	output.LightVector = normalize(g_vLight).xyz;
	//output.ShadowPos = Pos;

	matrix WLP;
	WLP = mul(g_mWorld, g_mLightView);
	WLP = mul(WLP, g_mProj);


	matrix mTex;
	mTex._11 = 0.5f;
	mTex._12 = 0.0f;
	mTex._13 = 0.0f;
	mTex._14 = 0.0f;

	mTex._21 = 0.0f;
	mTex._22 = -0.5f;
	mTex._23 = 0.0f;
	mTex._24 = 0.0f;

	mTex._31 = 0.0f;
	mTex._32 = 0.0f;
	mTex._33 = 1.0f;
	mTex._34 = 0.0f;

	mTex._41 = 0.5f;
	mTex._42 = 0.5f;
	mTex._43 = 0.0f;
	mTex._44 = 1.0f;

	matrix WLPT;
	WLPT = mul(WLP, mTex);


	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`���[���W
	output.LightTexCoord = mul(Pos, WLPT);//���̓_���A���C�g�r���[�ł������Ƃ��̈ʒu���킩��
	//���C�g�r���[�ɂ�����ʒu(�ϊ���)
	output.LightViewPos = mul(Pos, WLP);



	return output;
}


//�A�j���[�V����---------------------------------------------------------------------------------------------
matrix FetchClusterMatrix(uint index)
{
	//return (index == 254) ? g_Cluster[254] : g_Cluster[index];
	return g_Cluster[index];
}



VS_OUT VS_FBX_ANIM(
	float4 Pos			: POSITION,
	float4 Normal		: NORMAL,
	float2 Tex			: TEXCOORD,
	float4 Tangent		: TANGENT,
	uint4  ClusterIndex	: CLUSTER_INDEX,
	float4 ClusterWeight: CLUSTER_WEIGHT)
{
	VS_OUT output = (VS_OUT)0;

	float4 localPos = Pos;
	float3 localNormal = Normal.xyz;



	//�A�j���[�V�����̏ꍇ�����Ŏp�����䂷��-------------------

	matrix tmp_mat;
	float weight = 0.0f;
	matrix mat;
	tmp_mat._11 = 1.0f;
	tmp_mat._12 = 0.0f;
	tmp_mat._13 = 0.0f;
	tmp_mat._14 = 0.0f;

	tmp_mat._21 = 0.0f;
	tmp_mat._22 = 1.0f;
	tmp_mat._23 = 0.0f;
	tmp_mat._24 = 0.0f;

	tmp_mat._31 = 0.0f;
	tmp_mat._32 = 0.0f;
	tmp_mat._33 = 1.0f;
	tmp_mat._34 = 0.0f;

	tmp_mat._41 = 0.0f;
	tmp_mat._42 = 0.0f;
	tmp_mat._43 = 0.0f;
	tmp_mat._44 = 1.0f;
	//x,y,z,w��z��ƍl����
	//�N���X�^�P
	weight = ClusterWeight.x;
	mat = FetchClusterMatrix(ClusterIndex.x);
	output.Pos += weight * mul(localPos, mat);
	tmp_mat += weight * mat;
	output.WorldNormal += weight * mul(localNormal, (float3x3)mat);
	
	//�N���X�^�Q
	weight = ClusterWeight.y;
	mat = FetchClusterMatrix(ClusterIndex.y);
	output.Pos += weight * mul(localPos, mat);
	tmp_mat += weight * mat;
	output.WorldNormal += weight * mul(localNormal, (float3x3)mat);
	
	//�N���X�^�R�@
	weight = ClusterWeight.z;
	mat = FetchClusterMatrix(ClusterIndex.z);
	output.Pos += weight * mul(localPos, mat);
	tmp_mat += weight * mat;
	output.WorldNormal += weight * mul(localNormal, (float3x3)mat);
	
	//�N���X�^�S�@
	weight = ClusterWeight.w;
	mat = FetchClusterMatrix(ClusterIndex.w);
	output.Pos += weight * mul(localPos, mat);
	tmp_mat += weight * mat;
	output.WorldNormal += weight * mul(localNormal, (float3x3)mat);

	localPos = mul(float4(localPos.xyz,1), tmp_mat);
	localPos.x *= -1;
	output.Pos = localPos;
	/*output.Pos = Pos;
	output.WorldNormal = Normal;*/
	//--------------------------------------------------------

	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);

	output.WorldNormal = normalize(mul(output.WorldNormal, (float3x3)g_mWorld));
	output.WorldPos = mul(output.Pos, g_mWorld);
	output.Tex = Tex;
	output.LightVector = normalize(g_vLight).xyz;
	//output.ShadowPos = Pos;

	matrix WLP;
	WLP = mul(g_mWorld, g_mLightView);
	WLP = mul(WLP, g_mProj);


	matrix mTex;
	mTex._11 = 0.5f;
	mTex._12 = 0.0f;
	mTex._13 = 0.0f;
	mTex._14 = 0.0f;

	mTex._21 = 0.0f;
	mTex._22 = -0.5f;
	mTex._23 = 0.0f;
	mTex._24 = 0.0f;

	mTex._31 = 0.0f;
	mTex._32 = 0.0f;
	mTex._33 = 1.0f;
	mTex._34 = 0.0f;

	mTex._41 = 0.5f;
	mTex._42 = 0.5f;
	mTex._43 = 0.0f;
	mTex._44 = 1.0f;

	matrix WLPT;
	WLPT = mul(WLP, mTex);


	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`���[���W
	output.LightTexCoord = mul(output.Pos, WLPT);//���̓_���A���C�g�r���[�ł������Ƃ��̈ʒu���킩��
	//���C�g�r���[�ɂ�����ʒu(�ϊ���)
	output.LightViewPos = mul(output.Pos, WLP);

	output.Pos = mul(output.Pos, WVP);


	return output;
}

//float4 Animation(float4 Pos,uint4 ClusterIndex, float4 ClusterWeight)
//{
//	float weight = 0.0f;
//	matrix = mat;
//	//x,y,z,w��z��ƍl����
//	//�N���X�^�P
//	weight = ClusterWeight.x;
//	mat = FetchClusterMatrix(ClusterIndex.x)
//	Pos += weight * mul()
//	//�N���X�^�Q
//	//�N���X�^�R�@
//	//�N���X�^�S�@
//
//}

