#include "../03 Shader File/FBX_Geometry.hlsl"


Texture2D g_tex			: register(t0);

Texture2D g_texColor	: register(t1);
Texture2D g_texNormal	: register(t2);
Texture2D g_texPosition	: register(t3);
Texture2D g_texOutline	: register(t4);
Texture2D g_texDepth    : register(t5);

SamplerState g_samLinear : register(s0);
//SamplerState g_samDeferredLinear : register(s1);
SamplerState g_samShadowLinear : register(s2);

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


/*float4 PLight(float4 Ambient, float4 Diffuse, float4 Specular, float3 Pos, float3 LightDir, float3 LPos, float range, float rate, float Intensity, float3 Normal, float2 Tex, float3 EyeVector)
{


	float NL_2 = saturate(dot(Normal, LightDir));//
	//�@�|�C���g���C�g������ɏ悶��
	//float4 diffuse = (Diffuse)*NL_2;
	float4 diffuse = (Diffuse / 2 + g_texColor.Sample(g_samLinear, Tex) / 2) * NL_2;

	//���ʔ��ˌ��@��
	float3 Reflect = normalize(2 * NL_2 * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, EyeVector)), 4) * Specular;

	float4 ambient = Ambient;


	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 Color = ambient + diffuse + specular;

	Color *= Intensity;

	//����
	float Distance = length(LPos - Pos) / range;// / 100;//�����Ŋ���l�ɂ���Ĕ͈͂�ύX�ł���
	Color *= pow(saturate(1 / Distance), rate);//�K����1�P�ʂ��猸���J�n�B�����̒l�Ō�������ύX�ł���

	return Color;
}*/

PS_OUT PS_FBX(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	//�J���[�e�N�X�`���[�֏o�� 
	Out.vColor = g_tex.Sample(g_samLinear, input.Tex) + float4(0.01, 0.01, 0.01, 0);

	//���[���h�@���e�N�X�`���[�֏o��
	float3 vNormal = input.WorldNormal;
	vNormal = vNormal * 0.5 + 0.5;
	Out.vNormal = normalize(float4(vNormal, 0));

	float  NL = saturate(dot(input.WorldNormal, input.LightVector));
	NL = NL * 0.9f + 0.1f;
	Out.vColor *= NL;


	//float3 Reflect = normalize(2 * NL * input.WorldNormal - input.LightVector);

	//���[���h���W�e�N�X�`���[�֏o��
	//Out.vPosition = input.ShadowPos;


	//�V���h�E�}�b�v�p�ɗ��p����
	//�e�̏���
	//Out.vPosition = Out.vColor;
	float4 shadow = float4(1, 1, 1, 0);
	Out.vPosition = shadow;
	input.LightTexCoord /= input.LightTexCoord.w;
	float TexValue = g_texDepth.Sample(g_samShadowLinear, input.LightTexCoord).r;
	float LightLength = input.LightViewPos.z / input.LightViewPos.w;
	if (TexValue + 0.000001 < LightLength)//���C�g�r���[�ł̒������Z���i���C�g�r���[�ł͎Օ���������j
	{
		//Out.vPosition /= 3;//�e
		Out.vPosition = (0, 0, 0, 0);
	}

	return Out;
}

//�A�j���[�V����---------------------------------------------------------------------------------------------

PS_OUT PS_FBX_ANIM(VS_OUT input)
{
	PS_OUT Out = (PS_OUT)0;

	//�J���[�e�N�X�`���[�֏o�� 
	Out.vColor = g_tex.Sample(g_samLinear, input.Tex) + float4(0.01, 0.01, 0.01, 0);

	//���[���h�@���e�N�X�`���[�֏o��
	float3 vNormal = input.WorldNormal;
	vNormal = vNormal * 0.5 + 0.5;
	Out.vNormal = normalize(float4(vNormal, 0));

	float  NL = saturate(dot(input.WorldNormal, input.LightVector));
	NL = NL * 0.9f + 0.1f;
	Out.vColor *= NL;


	//float3 Reflect = normalize(2 * NL * input.WorldNormal - input.LightVector);

	//���[���h���W�e�N�X�`���[�֏o��
	//Out.vPosition = input.ShadowPos;


	//�V���h�E�}�b�v�p�ɗ��p����
	//�e�̏���
	//Out.vPosition = Out.vColor;
	float4 shadow = float4(1, 1, 1, 0);
	Out.vPosition = shadow;
	input.LightTexCoord /= input.LightTexCoord.w;
	float TexValue = g_texDepth.Sample(g_samShadowLinear, input.LightTexCoord).r;
	float LightLength = input.LightViewPos.z / input.LightViewPos.w;
	if (TexValue + 0.000001 < LightLength)//���C�g�r���[�ł̒������Z���i���C�g�r���[�ł͎Օ���������j
	{
		//Out.vPosition /= 3;//�e
		Out.vPosition = (0, 0, 0, 0);
	}

	return Out;
}