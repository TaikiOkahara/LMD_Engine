/*---------------------------------------
*　projectionShadow.cpp
*
* 投影テクスチャをもとにプレイヤーの足元に影を描画するクラス
*@author：Okahara Taiki
----------------------------------------*/
#include "base.h"
#include "director.h"
#include "renderer.h"
#include "input.h"
#include "projectionShadow.h"
#include "Imgui11.h"
#include "pointLight.h"
#include "player.h"

void CProjShadow::Init()
{

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEINDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	//シェーダー作成
	RENDERER::CreateVertexShader(&m_pVertexShader, &m_pVertexLayout, layout, ARRAYSIZE(layout), "projectionTextureMappingShadowVS.cso");
	RENDERER::CreatePixelShader(&m_pPixelShader, "projectionTextureMappingShadowPS.cso");


	//テクスチャ生成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		D3D11_RENDER_TARGET_VIEW_DESC RTVDesc;
		D3D11_TEXTURE2D_DESC T2DDesc;
		ZeroMemory(&T2DDesc, sizeof(T2DDesc));
		ZeroMemory(&SRVDesc, sizeof(SRVDesc));
		ZeroMemory(&RTVDesc, sizeof(RTVDesc));

		T2DDesc.Width = WINDOW_WIDTH;
		T2DDesc.Height = WINDOW_HEIGHT;
		T2DDesc.MipLevels = 1;
		T2DDesc.ArraySize = 1;
		T2DDesc.CPUAccessFlags = 0;
		T2DDesc.MiscFlags = 0;

		T2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		T2DDesc.SampleDesc.Count = 1;
		T2DDesc.SampleDesc.Quality = 0;
		T2DDesc.Usage = D3D11_USAGE_DEFAULT;
		T2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		RENDERER::GetDevice()->CreateTexture2D(&T2DDesc, NULL, &(m_pShadowTexture));

		SRVDesc.Format = T2DDesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		RENDERER::GetDevice()->CreateShaderResourceView(m_pShadowTexture, &SRVDesc, &(m_pShadowShaderResourceView));
	}
}

void CProjShadow::Uninit()
{
	m_pVertexLayout->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();

	m_pShadowShaderResourceView->Release();
	m_pShadowTexture->Release();
}

void CProjShadow::Update()
{
	}

void CProjShadow::Draw()
{

	//レンダーターゲット
	RENDERER::ShadowBegin();

	//ライトからプレイヤー方向に向けたワールド変換行列を作成
	CPlayer* player = Base::GetScene()->GetGameObject<CPlayer>();
	CPointLight* pointLight = Base::GetScene()->GetGameObject<CPointLight>();


	RENDERER::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	RENDERER::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	RENDERER::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);


	for (int i = 0; i < 3; i++)
	{
		
		D3DXMATRIX Lightproj, Lightview, LightVP;
		D3DXVECTOR3 pos, target;
		D3DXVECTOR3 lPos = pointLight->GetPosition(i);

		D3DXVECTOR4 playerPos = D3DXVECTOR4(player->GetPosition().x, player->GetPosition().y, player->GetPosition().z, 0);
		RENDERER::GetConstantList().GetStruct<PointLightBuffer>()->SetPlayerPos(playerPos);


		pos = lPos + D3DXVECTOR3(0, 2, 0);
		target = player->GetPosition();

		D3DXMatrixLookAtLH(&Lightview, &pos, &target, &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixPerspectiveFovLH(&Lightproj, D3DX_PI / 4, 1, 0.1f, 100.0f);
		D3DXMatrixMultiply(&LightVP, &Lightview, &Lightproj);



		RENDERER::GetConstantList().GetStruct<PointLightBuffer>()->SetLightVP(LightVP, i);
		RENDERER::GetConstantList().GetStruct<PointLightBuffer>()->SetIndex(i);
		

		//プレイヤー影描画
		player->DrawShadow();
	}

	RENDERER::CommonDraw();	
}

void CProjShadow::Imgui()
{
}
