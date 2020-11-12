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
	m_pMesh->LoadModel("../02 Visual File//DoorWay//doorWay.fbx");
	m_pMesh->LoadTexture("");


	m_Transform.position = D3DXVECTOR3(-2.5f, 0.0f, 0.0f);
	m_Transform.rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
	//m_Scale = D3DXVECTOR3(1.0f, 0.9f, 0.9f);
	m_Transform.scale = D3DXVECTOR3(0.85f, 0.85f, 0.85f);


	
	//�V�F�[�_�[�쐬
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	

	
	{
		////�@��
		VECTOR vector;
		vector.rotation = m_Transform.rotation;
		vector.scale = m_Transform.scale;
		vector.position = m_Transform.position;
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
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);
}