#include "director.h"
#include "renderer.h"
#include "floor.h"
#include "Imgui11.h"
#include "input.h"


//
//
//
void CFloor::Init()
{
	//Field
	m_pField = new Tile();


	m_pField->Init("BrickFloor//T_BrickFloor_Clean_A.dds", "BrickFloor//T_BrickFloor_Clean_N.dds", "BrickFloor//T_BrickFloor_Clean_C.dds",100,100,1.0f);
	
	//m_Transform.rotation = D3DXVECTOR3(0, -D3DX_PI/2, 0.0f);
	
	m_Collision.Init(D3DXVECTOR3(100.0f, 1.0f, 100.0f), D3DXVECTOR3(0, 0, 0));


	m_TransformList.push_back(TRANSFORM{ D3DXVECTOR3(0, 0.0f,0),D3DXVECTOR3(0, -D3DX_PI / 2, 0.0f), D3DXVECTOR3(1,1,1) });

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader, &m_VertexLayout, layout, 5, "commonVS.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "tilePS.cso");


	InitInstance();
	UpdateInstance();
}
//
//
//
void CFloor::Uninit()
{
	m_pField->Uninit();
	delete m_pField;

	UninitInstance();
	m_Collision.Uninit();
}
//
//
//
void CFloor::Update()
{

}
//
//
//
void CFloor::Draw()
{

	DrawInstance();


	////　マトリクス設定
	//SetWorldMatrix();

	//RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->VSSetShader(m_pInstanceVertexShader, NULL, 0);
	//RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pCommonPixelShader, NULL, 0);
	//RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pCommonVertexLayout);

	//m_pField->Draw();
	m_pField->DrawInstanced(m_MeshCount);


	if (m_EnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}


void CFloor::Imgui()
{
	static bool show = true;


	if (CInput::KeyTrigger(DIK_F1))
		show = !show;

	if (show)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Floor", &lw_is_open, lw_flag);

		ImGui::Checkbox("EnableCollision", &m_EnableCollision);

		ImGui::Text("MeshCount : %d / %d", m_MeshCount, m_MeshMax);


		ImGui::End();
	}
}