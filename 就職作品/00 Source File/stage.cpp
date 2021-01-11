#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include "stage.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CStage::Init()
{
	
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//DoorWay//doorWayPBR_NoTex.fbx");
	m_pMesh->LoadTexture("../02 Visual File//DoorWay//texture");


	m_Transform.position = D3DXVECTOR3(-2.5f, 0.0f, 0.0f);
	m_Transform.rotation = D3DXVECTOR3(0.0f, D3DX_PI/2, 0.0f);
	
	m_Transform.scale = D3DXVECTOR3(1.5, 0.85f,1);

	m_Collision.Init(D3DXVECTOR3(0.5, 5, 5), D3DXVECTOR3(0, 2.5, 0));
	
	
	{
		////　扉
		TRANSFORM vector;
		vector.rotation = m_Transform.rotation;
		vector.scale = m_Transform.scale;
		vector.position = m_Transform.position;
		m_TransformList.push_back(vector);
	}

	InitInstance();
}


void CStage::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();

	m_Collision.Uninit();

}


void CStage::Update()
{
	UpdateInstance();

}


void CStage::Draw()
{
	//　マトリクス設定
	//SetWorldMatrix();
	DrawInstance();


	//RENDERER::m_pDeviceContext->VSSetShader(RENDERER::m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pCommonPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CStage::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("DoorWay", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}