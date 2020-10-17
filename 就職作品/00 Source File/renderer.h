/*�uDIRECT3D11.h�v=============================================
�@�E���̃v���W�F�N�g�̓����̂悤�ȃN���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once


#define		LIGHT_NUM		(10)	//���C�g�̐���L���ɂ���



struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;	
	D3DXVECTOR2 TexturePos;
	
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
};

struct DEFERRED_VERTEX
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 TexturePos;
};

struct ANIMVERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexturePos;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
	
	UINT		BoneIndex[4]{ 0,0,0,0 };
	D3DXVECTOR4 BoneWeight{ 0.0f,0.0f,0.0f,0.0f };
};

struct ANIMATIONMATRIX
{
	D3DXMATRIX Animation[256];
};


struct MATERIAL
{
	D3DXVECTOR4		Ambient;
	D3DXVECTOR4		Diffuse;
	D3DXVECTOR4		Specular;
};

struct DIRECTIONALLIGHT
{
	D3DXVECTOR4 LightDir;
	//D3DXVECTOR4 Intensity;

	//D3DXMATRIX LightView;//�V���h�E�}�b�v�쐬�p
};

struct P_LIGHT
{
	D3DXVECTOR4 g_vLightPos[LIGHT_NUM];
	D3DXVECTOR4 g_fIntensity[LIGHT_NUM];
	D3DXVECTOR4 g_fRange[LIGHT_NUM];
	D3DXVECTOR4 g_fAttRate[LIGHT_NUM];
	D3DXVECTOR4 g_fNumLight = D3DXVECTOR4(LIGHT_NUM, 0, 0, 1);

};

struct POINTLIGHT
{
	D3DXVECTOR4 LightPosition[LIGHT_NUM];//w�͌��̋���
};

struct EYE
{
	D3DXVECTOR4 Eye;
};

//�@������
struct D3D_INIT
{
	HWND hWnd;
};
//
//
//�@�N���X
class RENDERER
{
	static D3DXMATRIX m_World;
	static D3DXMATRIX m_View;
	static D3DXMATRIX m_Proj;
	static D3DXMATRIX GetWorldMatrix() { return m_World; }
	static D3DXMATRIX GetViewMatrix() { return m_View; }
	static D3DXMATRIX GetProjectionMatrix() { return m_Proj; }
	static void CreateRenderFormat(DXGI_FORMAT dxgi_format, ID3D11Texture2D** pTexture, ID3D11RenderTargetView** pRTV, ID3D11ShaderResourceView** pSRV);
	static void CreateConstantBuffers();

	static IDXGISwapChain* m_pSwapChain;
	
	
	//�萔�o�b�t�@	
	static ID3D11Buffer* m_pWorldBuffer;
	static ID3D11Buffer* m_pViewBuffer;
	static ID3D11Buffer* m_pProjectionBuffer;
	static ID3D11Buffer* m_pMaterialBuffer;
	static ID3D11Buffer* m_pDLightBuffer;
	static ID3D11Buffer* m_pPLightBuffer;
	static ID3D11Buffer* m_pEyeBuffer;
	static ID3D11Buffer* m_pAnimationMatrixBuffer;

	//�\���̃o�b�t�@
	//�|�C���g���C�g
	static ID3D11Buffer* m_pPointLightBuffer;
	static ID3D11ShaderResourceView* m_pPointLightBufferSRV;

	//static ID3D11Buffer* m_ComputeBufferIn;
	//static ID3D11Buffer* m_ComputeBufferOut;
	//static ID3D11ComputeShader* m_pComputeShader;
	//static ID3D11ShaderResourceView* m_pCompute_SRV;
	//static ID3D11ShaderResourceView* m_pComput_SRVOut;
	//static ID3D11UnorderedAccessView* m_pCompute_UAV;
	//======================================================
	//�����o�[�g
	static ID3D11Texture2D* m_pColorTexture;
	static ID3D11RenderTargetView* m_pColor_RTV;
	static ID3D11ShaderResourceView* m_pColor_SRV;
	//�m�[�}��
	static ID3D11Texture2D* m_pNormalTexture;
	static ID3D11RenderTargetView* m_pNormal_RTV;
	static ID3D11ShaderResourceView* m_pNormal_SRV;
	//�|�W�V����
	static ID3D11Texture2D* m_pPositionTexture;
	static ID3D11RenderTargetView* m_pPosition_RTV;
	static ID3D11ShaderResourceView* m_pPosition_SRV;
	//���C�e�B���O
	static ID3D11Texture2D* m_pLightingTexture;
	static ID3D11RenderTargetView* m_pLighting_RTV;
	static ID3D11ShaderResourceView* m_pLighting_SRV;



	//�f�B�t�@�[�h
	static ID3D11Buffer* m_pScreenPolyVB;
	static ID3D11VertexShader* m_pVS_Deferred;
	static ID3D11PixelShader* m_pPS_Deferred;
	static ID3D11InputLayout* m_pDeferredVertexLayout;
	static ID3D11RenderTargetView* m_pDeferred_TexRTV;	
	static ID3D11DepthStencilView* m_pDeferred_DSTexDSV;
	static ID3D11Texture2D* m_pDepthStencil;
	static ID3D11SamplerState* m_pDeferredSamplerState;
	static ID3D11RasterizerState* m_pDeferredRasterizerState;
	static ID3D11BlendState* m_pDeferredBlendState;
	//======================================================

	//�r���[�|�[�g
	static D3D11_VIEWPORT m_Vp;
	//�f�v�X�X�e���V���X�e�[�g
	static ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;

public:

	//�ʏ�
	static ID3D11RasterizerState* m_pCommonRasterizerState;
	static ID3D11SamplerState*	m_pCommonSamplerState;
	//static ID3D11BlendState*	m_pCommonBlendState;			
	
	static ID3D11VertexShader*	m_pCommonVertexShader;
	static ID3D11PixelShader*	m_pCommonPixelShader;
	static ID3D11InputLayout*	m_pCommonVertexLayout;

	static ID3D11Device*		m_pDevice;			
	static ID3D11DeviceContext*	m_pDeviceContext;	

	

	//static ID3D11DepthStencilState*		m_pBuckBuffer_DSTexState;
	
	
	
	
	

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetAnimationMatrix(ANIMATIONMATRIX Animation);

	static void SetDirectionalLight(D3DXVECTOR4 Light,D3DXMATRIX matrix);
	static void SetPointLight(POINTLIGHT Light);
	static void SetEye(EYE Eye);
	static void SetPointLight(ID3D11Buffer*,ID3D11ShaderResourceView*);
	
	RENDERER() {}	//�@�R���X�g���N�^
	~RENDERER() {}	//�@�f�X�g���N�^

	//�����o�֐�
	static HRESULT Init(D3D_INIT*);//�@������
	static void Uninit();
	static void ShowFPS();			//�@FPS�\��
	static void Clear();			//�@�N���A


	static HRESULT CreateVertexShader(ID3D11VertexShader**,ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC* ,UINT , const char* );
	static HRESULT CreatePixelShader(ID3D11PixelShader**,const char* FileName);
	static HRESULT CreateComputeShader(const char*, ID3D11ComputeShader**);

	static HRESULT CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut);
	static HRESULT Deferred();//�f�B�t�@�[�h
	static HRESULT Lighting();//���C�e�B���O
	static HRESULT Present();		//�@��ʍX�V
};
