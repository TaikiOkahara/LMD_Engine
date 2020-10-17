#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "StaticMesh.h"
#include "pointLight.h"
#include "player.h"

void CPointLight::Init()
{
	{


		/*for (int i = 0; i < 3; i++)
		{
			D3DXVECTOR4 pos;
			pos = D3DXVECTOR4(0, 3, i * 3,1);
			m_PointList.push_back(&pos);

		}*/
		D3DXVECTOR4 pos = D3DXVECTOR4(-2.5, 0.0f, 5.0f, 3);


		//m_PointList.push_back(pos);//wは光の強さ
		m_PointLight.LightPosition[0] = pos;
	}
	m_pMesh = new StaticMesh();
	m_pMesh->LoadModel("../02 Visual File//UV.fbx");
	//m_pMesh->LoadTexture("");

	m_Position = D3DXVECTOR3(-2.5f, 0.0f, 5.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "PointLightVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "PointLightPixelShader.cso");


}

void CPointLight::Uninit()
{
	m_pMesh->Unload();
	delete m_pMesh;

	
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pPointLightBuffer);
	SAFE_RELEASE(m_pPointLightBufferSRV);
}

void CPointLight::Update()
{

	D3DXVECTOR3 pos = Base::GetScene()->GetGameObject<CPlayer>(1)->GetPosition();

	m_PointLight.LightPosition[0] = D3DXVECTOR4(pos.x, 0.0f, pos.z, 1);
	m_Position = D3DXVECTOR3(pos.x, 0.0f, pos.z);
	
}

void CPointLight::Draw()
{
	RENDERER::SetPointLight(m_PointLight);
	
	SetWorldMatrix();

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	
	m_pMesh->Draw();

}

void CPointLight::Imgui()
{
	static bool show_light_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_light_window = !show_light_window;

	if (show_light_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Light", &lw_is_open, lw_flag);

		for (int i = 0; i < 1; i++)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(m_PointLight.LightPosition[i].x, m_PointLight.LightPosition[i].y, m_PointLight.LightPosition[i].z);
			ImGui::InputFloat3("Position", pos, 1);
		}


		ImGui::End();
	}
}