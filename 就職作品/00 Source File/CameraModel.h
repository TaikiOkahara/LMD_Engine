#pragma once

class CAMERA_MODEL
{
	//���_�̍\����
	struct GRID_3D
	{
		D3DXVECTOR3 Pos; //�ʒu
	};
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
	};

public:
	CAMERA_MODEL();
	~CAMERA_MODEL();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3		m_Pos;// �n�ʂ̈ʒu
	D3DXVECTOR3		m_Scale;//
	D3DXVECTOR3		m_Rotate;
private:

	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext11 = NULL;

	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;
	ID3D11PixelShader* m_pPixelShader = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;
	//�����f������	
	ID3D11Buffer* m_pVertexBuffer = NULL;	// ���_�o�b�t�@�ւ̃|�C���^


	int g_NumVertex;
	int g_NumIndex;

	float FarClip = 10.0f;
	float NearClip = 0.1f;
	float Aspect = 1280 / 720;
	float Angle = D3DX_PI/4;
};