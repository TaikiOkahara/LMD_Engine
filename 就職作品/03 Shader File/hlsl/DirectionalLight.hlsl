//�O���[�o��
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);



cbuffer per_mesh:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ������s��
};

cbuffer per_material:register(b1)
{
	float4 g_Ambient;//����
	float4 g_Diffuse; //�g�U���ˌ�
	float4 g_Specular;//���ʔ��ˌ�
};

//�O���[�o��
cbuffer per_frame:register(b2)
{
	float4 g_vLightDir;//�f�B���N�V���i�����C�g���i���C�g�̕����j
	float4 g_vEye;//�J�����ʒu
	float4 g_fIntensity;//�f�B���N�V���i�����C�g���i���C�g�̖��邳�j
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{	
	float4 Pos		: SV_POSITION;
	float3 Light	: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
	//float3 HalfVetor : TEXCOORD2;
	float3 EyeVector: TEXCOORD2;
	float2 Tex		: TEXCOORD3;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	output.Pos=mul(Pos,g_mWVP);
	//�@�������f���̎p���ɍ��킹��(���f������]����Ζ@������]������K�v�����邽�߁j
	output.Normal=mul(Norm, (float3x3)g_mW);
	//���C�g���� �@�������|�C���g�I�I�@�f�B���N�V���i���͂ǂ��ł����������B�ʒu�͖��֌W
	output.Light=normalize(g_vLightDir);
	//�����x�N�g���@���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	float3 PosWorld = mul(Pos,g_mW);
	output.EyeVector = normalize(g_vEye - PosWorld);
	//�e�N�X�`���[���W
	output.Tex=Tex;

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	//�����@��
	float4 ambient=g_Ambient;
	//�g�U���ˌ��@��
	float NL=saturate(dot(input.Normal, input.Light));
	float4 diffuse = (g_Diffuse/2+g_texColor.Sample( g_samLinear, input.Tex )/2)*NL;
	//���ʔ��ˌ��@��
	float3 reflect=normalize(2*NL*input.Normal-input.Light); 
	float4 specular=pow(saturate(dot(reflect,input.EyeVector)), 4)*g_Specular;
	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 color=ambient+diffuse+specular;
	//���C�g���x�𔽉f
	color*=g_fIntensity.x;

	return color;
}