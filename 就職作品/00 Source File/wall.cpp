#include "director.h"
#include "renderer.h"
#include "wall.h"


//
//
//
void CWall::Init()
{
	
	m_pWall = new Tile();


	m_pWall->Init("Wall//T_StoneWall_A.BMP", "Wall//T_StoneWall_N.BMP", 1,2, 4.0f);

	m_Position = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(-D3DX_PI/2, D3DX_PI/2, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

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

	

	
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	std::vector<D3DXVECTOR3> posList;
	std::vector<D3DXVECTOR3> rotList;

	//スタート地点壁
	{
		{
			pos = m_Position + D3DXVECTOR3(4 - 2.0f, 0.0f, -2.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-7.0f, 0.0f, -2.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);
		}
		{
			pos = m_Position + D3DXVECTOR3(4, 0.0f, -4.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-9.0f, 0.0f, -4.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(4, 0.0f, -8.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-9.0f, 0.0f, -8.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI, 0);

			posList.push_back(pos);
			rotList.push_back(rot);
		}
		{
			pos = m_Position + D3DXVECTOR3(0, 0.0f, -10.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(4, 0.0f, -10.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-4.0f, 0.0f, -10.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-8.0f, 0.0f, -10.0f);
			rot = m_Rotation + D3DXVECTOR3(0, D3DX_PI / 2, 0);

			posList.push_back(pos);
			rotList.push_back(rot);
		}
		{
			pos = m_Position + D3DXVECTOR3(2.0f, 4.0f, -8.0f);
			rot = m_Rotation + D3DXVECTOR3(D3DX_PI/2, 0, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(2.0f, 4.0f, -4.0f);
			rot = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-6.0f, 4.0f, -8.0f);
			rot = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-6.0f, 4.0f, -4.0f);
			rot = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			posList.push_back(pos);
			rotList.push_back(rot);

			pos = m_Position + D3DXVECTOR3(-2.0f, 4.0f, 0.0f);
			rot = m_Rotation + D3DXVECTOR3(D3DX_PI / 2, 0, 0);

			posList.push_back(pos);
			rotList.push_back(rot);
			
		}
	}
	

	for (int i = 0; i < 7; i++)
	{
		pos = m_Position + D3DXVECTOR3(0, 0, i * 4.0f);
		rot = m_Rotation;
		posList.push_back(pos);
		rotList.push_back(rot);
	}
	for (int i = 0; i < 7; i++)
	{
		pos = m_Position + D3DXVECTOR3(-5.0f, 0, i * 4.0f);
		rot = m_Rotation + D3DXVECTOR3(0,-D3DX_PI,0);

		posList.push_back(pos);
		rotList.push_back(rot);
	}

	for (int i = 0; i < 8; i++)
	{
		pos = m_Position + D3DXVECTOR3(i * -4.0f + 4 * 4.0f, 0.0f, 35.0f);
		rot = m_Rotation + D3DXVECTOR3(0,D3DX_PI/2, 0);

		posList.push_back(pos);
		rotList.push_back(rot);
	}

	for (int i = 0; i < posList.size(); i++)
	{

		//　マトリクス設定
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, rotList[i].y, rotList[i].x, rotList[i].z);
		D3DXMatrixTranslation(&trans, posList[i].x, posList[i].y, posList[i].z);
		world = scale * rot * trans;
		D3DXMatrixTranspose(&world, &world);

		m_MatrixList.push_back(world);
	}

	posList.clear();
	rotList.clear();

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
}