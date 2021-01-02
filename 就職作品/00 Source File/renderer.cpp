/*「DIRECT3D11.cpp」=============================================
　・このプロジェクトの統括のようなクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#include "window.h"
#include "director.h"
#include "renderer.h"
#include <io.h>



ConstantBufferList RENDERER::m_ConstantBufferList;

ID3D11Device* RENDERER::m_pDevice = NULL;
ID3D11DeviceContext* RENDERER::m_pDeviceContext = NULL;
IDXGISwapChain* RENDERER::m_pSwapChain = NULL;
ID3D11RenderTargetView* RENDERER::m_pDeferred_TexRTV = NULL;

ID3D11DepthStencilView* RENDERER::m_pDeferred_DSTexDSV = NULL;
ID3D11ShaderResourceView* RENDERER::m_pDeferred_SRV = NULL;
//ディファードレンダリング用
ID3D11RasterizerState* RENDERER::m_pDeferredRasterizerState = NULL;
ID3D11SamplerState* RENDERER::m_pDeferredSamplerState = NULL;

//ポイントライト
ID3D11RasterizerState* RENDERER::m_pPointLightingRasterizerState = NULL;
ID3D11BlendState* RENDERER::m_pPointLightBlendState = NULL;

//通常
ID3D11SamplerState* RENDERER::m_pCommonSamplerState = NULL;
ID3D11RasterizerState* RENDERER::m_pCommonRasterizerState = NULL;


//ビューポート
D3D11_VIEWPORT RENDERER::m_Vp;
//デプスステンシルステート
ID3D11DepthStencilState* RENDERER::m_pBuckBuffer_DSTexState = NULL;


//GBuffer --------------------------------------------
GBuffer RENDERER::m_Diffuse_GBuffer;
GBuffer RENDERER::m_Normal_GBuffer;
GBuffer RENDERER::m_Position_GBuffer;
GBuffer RENDERER::m_Lighting_GBuffer;
GBuffer RENDERER::m_Velocity_GBuffer;
GBuffer RENDERER::m_DepthPBR_GBuffer;
GBuffer RENDERER::m_Shadow_GBuffer;
//===============================================
ID3D11InputLayout* RENDERER::m_pDeferredVertexLayout = NULL;

ID3D11Texture2D* RENDERER::m_pDepthStencil = NULL;
ID3D11Buffer* RENDERER::m_pScreenPolyVB = NULL;
ID3D11VertexShader* RENDERER::m_pDeferredVertexShader = NULL;
ID3D11PixelShader* RENDERER::m_pDeferredPixelShader = NULL;
//ディレクショナルライト
//ID3D11PixelShader* RENDERER::m_pDirectionalPixelShader = NULL;



//
//
//　初期化
HRESULT RENDERER::Init(HWND phWnd)
{


	DXGI_SWAP_CHAIN_DESC sd;//　スワップチェイン
	ZeroMemory( &sd, sizeof(sd) );//　ゼロクリア
	//　スワップチェイン設定
	sd.BufferCount = 1;
	sd.BufferDesc.Width=WINDOW_WIDTH;
	sd.BufferDesc.Height=WINDOW_HEIGHT;
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator=60;
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow= phWnd;
	sd.SampleDesc.Count=1;
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;
	

	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
	
	
	if( FAILED( D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		&FeatureLevels,
		&m_pDeviceContext
	)) )
	{
		return FALSE;
	}



	//バックバッファーテクスチャーを取得
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pDeferred_TexRTV );
	SAFE_RELEASE(pBackBuffer);	

	//通常の深度ステンシルビュー用のテクスチャーを作成
	
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;

	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil);
	
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvd.Flags = 0;
	dsvd.Texture2D.MipSlice = 0;
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvd, &m_pDeferred_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにセット
	m_pDeviceContext->OMSetRenderTargets(1, &m_pDeferred_TexRTV,m_pDeferred_DSTexDSV);

	//シェーダーリソースビュー
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = descDepth.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	m_pDevice->CreateShaderResourceView(m_pDepthStencil, &SRVDesc, &m_pDeferred_SRV);
	//----------------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------------------
	//震度ステンシルステートを作成
	
	D3D11_DEPTH_STENCIL_DESC dc;
	ZeroMemory(&dc,sizeof(dc));
	dc.DepthEnable=true;
	dc.DepthWriteMask= D3D11_DEPTH_WRITE_MASK_ZERO;
	//dc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	dc.DepthFunc= D3D11_COMPARISON_LESS_EQUAL;
	//dc.DepthFunc=D3D11_COMPARISON_LESS;
	//dc.StencilEnable=false;
	if(FAILED(m_pDevice->CreateDepthStencilState(&dc,&m_pBuckBuffer_DSTexState)))
	{
		return E_FAIL;
	}
	//深度ステンシルステートを適用
	//m_pDeviceContext->OMSetDepthStencilState(m_pBuckBuffer_DSTexState,NULL);
	

	//ビューポートの設定
	
	m_Vp.Width = WINDOW_WIDTH;
	m_Vp.Height = WINDOW_HEIGHT;
	m_Vp.MinDepth = 0.0f;
	m_Vp.MaxDepth = 1.0f;
	m_Vp.TopLeftX = 0;
	m_Vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &m_Vp);

	//ディファードレンダリング用ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode= D3D11_CULL_MODE::D3D11_CULL_BACK;
	rdc.FillMode= D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//rdc.FrontCounterClockwise = FALSE;//デフォルト
	rdc.DepthClipEnable = FALSE;
	//rdc.MultisampleEnable = FALSE;
	m_pDevice->CreateRasterizerState(&rdc,&m_pDeferredRasterizerState);


	//通常用ラスタライズ設定
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//rdc.FrontCounterClockwise = FALSE;//デフォルト
	rdc.DepthClipEnable = FALSE;

	//rdc.MultisampleEnable = FALSE;

	m_pDevice->CreateRasterizerState(&rdc, &m_pCommonRasterizerState);

	//ポイントライト用バックカリングラスタライズ設定
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	//rdc.FrontCounterClockwise = FALSE;
	//rdc.DepthClipEnable = FALSE;
	//rdc.MultisampleEnable = FALSE;

	m_pDevice->CreateRasterizerState(&rdc, &m_pPointLightingRasterizerState);



	//ライティング用ブレンドステート作成
	D3D11_BLEND_DESC dblend;
	ZeroMemory(&dblend, sizeof(D3D11_BLEND_DESC));
	dblend.IndependentBlendEnable = false;
	dblend.AlphaToCoverageEnable = false;
	dblend.RenderTarget[0].BlendEnable = true;
	dblend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//メッシュのレンダリングイメージ
	dblend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;//レンダーターゲットサーファスのイメージ
	dblend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	dblend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	dblend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	dblend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	dblend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&dblend, &m_pPointLightBlendState)))
	{
		return E_FAIL;
	}
	

	//アルファブレンド種類
	/*
	*アルファブレンド無効
	*SrcBlend = D3D11_BLEND_ONE;//1
	*DestBlend = D3D11_BLEND_ZERO;//0
	*BlendOp = D3D11_BLEND_OP_ADD;//SrcBlend * 1 + DestBlend * 0
	*線形合成
	*SrcBlend = SrcBlendAlpha;//1
	*DestBlend = (1 - SrcBlendAlpha);//0
	*BlendOp = D3D11_BLEND_OP_ADD;
	*
	*etc...
	*参考文献 
	*http://maverickproj.web.fc2.com/d3d11_06.html 
	*/





	// ディファードレンダリング用サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pDevice->CreateSamplerState(&samplerDesc, &m_pDeferredSamplerState);
	m_pDeviceContext->PSSetSamplers(1, 1, &m_pDeferredSamplerState);

	//通常サンプラーステート
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	m_pDevice->CreateSamplerState(&samplerDesc, &m_pCommonSamplerState);
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pCommonSamplerState);


	//==================================================================
	//GBuffer作成

	CreateGBufferFormat(&m_Diffuse_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	CreateGBufferFormat(&m_Normal_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	CreateGBufferFormat(&m_Position_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	CreateGBufferFormat(&m_Lighting_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	CreateGBufferFormat(&m_Velocity_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R16G16_FLOAT);
	CreateGBufferFormat(&m_DepthPBR_GBuffer,DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	CreateGBufferFormat(&m_Shadow_GBuffer, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);


	//スクリーンサイズのポリゴン用　バーテックスバッファーを作成
	DEFERRED_VERTEX VertexData[] ={
		D3DXVECTOR3(-1,-1,0),D3DXVECTOR2(0,1),
		D3DXVECTOR3(-1, 1,0),D3DXVECTOR2(0,0),
		D3DXVECTOR3( 1,-1,0),D3DXVECTOR2(1,1),
		D3DXVECTOR3( 1, 1,0),D3DXVECTOR2(1,0)
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DEFERRED_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = VertexData;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pScreenPolyVB)))
		return FALSE;


	//==================================================================



	//ディファード---------------------------------------------------------------------------------------

	//　入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[]{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };
	
	CreateVertexShader(&m_pDeferredVertexShader, &m_pDeferredVertexLayout,layout, ARRAYSIZE(layout), "deferredVS.cso");
	CreatePixelShader(&m_pDeferredPixelShader, "deferredPS.cso");

	

	//ディレクショナルライト-------------------------------------------------------------------------------
	//CreatePixelShader(&m_pDirectionalPixelShader, "directionalLightPS.cso");



	//定数バッファ作成
	CreateConstantBuffers();


	return S_OK;
}
//
//
//
void RENDERER::Uninit()
{
	

	SAFE_RELEASE(m_pDeferredPixelShader);
	SAFE_RELEASE(m_pDeferredVertexShader);
	SAFE_RELEASE(m_pScreenPolyVB);
	SAFE_RELEASE(m_pDepthStencil);

	SAFE_RELEASE(m_pBuckBuffer_DSTexState);

	//GBuffer
	
	m_Diffuse_GBuffer.Release();
	m_Normal_GBuffer.Release();
	m_Position_GBuffer.Release();
	m_Lighting_GBuffer.Release();
	m_Velocity_GBuffer.Release();
	m_DepthPBR_GBuffer.Release();
	m_Shadow_GBuffer.Release();

	SAFE_RELEASE(m_pDeferred_DSTexDSV);
	SAFE_RELEASE(m_pDeferred_SRV);
	SAFE_RELEASE(m_pDeferred_TexRTV);
	SAFE_RELEASE(m_pDeferredVertexLayout);
	SAFE_RELEASE(m_pDeferredSamplerState);
	SAFE_RELEASE(m_pDeferredRasterizerState);

	//DirectionaｌLight
	//SAFE_RELEASE(m_pDirectionalPixelShader);
	//PointLight
	SAFE_RELEASE(m_pPointLightingRasterizerState);
	SAFE_RELEASE(m_pPointLightBlendState);

	
	SAFE_RELEASE(m_pCommonSamplerState);
	SAFE_RELEASE(m_pCommonRasterizerState);
	

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//
//
void RENDERER::Clear()
{
	
	m_pDeviceContext->RSSetViewports(1, &m_Vp);
	m_pDeviceContext->RSSetState(m_pCommonRasterizerState);
	//m_pDeviceContext->PSSetSamplers(0, 1, &m_pCommonSamplerState);


	//全てのテクスチャーをレンダーターゲットにセット
	ID3D11RenderTargetView* pViews[6];


	pViews[0] = m_Diffuse_GBuffer.m_pRenderTargetView;
	pViews[1] = m_Normal_GBuffer.m_pRenderTargetView;
	pViews[2] = m_Position_GBuffer.m_pRenderTargetView;
	pViews[3] = m_Lighting_GBuffer.m_pRenderTargetView;
	pViews[4] = m_Velocity_GBuffer.m_pRenderTargetView;
	pViews[5] = m_DepthPBR_GBuffer.m_pRenderTargetView;
	m_pDeviceContext->OMSetRenderTargets(6, pViews, m_pDeferred_DSTexDSV);//深度ステンシルビューは全てに共通の1つだけを使う
	
	


	//クリア
	float ClearColor[4] = { 0,0,0,1 };
	m_pDeviceContext->ClearRenderTargetView(m_pDeferred_TexRTV, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Diffuse_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Normal_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Position_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Lighting_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Velocity_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_DepthPBR_GBuffer.m_pRenderTargetView, ClearColor);
	m_pDeviceContext->ClearRenderTargetView(m_Shadow_GBuffer.m_pRenderTargetView, ClearColor);
	

	m_pDeviceContext->ClearDepthStencilView(m_pDeferred_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);


}
//
//
//ディファード
void RENDERER::Deferred()
{

	//レンダーターゲットを通常に戻す
	m_pDeviceContext->OMSetRenderTargets(1, &m_pDeferred_TexRTV, m_pDeferred_DSTexDSV);	
	m_pDeviceContext->OMSetDepthStencilState(m_pBuckBuffer_DSTexState, NULL);
	
	//ブレンドステート
	UINT mask = 0xffffffff;
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->OMSetBlendState(NULL, blend, mask);
	
	m_pDeviceContext->RSSetState(m_pDeferredRasterizerState);


	//使用するシェーダーは、テクスチャーを参照するシェーダー	
	m_pDeviceContext->VSSetShader(m_pDeferredVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pDeferredPixelShader, NULL, 0);
	m_pDeviceContext->IASetInputLayout(m_pDeferredVertexLayout);
	

	//1パス目で作成したテクスチャーをセット
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_Diffuse_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(1, 1, &m_Normal_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(2, 1, &m_Position_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(3, 1, &m_Lighting_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(4, 1, &m_Velocity_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(5, 1, &m_DepthPBR_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(6, 1, &m_Shadow_GBuffer.m_pShaderResourceView);
	

	//スクリーンサイズのポリゴンをレンダー
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT stride = sizeof(DEFERRED_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pScreenPolyVB, &stride, &offset);

	m_pDeviceContext->Draw(4, 0);
}


//ライティング
void RENDERER::PointLighting()
{
	
	//レンダーターゲット
	m_pDeviceContext->OMSetRenderTargets(1, &m_Lighting_GBuffer.m_pRenderTargetView, m_pDeferred_DSTexDSV);


	
	ID3D11DepthStencilState* stencil = NULL;
	D3D11_DEPTH_STENCIL_DESC dc;
	ZeroMemory(&dc, sizeof(dc));
	dc.DepthEnable = false;
	dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	
	dc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	m_pDevice->CreateDepthStencilState(&dc, &stencil);
	//深度ステンシルステートを適用
	m_pDeviceContext->OMSetDepthStencilState(stencil,NULL);
	


	//バックカリング
	m_pDeviceContext->RSSetState(m_pPointLightingRasterizerState);
	//ブレンドステート
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->OMSetBlendState(m_pPointLightBlendState, blend, 0xffffffff);

	m_pDeviceContext->PSSetShaderResources(0, 1, &m_Diffuse_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(1, 1, &m_Normal_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(2, 1, &m_Position_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(3, 1, &m_DepthPBR_GBuffer.m_pShaderResourceView);


}

void RENDERER::CommonDraw()
{
	//全てのテクスチャーをレンダーターゲットにセット
	ID3D11RenderTargetView* pViews[7];


	pViews[0] = m_Diffuse_GBuffer.m_pRenderTargetView;
	pViews[1] = m_Normal_GBuffer.m_pRenderTargetView;
	pViews[2] = m_Position_GBuffer.m_pRenderTargetView;
	pViews[3] = m_Lighting_GBuffer.m_pRenderTargetView;
	pViews[4] = m_Velocity_GBuffer.m_pRenderTargetView;
	pViews[5] = m_DepthPBR_GBuffer.m_pRenderTargetView;
	pViews[6] = m_Shadow_GBuffer.m_pRenderTargetView;
	m_pDeviceContext->OMSetRenderTargets(7, pViews, m_pDeferred_DSTexDSV);//深度ステンシルビューは全てに共通の1つだけを使う

	m_pDeviceContext->OMSetDepthStencilState(NULL, 0);

	m_pDeviceContext->RSSetState(m_pCommonRasterizerState);

	//ブレンドステート
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->OMSetBlendState(nullptr, blend, 0xffffffff);
}



void RENDERER::PostProcessDraw()
{
	//仮-------------------------------------------
	//SetEffect();
	m_ConstantBufferList.GetStruct<EffectBuffer>()->Set();
	m_pDeviceContext->OMSetRenderTargets(1, &m_pDeferred_TexRTV, m_pDeferred_DSTexDSV);
	m_pDeviceContext->OMSetDepthStencilState(m_pBuckBuffer_DSTexState, NULL);
	m_pDeviceContext->IASetInputLayout(m_pDeferredVertexLayout);
	m_pDeviceContext->RSSetState(m_pDeferredRasterizerState);
	//---------------------------------------------


	//SetEffect();
	m_ConstantBufferList.GetStruct<EffectBuffer>()->Set();

	
	//1パス目で作成したテクスチャー3枚をセット
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_Diffuse_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(1, 1, &m_Normal_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(2, 1, &m_Position_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(3, 1, &m_Lighting_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(4, 1, &m_Velocity_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(5, 1, &m_DepthPBR_GBuffer.m_pShaderResourceView);
	m_pDeviceContext->PSSetShaderResources(6, 1, &m_Shadow_GBuffer.m_pShaderResourceView);


	//スクリーンサイズのポリゴンをレンダー
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT stride = sizeof(DEFERRED_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pScreenPolyVB, &stride, &offset);

	m_pDeviceContext->Draw(4, 0);


	//標準化------------------------------------------------------

	//ブレンドステート
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->OMSetBlendState(nullptr, blend, 0xffffffff);

}

void RENDERER::ShadowDraw()
{

	//レンダーターゲット
	m_pDeviceContext->OMSetRenderTargets(1, &m_Shadow_GBuffer.m_pRenderTargetView, m_pDeferred_DSTexDSV);

	m_pDeviceContext->PSSetShaderResources(3, 1, &m_Lighting_GBuffer.m_pShaderResourceView);

}


//　画面更新
void RENDERER::Present()
{
	m_pSwapChain->Present(1,0);//画面更新（バックバッファをフロントバッファに）
	
	
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->RSSetState(NULL);
	m_pDeviceContext->OMSetBlendState(NULL, blend, 0xFFFFFFFF);
	m_pDeviceContext->OMSetDepthStencilState(NULL, 0);
}


// FPS固定
void RENDERER::ShowFPS()
{
	//　初期化
	static INT Frames=0,FPS=0;
	static LARGE_INTEGER Frq={0},PreviousTime={0},CurrentTime={0};//　Manual.txt参照してー
	DOUBLE Time=0;
	char sz[11]={0};



	QueryPerformanceFrequency(&Frq);
	
	QueryPerformanceCounter(&CurrentTime);
	Time=(DOUBLE)CurrentTime.QuadPart-PreviousTime.QuadPart;
	Time *=(DOUBLE)1100.0 / (DOUBLE)Frq.QuadPart;
	if(Time>=1100.000)
	{
		PreviousTime=CurrentTime;
		FPS=Frames;
		Frames=0;
	}

	Frames++;
}





void RENDERER::SetWorldViewProjection2D()
{
	ID3D11Buffer* worldBuff = m_ConstantBufferList.GetStruct<WorldBuffer>()->GetBuffer();
	ID3D11Buffer* viewBuff  = m_ConstantBufferList.GetStruct<ViewBuffer>()->GetBuffer();
	ID3D11Buffer* projBuff  = m_ConstantBufferList.GetStruct<ProjBuffer>()->GetBuffer();


	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_pDeviceContext->UpdateSubresource(worldBuff, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_pDeviceContext->UpdateSubresource(viewBuff, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_pDeviceContext->UpdateSubresource(projBuff, 0, NULL, &projection, 0, 0);

}



void RENDERER::SetRasterizerState(D3D11_CULL_MODE mode)
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = mode;
	rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rdc.DepthClipEnable = FALSE;
	m_pDevice->CreateRasterizerState(&rdc, &m_pCommonRasterizerState);
	m_pDeviceContext->RSSetState(m_pCommonRasterizerState);
}

void RENDERER::SetBlendState_Lighting()
{
	//ブレンドステート
	float blend[4] = { 1,1,1,1 };
	m_pDeviceContext->OMSetBlendState(m_pPointLightBlendState, blend, 0xffffffff);
}



void RENDERER::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, D3D11_INPUT_ELEMENT_DESC* pLayout, UINT layoutSize, const char* FileName)
{
	SetShaderDirectory();

	FILE* file;
	long int fsize;
	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_pDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	if (pLayout)
	{
		m_pDevice->CreateInputLayout(pLayout, layoutSize, buffer, fsize, VertexLayout);
	}


	delete[] buffer;
}


void RENDERER::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	SetShaderDirectory();

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_pDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);


	delete[] buffer;
}

void RENDERER::CreateComputeShader(const char* FileName,ID3D11ComputeShader** ppComputeShader)
{
	SetShaderDirectory();
	

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_pDevice->CreateComputeShader(buffer, fsize, NULL, ppComputeShader);


	delete[] buffer;

}

void RENDERER::CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut)
{
	(*ppBufferOut) = nullptr;

	D3D11_BUFFER_DESC desc;
	memset(&desc, 0, sizeof(desc));

	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	if (pInitData)
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInitData;

		m_pDevice->CreateBuffer(&desc, &initData, ppBufferOut);
		return;
	}

	m_pDevice->CreateBuffer(&desc, nullptr, ppBufferOut);
}


void RENDERER::CreateGBufferFormat(GBuffer* pGbuffer, DXGI_FORMAT dxgi_format)
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

	T2DDesc.Format = dxgi_format;
	T2DDesc.SampleDesc.Count = 1;
	T2DDesc.SampleDesc.Quality = 0;
	T2DDesc.Usage = D3D11_USAGE_DEFAULT;
	T2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pDevice->CreateTexture2D(&T2DDesc, NULL, &(pGbuffer->m_pTexture));

	RTVDesc.Format = T2DDesc.Format;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;
	m_pDevice->CreateRenderTargetView(pGbuffer->m_pTexture, &RTVDesc, &(pGbuffer->m_pRenderTargetView));


	SRVDesc.Format = T2DDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	m_pDevice->CreateShaderResourceView(pGbuffer->m_pTexture, &SRVDesc, &(pGbuffer->m_pShaderResourceView));
}


void RENDERER::CreateConstantBuffers()
{
	
	m_ConstantBufferList.AddConstantBuffer<WorldBuffer>(0);

	m_ConstantBufferList.AddConstantBuffer<ViewBuffer>(1);

	m_ConstantBufferList.AddConstantBuffer<ProjBuffer>(2);

	m_ConstantBufferList.AddConstantBuffer<EyeBuffer>(3);

	m_ConstantBufferList.AddConstantBuffer<DirectionalLightBuffer>(4);

	m_ConstantBufferList.AddConstantBuffer<PointLightBuffer>(5);

	m_ConstantBufferList.AddConstantBuffer<EffectBuffer>(6);

	m_ConstantBufferList.AddConstantBuffer<AnimationBuffer>(7);

	m_ConstantBufferList.AddConstantBuffer<ToggleBuffer>(8);

	m_ConstantBufferList.AddConstantBuffer<CullingBuffer>(9);

}