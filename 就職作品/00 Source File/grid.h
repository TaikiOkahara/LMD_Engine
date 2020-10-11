#pragma once
//
//
//
class GRID
{
	//���_�̍\����
	struct GRID_3D
	{
		D3DXVECTOR3 Pos; //�ʒu
	};
public:

	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
	};
public:
	HRESULT Init();
	void	Uninit(void);
	void	Update(void);
	void	Draw();

	D3DXVECTOR3		m_Pos;// �n�ʂ̈ʒu
	D3DXVECTOR3		m_Scale;//
	float m_Yaw, m_Pitch, m_Roll;

	GRID();
	~GRID();

private:

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext *m_pDeviceContext11 = NULL;

	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	//�����f������	
	ID3D11Buffer* m_pVertexBuffer = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	//ID3D11Buffer* m_pIndexBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	//ID3D11SamplerState* m_pSampleLinear;//�e�N�X�`���[�̃T���v���[
	//ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`��


	int g_NumVertex;
	int g_NumIndex;
};