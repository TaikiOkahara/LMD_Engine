#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include "pillar.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CPillar::Init()
{
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//Pillar//untitled.fbx");
	m_pMesh->LoadTexture("");



	m_Collision.Init(D3DXVECTOR3(1.0f, 3.25f, 1.0f), D3DXVECTOR3(0, 1.7f, 0));



	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &RENDERER::m_pCommonVertexLayout, nullptr, 0, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PixelShader.cso");



	VECTOR vector;

	vector.scale = D3DXVECTOR3(1, 1, 1);


	for (int i = 0; i < 6; i++)
	{
		//　マトリクス設定
		vector.position = D3DXVECTOR3(-0.5f, 0.0f,5.0f +  5.0f * i);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vector.push_back(vector);

		//　マトリクス設定
		vector.position = D3DXVECTOR3(-4.5f, 0.0f, 5.0f + 5.0f * i);
		vector.rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vector.push_back(vector);
	}

	

	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}

void CPillar::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	UninitInstance();
	m_Collision.Uninit();

	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
}

void CPillar::Update()
{

}

void CPillar::Draw()
{
	DrawInstance();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(RENDERER::m_pCommonVertexLayout);


	m_pMesh->DrawInstanced(m_MeshCount);

	if (!isEnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CPillar::Imgui()
{
	static bool show_pillar_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_pillar_window = !show_pillar_window;

	if (show_pillar_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Pillar", &lw_is_open, lw_flag);

		ImGui::Checkbox("isEnableCollision", &isEnableCollision);

		int count = m_MeshCount;
		ImGui::InputInt("MeshCount", &count, 1);


		ImGui::End();
	}
}