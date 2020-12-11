/*�uDIRECT3D11.h�v=============================================
�@�E���̃v���W�F�N�g�̓����̂悤�ȃN���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once
#include "struct.h"



class RENDERER
{
	static EFFECT m_Effect;
	static VIEWMATRIX m_ViewMatrix;
	static PROJMATRIX m_ProjMatrix;

	static void SetEffect();

	static void CreateGBufferFormat(GBuffer* pGbuffer, DXGI_FORMAT dxgi_format);
	static void CreateConstantBuffers();

	static IDXGISwapChain* m_pSwapChain;
	
	
	//�萔�o�b�t�@	
	static ID3D11Buffer* m_pWorldBuffer;
	static ID3D11Buffer* m_pViewBuffer;
	static ID3D11Buffer* m_pProjectionBuffer;
	
	static ID3D11Buffer* m_pDirectionalLightBuffer;
	static ID3D11Buffer* m_pPointLightBuffer;
	static ID3D11Buffer* m_pEyeBuffer;
	static ID3D11Buffer* m_pAnimationMatrixBuffer;
	static ID3D11Buffer* m_pEffectBuffer;
	static ID3D11Buffer* m_pCullingBuffer;
	
	//======================================================
	//GBuffer
	static GBuffer m_Diffuse_GBuffer;
	static GBuffer m_Normal_GBuffer;
	static GBuffer m_Position_GBuffer;
	static GBuffer m_Lighting_GBuffer;
	static GBuffer m_Velocity_GBuffer;
	static GBuffer m_DepthPBR_GBuffer;

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
	//�f�B���N�V���i�����C�g
	static ID3D11PixelShader* m_pDirectionalPixelShader;

	//�|�C���g���C�e�B���O
	static ID3D11RasterizerState* m_pPointLightingRasterizerState;
	

	//�r���[�|�[�g
	static D3D11_VIEWPORT m_Vp;
	//�f�v�X�X�e���V���X�e�[�g
	static ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;

public:

	//�ʏ�
	static ID3D11RasterizerState* m_pCommonRasterizerState;
	static ID3D11SamplerState*	m_pCommonSamplerState;
	
	static ID3D11VertexShader*	m_pCommonVertexShader;
	static ID3D11PixelShader*	m_pCommonPixelShader;
	static ID3D11InputLayout*	m_pCommonVertexLayout;

	static ID3D11Device*		m_pDevice;			
	static ID3D11DeviceContext*	m_pDeviceContext;	

	
	static bool toggleDirectional;
	static bool togglePoint;
	static bool toggleColor;
	


	//�萔�o�b�t�@�Z�b�g
	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(WORLDMATRIX WorldMatrix);
	static void SetViewMatrix(VIEWMATRIX ViewMatrix);
	static void SetProjectionMatrix(PROJMATRIX ProjectionMatrix);
	static void SetAnimationMatrix(ANIMATIONMATRIX Animation);

	static void SetDirectionalLight(DIRECTIONALLIGHT light);
	static void SetPointLight(POINTLIGHT light);
	static void SetEye(EYE Eye);
	static void SetCulling(CULLING count);
	static void SetFog(FOG fog);
	static void SetDeferred(D3DXVECTOR4 param);

	static void SetRasterizerState(D3D11_CULL_MODE mode);

	static VIEWMATRIX GetViewMatrix() { return m_ViewMatrix; }
	static PROJMATRIX GeProjMatrix() { return m_ProjMatrix; }

	//�����o�֐�
	static HRESULT Init(HWND hWnd);//�@������
	static void Uninit();
	static void ShowFPS();			//�@FPS�\��
	static void Clear();			//�@�N���A


	static void CreateVertexShader(ID3D11VertexShader**,ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC* ,UINT , const char* );
	static void CreatePixelShader(ID3D11PixelShader**,const char* FileName);
	static void CreateComputeShader(const char*, ID3D11ComputeShader**);
	static void CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut);

	static void Deferred();//�f�B�t�@�[�h
	static void DirectionlLighting();//���C�e�B���O
	static void PointLighting();	//�|�C���g���C�g
	static void EffectDraw();	//�|�X�g�G�t�F�N�g
	static void Present();		//�@��ʍX�V
};
