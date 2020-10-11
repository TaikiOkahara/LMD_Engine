//�O���[�o��
Texture2D g_texColor: register(t0);
Texture2D g_texNormal: register(t1);
SamplerState g_samLinear : register(s0);

#define PI 3.1415926535897

cbuffer WorldBuffer : register(b0)
{
	matrix g_mWorld;//���[���h�s��
	matrix g_mWIT;//���[���h�̋t�̓]�u�s��
	//matrix g_mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
}

cbuffer ViwBuffer : register(b1)
{
	matrix g_mView;//���[���h�s��
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix g_mProj;//���[���h�s��
}

cbuffer MaterialBuffer: register(b3)
{
	float4 g_Ambient;//�A���r�G���g��
	float4 g_Diffuse;//�g�U����(�F�j
	float4 g_Specular;//���ʔ���
}

//�@�f�B���N�V���i�����C�g
cbuffer DirectionalLightBuffer : register(b4)
{
	float4 g_vLight;//���C�g�̕����x�N�g��
	float4 g_fIntensity;//���邳	
	float4 g_vEye;//���_�A�J�����ʒu
}

//�@�|�C���g���C�g
cbuffer PointLightBuffer : register(b5)
{
	float4  g_vLightPos[10];		//	���W

	float4	g_fIntensity_point[10];	//�@���邳
	float4	g_Range[10];			//�@�͈�
	float4	g_AttRate[10];			//�@������

	float4	g_fNumLight;			//�@���C�g����

	//float Dummy;
}

struct VS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float3 WorldPos		: POSITION;
	float3 Light		: TEXCOORD0;//	
	float3 Normal		: TEXCOORD1;//	�@��
	float3 HalfVector	: TEXCOORD2;//�@�m�[�}���}�b�v�ɕK�v�i�ڐ��H�j
	//float3 EyeVector	: TEXCOORD3;//�@�J�������_
	float2 Tex			: TEXCOORD4;
	float3 LightTanDir  : TEXCOORD5;
};


VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	matrix WVP;
	WVP = mul(g_mWorld, g_mView);
	WVP = mul(WVP, g_mProj);


	//�@�ˉe�ϊ�
	float3 worldPos = mul(Pos, g_mWorld).xyz;

	
	float3 LightVector = -g_vLight.xyz;
	
	float3 EyeVector = normalize(g_vEye.xyz - worldPos);
	//float3 EyeVector = normalize(g_vEye.xyz);

	float3 HalfVector = normalize(LightVector + EyeVector);//���ʔ��ˌ��̂��߂̃n�[�t�x�N�g��

	float3 T = mul(Tangent.xyz, (float3x3)g_mWIT);
	float3 N = mul(Normal.xyz, (float3x3)g_mWIT);
	float3 B = cross(N, T) * Tangent.w;	//

	float3x3 TangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);
	//�@�@�������f���̎p���ɍ��킹��i���f������]����Ζ@������]������K�v�����邽�߁j
	output.Normal = Normal.xyz;// normalize(mul(Normal, g_mWorld));

	
	output.WorldPos = mul(Pos, g_mWorld).xyz;
	output.Pos = mul(Pos, WVP);
	output.Tex = Tex;

	//VS���PS�̂ق������|�I�ɏ������ׂ������̂ŁA����s���v�Z�͂ł��邾��VS���ōs��
	//�@���̐��K����CPU���Ŏ��O�ɍs���Ă���

	

	output.HalfVector = HalfVector;
	//output.HalfVector = mul(HalfVector, TangentToObject);
	output.Light = LightVector;
	
	output.LightTanDir = mul(LightVector, TangentToObject);
	
	return output;
}

