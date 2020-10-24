#include "director.h"
#include "renderer.h"
//#include "tile.h"
#include "StaticMesh.h"
#include "stage.h"


//
//
//
void CStage::Init()
{
	
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//DoorWay//SMDoorWay.fbx");
	m_pMesh->LoadTexture("");


	m_Position = D3DXVECTOR3(-2.5f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f);
	//m_Scale = D3DXVECTOR3(0.01f, 0.009f, 0.009f);
	m_Scale = D3DXVECTOR3(1.0f, 0.9f, 0.9f);


	//�@���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//�V�F�[�_�[�쐬
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "Tile_PixelShader.cso");

	

	
	{
		////�@��
		VECTOR vector;
		vector.rotation = m_Rotation;
		vector.scale = m_Scale;
		vector.position = m_Position;
		m_Vector.push_back(vector);
	}

	InitInstance();
	UpdateInstance();//�������x�������邩��������Init�ɒu���Ă�
}


void CStage::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();

	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}


void CStage::Update()
{

}


void CStage::Draw()
{
	//�@�}�g���N�X�ݒ�
	//SetWorldMatrix();
	DrawInstance();


	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}