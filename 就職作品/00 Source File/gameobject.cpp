#include "gameobject.h"

ID3D11VertexShader* CGameObject::m_pCommonVertexShader = nullptr;
ID3D11PixelShader* CGameObject::m_pCommonPixelShader = nullptr;
ID3D11InputLayout* CGameObject::m_pCommonVertexLayout = nullptr;

void CGameObject::Load()
{
	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC clayout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pCommonVertexShader, &m_pCommonVertexLayout, clayout, ARRAYSIZE(clayout), "commonVS.cso");
	RENDERER::CreatePixelShader(&m_pCommonPixelShader, "commonPS.cso");
}

void CGameObject::Unload()
{
	m_pCommonVertexShader->Release();
	m_pCommonPixelShader->Release();
	m_pCommonVertexLayout->Release();
}
