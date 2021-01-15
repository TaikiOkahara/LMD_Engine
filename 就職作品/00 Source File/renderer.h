/*「DIRECT3D11.h」=============================================
　・このプロジェクトの統括のようなクラス
　　製作者：岡原大起　	(-"-)
=============================================================*/
#pragma once
#include "struct.h"




class RENDERER
{
	static void CreateGBufferFormat(GBuffer* pGbuffer, DXGI_FORMAT dxgi_format);
	static void CreateConstantBuffers();

	static IDXGISwapChain* m_pSwapChain;
	
	//======================================================
	//GBuffer
	static GBuffer m_Diffuse_GBuffer;
	static GBuffer m_Normal_GBuffer;
	static GBuffer m_Position_GBuffer;
	static GBuffer m_Lighting_GBuffer;
	static GBuffer m_Velocity_GBuffer;
	static GBuffer m_DepthPBR_GBuffer;
	static GBuffer m_Shadow_GBuffer;
	//======================================================




	//ディファード
	static ID3D11Buffer* m_pScreenPolyVB;
	static ID3D11VertexShader* m_pDeferredVertexShader;
	static ID3D11PixelShader* m_pDeferredPixelShader;
	static ID3D11InputLayout* m_pDeferredVertexLayout;
	static ID3D11RenderTargetView* m_pDeferred_TexRTV;	
	static ID3D11DepthStencilView* m_pDeferred_DSTexDSV;
	static ID3D11ShaderResourceView* m_pDeferred_SRV;
	static ID3D11Texture2D* m_pDepthStencil;
	static ID3D11SamplerState* m_pDeferredSamplerState;
	static ID3D11RasterizerState* m_pDeferredRasterizerState;
	static ID3D11BlendState* m_pPointLightBlendState;


	//ポイントライティング
	static ID3D11RasterizerState* m_pPointLightingRasterizerState;
	

	//ビューポート
	static D3D11_VIEWPORT m_Vp;
	//デプスステンシルステート
	static ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;


public:
	static ConstantBufferList m_ConstantBufferList;
	


	//通常
	static ID3D11RasterizerState* m_pCommonRasterizerState;
	static ID3D11SamplerState*	m_pCommonSamplerState;
	


	static ID3D11Device*		m_pDevice;			
	static ID3D11DeviceContext*	m_pDeviceContext;	




	//メンバ関数
	static HRESULT Init(HWND hWnd);//　初期化
	static void Uninit();
	static void ShowFPS();			//　FPS表示
	static void Clear();			//　クリア


	static void CreateVertexShader(ID3D11VertexShader**,ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC* ,UINT , const char* );
	static void CreatePixelShader(ID3D11PixelShader**,const char* FileName);
	static void CreateComputeShader(const char*, ID3D11ComputeShader**);
	static void CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut);



	static void SetWorldViewProjection2D();
	
	static void SetRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill);
	static void SetBlendState_Lighting();

	//static ConstantBufferList GetConstantBufferList() { return m_ConstantBufferList; }
	static void Deferred();//ディファード
	static void PointLighting();	//ポイントライト
	static void CommonDraw();
	static void PostProcessDraw();
	static void ShadowDraw();
	static void Present();		//　画面更新
};