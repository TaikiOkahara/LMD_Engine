#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include "pots.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CPots::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Pots//PotB.fbx");
	m_pMesh->LoadTexture("");



	m_Collision.Init(D3DXVECTOR3(1.0f,3.0f,1.0f),D3DXVECTOR3(0,1.5f,0));



	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");

	

	VECTOR vector;
	
	vector.scale = D3DXVECTOR3(1,1,1);


	{
		//　マトリクス設定
		vector.position = D3DXVECTOR3(4.0f, 0.0f, -5.8f);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vector.push_back(vector);

		vector.position = D3DXVECTOR3(4.0f, 0.0f, -7.2f);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vector.push_back(vector);
	}

	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}

void CPots::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CPots::Update()
{

}

void CPots::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if(!isEnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CPots::Imgui()
{
	static bool show_pillar_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (CInput::KeyTrigger(DIK_F1))
		show_pillar_window = !show_pillar_window;

	if (show_pillar_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Pots", &lw_is_open, lw_flag);

		ImGui::Checkbox("isEnableCollision", &isEnableCollision);

		int count = m_MeshCount;
		ImGui::InputInt("MeshCount", &count, 1);
	

		ImGui::End();
	}
}