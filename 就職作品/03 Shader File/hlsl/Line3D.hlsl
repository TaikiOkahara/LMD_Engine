//�O���[�o��
cbuffer global: register(b0)
{
	matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};
//�\����
struct PS_INPUT
{
	float4 Normal : SV_POSITION;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS( float4 Pos : POSITION ) 
{
	PS_INPUT Out;
	Out.Normal =mul(Pos,g_WVP);
	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	return float4(0,1,0,1);
}