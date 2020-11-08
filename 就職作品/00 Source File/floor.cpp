#include "director.h"
#include "renderer.h"
#include "floor.h"


//
//
//
void CFloor::Init()
{
	//Field
	m_pField = new Tile();


	m_pField->Init("BrickFloor//T_BrickFloor_Clean_A.png", "BrickFloor//T_BrickFloor_Clean_N.BMP",100,100,1.0f);
	
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0, -D3DX_PI/2, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };


	//シェーダー作成
	RENDERER::CreateVertexShader(&m_VertexShader, &m_VertexLayout, layout, 5, "Tile_VertexShader.cso");
	RENDERER::CreatePixelShader(&m_PixelShader, "Tile_PixelShader.cso");

}
//
//
//
void CFloor::Uninit()
{
	m_pField->Uninit();
	delete m_pField;
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
	////　マトリクス設定
	SetWorldMatrix();

	RENDERER::m_pDeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	RENDERER::m_pDeviceContext->PSSetShader(m_PixelShader, NULL, 0);
	RENDERER::m_pDeviceContext->IASetInputLayout(m_VertexLayout);

	m_pField->Draw();
}