/*---------------------------------------
*�@renderer.h
* 
* �`�������Ǘ�����N���X
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "struct.h"




class RENDERER
{
	static ID3D11Device* m_pDevice;
	static ID3D11DeviceContext* m_pDeviceContext;
	static IDXGISwapChain* m_pSwapChain;

	static ID3D11RasterizerState* m_pCommonRasterizerState;
	static ID3D11SamplerState* m_pCommonSamplerState;

	//�r���[�|�[�g
	static D3D11_VIEWPORT m_Vp;
	//�f�v�X�X�e���V���X�e�[�g
	static ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;


	static ConstantBufferList m_ConstantBufferList;


	
	//======================================================
	//GBuffer
	static GBuffer m_Diffuse_GBuffer;
	static GBuffer m_Normal_GBuffer;
	static GBuffer m_Position_GBuffer;
	static GBuffer m_Lighting_GBuffer;
	static GBuffer m_VelocityDepth_GBuffer;
	static GBuffer m_PBR_GBuffer;
	static GBuffer m_Shadow_GBuffer;
	//======================================================


	//�f�B�t�@�[�h
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


	//�|�C���g���C�e�B���O
	static ID3D11RasterizerState* m_pPointLightingRasterizerState;
	


	static void CreateGBufferFormat(GBuffer* pGbuffer, DXGI_FORMAT dxgi_format);
	static void CreateConstantBuffers();
public:
	



	static HRESULT Init(HWND hWnd);
	static void Uninit();
	static void ShowFPS();//�@FPS�\��
	static void Clear();

	static ID3D11Device* GetDevice() { return m_pDevice; }
	static ID3D11DeviceContext* GetDeviceContext() { return m_pDeviceContext; }
	static ConstantBufferList GetConstantList() { return m_ConstantBufferList; }



	static void CreateVertexShader(ID3D11VertexShader**,ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC* ,UINT , const char* );
	static void CreatePixelShader(ID3D11PixelShader**,const char* FileName);
	static void CreateComputeShader(const char*, ID3D11ComputeShader**);
	static void CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut);


	//�QD(UI)�̃}�g���N�X�ϊ��֐�
	static void SetWorldViewProjection2D();
	
	static void SetRasterizerState(D3D11_CULL_MODE cull, D3D11_FILL_MODE fill);
	static void SetBlendState_Lighting();

	static GBuffer GetShadowGBuffer() { return m_Shadow_GBuffer; }
	
	static void Deferred();//�f�B�t�@�[�h�J�n
	static void PointLighting();//�|�C���g���C�g�J�n
	static void CommonDraw();
	static void PostProcessDraw();
	static void ShadowDraw();
	static void ShadowBegin();
	static void Present();//�@��ʍX�V
};