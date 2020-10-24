#include "director.h"
#include "renderer.h"
#include "wall.h"
#include "Imgui11.h"
#include "input.h"

//
//
//
void CWall::Init()
{
	
	m_pWall = new Tile();
	m_pWall->Init("Wall//T_StoneWall_A.BMP", "Wall//T_StoneWall_N.BMP", 1,2, 4.0f);



	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(-D3DX_PI/2, D3DX_PI/2, 0.0f);
	//m_Rotation = D3DXVECTOR3(0, 0, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_Collision.Init(D3DXVECTOR3(4, 0.1f, 8), D3DXVECTOR3(0, 0, 0));
	//m_collision.Init(D3DXVECTOR3(4, 4, 4), D3DXVECTOR3(0, 0, 0));

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, 5, "InstanceVertexShader.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "Tile_PixelShader.cso");
	
	

	
	//D3DXVECTOR3 pos;
	//D3DXVECTOR3 rot;
	
			
	VECTOR vector;

	vector.scale = m_Scale;
	//スタート地点壁
	{
		{
			vector.position = D3DXVECTOR3(4 - 2.0f, 2.0f, -2.0f);
			//vector.position = m_Position;
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);
			//vector.rotation = m_Rotation + D3DXVECTOR3(0, 0, 0);


			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-7.0f, 2.0f, -2.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			m_Vector.push_back(vector);
		}
		
		{
			vector.position = D3DXVECTOR3(4, 2.0f, -4.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-9.0f, 2.0f, -4.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(4, 2.0f, -8.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-9.0f, 2.0f, -8.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			m_Vector.push_back(vector);
		}
		{
			vector.position = D3DXVECTOR3(0, 2.0f, -10.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(4, 2.0f, -10.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-4.0f, 2.0f, -10.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-8.0f, 2.0f, -10.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			m_Vector.push_back(vector);
		}
		{
			vector.position = D3DXVECTOR3(2.0f, 6.0f, -8.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(D3DX_PI/2, 0, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(2.0f, 6.0f, -4.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-6.0f, 6.0f, -8.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-6.0f, 6.0f, -4.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			m_Vector.push_back(vector);

			vector.position = D3DXVECTOR3(-2.0f, 6.0f, 0.0f);
			vector.rotation = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			m_Vector.push_back(vector);
			
		}
		
	}
	
	
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(0, 2.0f, i * 4.0f);
		vector.rotation = m_Rotation;

		m_Vector.push_back(vector);
	}
	for (int i = 0; i < 7; i++)
	{
		vector.position = D3DXVECTOR3(-5.0f, 2.0f, i * 4.0f);
		vector.rotation = m_Rotation + D3DXVECTOR3(0,-D3DX_PI,0);

		m_Vector.push_back(vector);
	}

	for (int i = 0; i < 8; i++)
	{
		vector.position = D3DXVECTOR3(i * -4.0f + 4 * 4.0f, 2.0f, 35.0f);
		vector.rotation = m_Rotation + D3DXVECTOR3(0,D3DX_PI/2, 0);

		m_Vector.push_back(vector);
	}
	



	//m_wallCount = m_MeshCount;


	InitInstance();
	UpdateInstance();//処理速度が落ちるかもだからInitに置いてる
}
//
//
//
void CWall::Uninit()
{
	m_pWall->Uninit();
	delete m_pWall;

	UninitInstance();
	m_Collision.Uninit();
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
}
//
//
//
void CWall::Update()
{

}
//
//
//
void CWall::Draw()
{
	DrawInstance();
	

	RENDERER::m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_pVertexLayout);

	m_pWall->DrawInstanced(m_MeshCount);

	if(!isEnableCollision)
		m_Collision.DrawInstance(m_MeshCount);
}

void CWall::Imgui()
{
	static bool show_wall_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (Keyboard_IsTrigger(DIK_F1))
		show_wall_window = !show_wall_window;

	if (show_wall_window)
	{
		ImGuiWindowFlags lw_flag = 0;
		static bool lw_is_open;

		ImGui::Begin("Wall", &lw_is_open, lw_flag);

		ImGui::Checkbox("isEnableCollision", &isEnableCollision);

		int count = m_MeshCount;
		ImGui::InputInt("MeshCount",&count , 1);
		/*for (int i = 0; i < m_MatrixList.size(); i++)
		{

		}
		ImGui::SliderFloat3()*/

		ImGui::End();
	}
}