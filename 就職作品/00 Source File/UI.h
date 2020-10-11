/*�uUI.h�v=============================================
�@�EGUI�A�r���{�[�h�̎����N���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once

//UI�i���o�[
enum UI_TYPE
{
	BILLBOARD,
	GUI,

	TYPE_MAX
};


//
//
//�@�N���X
class UI
{
public:
	//���_�̍\����
	struct UIVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
		D3DXVECTOR2 UV; //�e�N�X�`���[���W
	};
	//
	struct UI_SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;

	};
public:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Scale;
	float m_Yaw, m_Pitch, m_Roll;
	
	HRESULT Draw();	//	 GUI
	HRESULT Draw(D3DXMATRIX& mView, D3DXMATRIX& mProj);//�@�r���{�[�h
	HRESULT Init(LPSTR,D3DXVECTOR2 pos);//�@GUI	
	HRESULT Init(LPSTR,D3DXVECTOR3 pos);//�@�r���{�[�h

	UI();	//�@�R���X�g���N�^
	~UI();	//�@�f�X�g���N�^
private:

	HWND m_hWnd;//�@�E�B���h�E�n���h��
	ID3D11Device* m_pDevice;//�@�f�o�C�X�|�C���^
	ID3D11DeviceContext* m_pDeviceContext;//�@�f�o�C�X���|�C���^
	//�@�o�b�N�o�b�t�@
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	//�@�o�b�t�@
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j

	//�@�����o�֐�
	HRESULT InitShader(LPSTR , LPSTR , LPSTR , void** , ID3DBlob** );
	HRESULT InitModel();
	VOID RenderSprite(D3DXMATRIX);
	int m_Type;//UI�̎�ށiGUI���A�r���{�[�h���j
};
