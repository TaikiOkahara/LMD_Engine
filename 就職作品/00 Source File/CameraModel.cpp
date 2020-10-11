#include "DIRECTOR.h"
#include "CameraModel.h"
#include "Scene.h"

//#define	M_CAM_POS_V_X		(0.0f)					// ���_�����ʒu(X���W)
//#define	M_CAM_POS_V_Y		(100.0f)				// ���_�����ʒu(Y���W)
//#define	M_CAM_POS_V_Z		(-200.0f)				// ���_�����ʒu(Z���W)
//#define	M_CAM_POS_R_X		(10.0f)					// �����_�����ʒu(X���W)
//#define	M_CAM_POS_R_Y		(100.0f)				// �����_�����ʒu(Y���W)
//#define	M_CAM_POS_R_Z		(10.0f)					// �����_�����ʒu(Z���W)
//#define	M_VIEW_ANGLE		(D3DX_PI/4)				// �r���[���ʂ̎���p
//#define	M_VIEW_ASPECT		((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
//#define	M_VIEW_NEAR_Z		(0.1f)											// �r���[���ʂ�NearZ�l
//#define	M_VIEW_FAR_Z		(100.0f)										// �r���[���ʂ�FarZ�l
//
//#define M_VALUE_MOVE_CAMERA		(0.1f)	//���_�ړ�����

static D3DXVECTOR2	model_trans_pos;
// ���_���̍쐬





CAMERA_MODEL::CAMERA_MODEL()
{
	ZeroMemory(this, sizeof(GRID));
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Rotate = D3DXVECTOR3(0, 0, 0);
}
//
CAMERA_MODEL::~CAMERA_MODEL()
{

	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);

	SAFE_RELEASE(m_pVertexBuffer);
}
//
//
//
HRESULT CAMERA_MODEL::Init()
{
	m_pDevice = Scene::m_pD3d->m_pDevice;
	m_pDeviceContext11 = Scene::m_pD3d->m_pDeviceContext;


	
	g_NumVertex = 16;

	GRID_3D	 pVtx[] =
	{
		m_Pos,

	};
	




	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(GRID_3D) * g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vrData;
	vrData.pSysMem = pVtx;


	if (FAILED(m_pDevice->CreateBuffer(&bd,				// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		&vrData,			// ���_�o�b�t�@�̎g�p�@�@
		&m_pVertexBuffer	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	)))					// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(GRID_3D);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	SetShaderDirectory();
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Line3D.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Line3D.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}


	


	return S_OK;

}
//
//
//
void CAMERA_MODEL::Uninit()
{

}
//
//
//
void CAMERA_MODEL::Update()
{

}
//
//
//
void CAMERA_MODEL::Draw()
{
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;



	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;

	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationY(&mYaw, m_Rotate.y);
	D3DXMatrixRotationX(&mPitch, m_Rotate.x);
	D3DXMatrixRotationZ(&mRoll, m_Rotate.z);
	D3DXMatrixTranslation(&mTran, m_Pos.x, m_Pos.y, m_Pos.z);
	mWorld = mScale * mYaw * mPitch * mRoll * mTran;

	//���[���h�A�J�����A�ˉe�s���n��
	CAMERA_DATA camera_data = Scene::m_Camera.GetCameraData();

	D3DXMATRIX m = mWorld * camera_data.View * camera_data.Projection;
	D3DXMatrixTranspose(&m, &m);

	SIMPLESHADER_CONSTANT_BUFFER scb;
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		

		scb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&scb), sizeof(scb));
		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}
	
	//m_pDeviceContext11->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		GRID_3D* pVtx = (GRID_3D*)pData.pData;

		pVtx[0].Pos = m_Pos;
		//pVtx[1].Pos = D3DXVECTOR3(0, 5, 0);
		pVtx[1].Pos =D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip),

		pVtx[2].Pos = m_Pos;
		pVtx[3].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[4].Pos = m_Pos;
		pVtx[5].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[6].Pos = m_Pos;
		pVtx[7].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		//--------------------------------------------------
		pVtx[8].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[9].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[10].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[11].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[12].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[13].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		pVtx[14].Pos = D3DXVECTOR3(
			FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);
		pVtx[15].Pos = D3DXVECTOR3(
			-FarClip * (FLOAT)tan(Angle / 2) * Aspect,
			-FarClip * (FLOAT)tan(Angle / 2),
			FarClip);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&pVtx), sizeof(pVtx));
		m_pDeviceContext11->Unmap(m_pVertexBuffer, 0);
	}

	
	
	
	
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	UINT stride = sizeof(GRID_3D);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pDeviceContext11->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//�v���~�e�B�u�������_�����O
	m_pDeviceContext11->Draw(g_NumVertex, 0);
}