//
//
//
float4 PLight(float4 Ambient, float4 Diffuse, float4 Specular, float3 Pos, float3 LightDir, float3 LPos, float range, float rate, float Intensity, float3 Normal, float2 Tex, float3 EyeVector)
{


	float NL_2 = saturate(dot(Normal, LightDir));//
	//�@�|�C���g���C�g������ɏ悶��
	//float4 diffuse = (Diffuse)*NL_2;
	float4 diffuse = (Diffuse / 2 + g_texColor.Sample(g_samLinear, Tex) / 2)*NL_2;

	//���ʔ��ˌ��@��
	float3 Reflect = normalize(2 * NL_2 * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, EyeVector)), 4)*Specular;

	float4 ambient = Ambient;


	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 Color = ambient + diffuse + specular;

	Color *= Intensity;

	//����
	float Distance = length(LPos - Pos)/ range;// / 100;//�����Ŋ���l�ɂ���Ĕ͈͂�ύX�ł���
	Color *= pow(saturate(1 / Distance), rate);//�K����1�P�ʂ��猸���J�n�B�����̒l�Ō�������ύX�ł���

	return Color;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 HalfVector = normalize(input.HalfVector);

	float3 Normal = normalize(g_texNormal.Sample(g_samLinear, input.Tex).rgb * 2.0f - 1.0f);
	//clip(length(Normal) - 0.1);
	
	float3 Bright = (dot(input.LightTanDir,Normal));
	Bright = max(0.0f, Bright);
	
	float NH = saturate(dot(Normal, HalfVector));
	//�@�g�U���ˌ�

	//float NL = saturate(dot(input.Normal,input.Light));
	float NL = saturate(dot(input.Normal, input.Light));
	NL = NL * 0.9f + 0.1f;
	//float NV = saturate(dot(Normal,input.EyeVector));
	//float VH = saturate(dot(input.EyeVector, input.HaltVector));
	//float LV = saturate(dot(Light, input.EyeVector));

	
	float3 LightColor = float3(1, 1, 1);
	float3 irradistance = NL * LightColor;
	//float3 irradistance = NL * LightColor;
	//return float4(irradistance, 1);

	//�@���ʔ��ˌ�
	float Power = (NL == 0.0f) ? 0.0f : pow(NH,1);//����4�́A���ʔ��ˋ��x�B0����10�܂ŁB�傫���قǍL���肪�V���[�v
	//Eye,Half

	//float3 Diffuse = (g_Diffuse/2 + g_texColor.Sample(g_samLinear, input.Tex)/2);//���ʔ��ˌ��͍��͏����Ă�
	float3 Diffuse = (g_texColor.Sample(g_samLinear, input.Tex).xyz);//���ʔ��ˌ��͍��͏����Ă�
	float3 Specular;// = Diffuse.xyz;

	Diffuse = Diffuse * irradistance;// / PI;
	

	float4 Ambient = g_Ambient;			//�@�f�B���N�V���i�����C�g
	Specular = g_Specular.xyz * Power;
	
	return float4(Diffuse, 1);


	//float4 DirectionalColor = Ambient;// +Diffuse + Specular;

	////DirectionalColor *= g_fIntensity.x;//�@���͂Ȃ��ɂ���	

	//float3 WorldPos = normalize(input.WorldPos);
	//float4 PointColor;
	//for (int i = 0; i < g_fNumLight.x; i++)
	//{
	//	float3 LightDir = normalize(g_vLightPos[i].xyz - WorldPos);
	//	PointColor += PLight(g_Ambient, g_Diffuse, g_Specular, input.WorldPos,LightDir, g_vLightPos[i].xyz, g_Range[i].x, g_AttRate[i].x, g_fIntensity_point[i].x,Normal,input.Tex,input.EyeVector);
	//}


	////�@���̓f�B���N�V���i�����C�g�ƃ|�C���g���C�g�̌��ʂ��������������ŁA
	////�@�|�C���g���C�g�̂ق��̓m�[�}���}�b�s���O����Ă��Ȃ�
	////�@���z�͂��̓�̃��C�g�̍������ꂽ���̂���m�[�}���}�b�s���O������
	////
	//float4 FinalColor = PointColor + DirectionalColor;//�@�|�C���g���C�g�ƃf�B���N�V���i�����C�g������


	//return FinalColor;
}
