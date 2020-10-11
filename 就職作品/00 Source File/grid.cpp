#include "DIRECTOR.h"
#include "grid.h"
#include "Scene.h"

#define	GRID_SIZE		(10.0f)						// �n�ʂ̕�
#define GRID_LINE		(10)
#define VERTEX_NUM		((GRID_LINE * 2) * 2 + 4)

GRID::GRID()
{

	ZeroMemory(this, sizeof(GRID));
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	m_Yaw = 0;
	m_Pitch = 0;
	m_Roll = 0;
}

GRID::~GRID()
{
	//SAFE_DELETE(pD3d);
	//SAFE_DELETE(m_pDeviceContext11);
	
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);

	SAFE_RELEASE(m_pVertexBuffer);
	//SAFE_RELEASE(m_pTexture);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT GRID::Init()
{
	m_pDevice = Scene::m_pD3d->m_pDevice;
	m_pDeviceContext11 = Scene::m_pD3d->m_pDeviceContext;
	

	// ���_���̍쐬


	
	g_NumVertex = VERTEX_NUM;

	GRID_3D	 pVtx[(GRID_LINE * 2) * 2 + 4];//(+4�͍Ō��2�{)
	for (int x = 0; x < GRID_LINE + 1; x++)
	{
		for (int z = 0; z < 2; z++)
		{
			pVtx[(x * 2) + z].Pos.x = m_Pos.x + (GRID_SIZE / 2) - ((GRID_SIZE / GRID_LINE * 1.0f) * x);
			pVtx[(x * 2) + z].Pos.y = m_Pos.y;
			pVtx[(x * 2) + z].Pos.z = m_Pos.z - (GRID_SIZE / 2) + (GRID_SIZE * z);
		}
	}
	for (int z = GRID_LINE + 1; z < (GRID_LINE * 2) + 2; z++)
	{
		for (int x = 0; x < 2; x++)
		{
			pVtx[(z * 2) + x].Pos.x = m_Pos.x - (GRID_SIZE / 2) + (GRID_SIZE * x);
			pVtx[(z * 2) + x].Pos.y = m_Pos.y;
			pVtx[(z * 2) + x].Pos.z = m_Pos.z + (GRID_SIZE/GRID_LINE) + (GRID_SIZE / 2) - ((GRID_SIZE / GRID_LINE * 1.0f) * (z - GRID_LINE));
		}
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(GRID_3D) * 	g_NumVertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
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
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
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
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;//D3D11_USAGE_DEFAULT


	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}



	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void GRID::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void GRID::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void GRID::Draw()
{

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;

	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX mWorld, mTran, mYaw, mPitch, mRoll, mScale;

	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationY(&mYaw, m_Yaw);
	D3DXMatrixRotationX(&mPitch, m_Pitch);
	D3DXMatrixRotationZ(&mRoll, m_Roll);
	D3DXMatrixTranslation(&mTran, m_Pos.x, m_Pos.y, m_Pos.z);
	mWorld = mScale * mYaw*mPitch*mRoll*mTran;

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
	m_pDeviceContext11->Draw(g_NumVertex,0);

}