//�f�B�t���[�Y�F���e�N�X�`���[�F�ɒu�������@

//�O���[�o��
Texture2D g_texColor: register(t0);
Texture2D g_texNormal: register(t1);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer per_WorldMatrix: register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWIT;//���[���h�s��̋t�s��̓]�u�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

cbuffer per_Material: register(b1)
{
	float4 g_Ambient;//�A���r�G���g��
	float4 g_Diffuse;//�g�U����(�F�j
	float4 g_Specular;//���ʔ���
};

//�@�f�B���N�V���i�����C�g + �J�����ʒu
cbuffer per_Light : register(b2)
{
	float4 g_vLight;//���C�g�̕����x�N�g��
	float4 g_fIntensity;//���邳
	float4 g_vEye;//���_�A�J�����ʒu
}

//�@�|�C���g���C�g
cbuffer per_Light_2 : register(b3)
{
	float4  g_vLightPos[10];		//	���W

	//�@x�����g�p���Ă��Ȃ��̂ɖ���float4�ĂԂ̂͂߂�ǂ�����
	//�@��Buffer�ɕϊ�����Ƃ��ɂ��ꂼ��̎��Ɋ��蓖�Ă�΁A����������
	float4	g_fIntensity_point[10];	//�@���邳
	float4	g_Range[10];			//�@�͈�
	float4	g_AttRate[10];			//�@������

	float4	g_fNumLight;			//�@���C�g����

	//float4	Dummy = 0;
}

//�\����
struct VS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float3 WorldPos		: POSITION;
	float3 Light		: TEXCOORD0;//�@�f�B���N�V���i�����C�g
	float3 Normal		: TEXCOORD1;
	float3 HalfVector	: TEXCOORD2;
	float3 EyeVector	: TEXCOORD3;
	float2 Tex			: TEXCOORD4;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//�@������FbxVertex�̗v�f�Ɠ���
VS_OUTPUT VS(
	float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 Tangent : TANGENT)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 worldPos = mul(Pos, g_mW).xyz;
	float3 LightVector = normalize(g_vLight).xyz;//
	float3 EyeVector = normalize(g_vEye.xyz - worldPos);
	float3 HalfVector = normalize(LightVector + EyeVector);//�@�m�[�}���}�b�s���O�ɂ͕K�{

	float3 T = mul(Tangent.xyz, (float3x3)g_mWIT);
	float3 N = mul(Normal.xyz, (float3x3)g_mWIT);
	float3 B = cross(N, T) * Tangent.w;

	float3x3 TangentToObject = float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z);//�@�m�[�}���}�b�s���O�ɂ͕K�{
	output.Normal = mul(Normal,g_mW);
	// ���[�J�����W * ���[���h���W�ϊ��s��
	output.WorldPos = mul(Pos, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;
	output.HalfVector = mul(HalfVector, TangentToObject);
	output.Light = mul(LightVector, TangentToObject);

	return output;
}
//
//
//
float4 PLight(float4 Ambient,float4 Diffuse, float4 Specular,float3 Pos,float3 LightDir,float3 LPos,float range,float rate,float Intensity,float3 Normal,float2 Tex, float3 EyeVector)
{

	
	float NL_2 = saturate(dot(Normal, LightDir));//
	//�@�|�C���g���C�g������ɏ悶��
	//float4 diffuse = (Diffuse)*NL_2;
	float4 diffuse = (Diffuse / 2 + g_texColor.Sample(g_samLinear, Tex)/2)*NL_2;

	//���ʔ��ˌ��@��
	float3 Reflect = normalize(2 * NL_2 * Normal -LightDir);
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
	float3 Light = normalize(input.Light);
	float3 Normal = normalize(g_texNormal.Sample(g_samLinear, input.Tex).rgb * 2.0f - 1.0f);
	float NH = saturate(dot(Normal, HalfVector));
	//�@���ʔ��ˌ��@��
	float NL = saturate(dot(Normal, Light));

	//float Power = (NL == 0.0f) ? 0.0f : pow(NH,4);

	//�@�f�B���N�V���i�����C�g
	float4 Diffuse = ((g_Diffuse *NL) / 2);	
	float4 Ambient = g_Ambient;		
	//float4 Specular = g_Specular; *Power;
	float3 Reflect = normalize(2 * NL * Normal - Light);
	float4 Specular = pow(saturate(dot(Reflect, input.EyeVector)), 4) * g_Specular;
	
	float4 DirectionalColor = Ambient + Diffuse + Specular;
	DirectionalColor *= g_fIntensity.x;

	//�@�|�C���g���C�g
	float3 WorldPos = normalize(input.WorldPos);
	float4 PointColor;
	for (int i = 0; i < g_fNumLight.x; i++)
	{		
		float3 LightDir = (normalize(g_vLightPos[i] - WorldPos));
		PointColor += PLight(g_Ambient, g_Diffuse, g_Specular, input.WorldPos,LightDir, g_vLightPos[i], g_Range[i].x, g_AttRate[i].x, g_fIntensity_point[i].x,input.Normal,input.Tex,input.EyeVector);
	}


	//�@���̓f�B���N�V���i�����C�g�ƃ|�C���g���C�g�̌��ʂ��������������ŁA
	//�@�|�C���g���C�g�̂ق��̓m�[�}���}�b�s���O����Ă��Ȃ�
	//�@���z�͂��̓�̃��C�g�̍������ꂽ���̂���m�[�}���}�b�s���O������
	//
	float4 FinalColor = PointColor + DirectionalColor;//�@�|�C���g���C�g�ƃf�B���N�V���i�����C�g������


	return FinalColor;
}