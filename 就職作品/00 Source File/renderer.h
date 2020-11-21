/*�uDIRECT3D11.h�v=============================================
�@�E���̃v���W�F�N�g�̓����̂悤�ȃN���X
�@�@����ҁF������N�@	(-"-)
=============================================================*/
#pragma once


#define	LIGHT_MAX		(128)	//���C�g�̐���L���ɂ���
#define	ANIMATION_MATRIX_MAX	(64)

struct TRANSFORM
{
	D3DXVECTOR3 position = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1);
};


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
	
	UINT		BoneIndex[4]{ 
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1 };//0�̓C���f�b�N�X�ŗ��p���邽�߁A255�ő��������Ă��Ȃ��C���f�b�N�X�����m
	
	D3DXVECTOR4 BoneWeight{ 0.0f,0.0f,0.0f,0.0f };
};

struct ANIMATIONMATRIX
{
	D3DXMATRIX Animation[ANIMATION_MATRIX_MAX];
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
	D3DXVECTOR4 Color;

};

struct PointLightInfo
{
	D3DXVECTOR3 color;
	FLOAT intensity;
	D3DXVECTOR3 calc;
	FLOAT specular;
};


struct POINTLIGHT
{
	PointLightInfo pointList[LIGHT_MAX];
};

struct EYE
{
	D3DXVECTOR4 Eye;
};

struct FOG
{
	D3DXVECTOR2 fogOffset[2];
	FLOAT texScale;
	FLOAT maxHeight;
	FLOAT minHeight;
	FLOAT dummy0;

	D3DXVECTOR3 color;
	FLOAT	fogEnable;
};

struct EFFECT
{
	D3DXVECTOR4 deferred;

	FOG			fog;
};

struct WORLDMATRIX
{
	D3DXMATRIX worldMatrix;
	D3DXMATRIX worldInverseMatrix;
};

struct VIEWMATRIX
{
	D3DXMATRIX viewMatrix;
	D3DXMATRIX viewOldMatrix;
};

struct PROJMATRIX
{
	D3DXMATRIX projMatrix;
	D3DXMATRIX projOldMatrix;
};

struct CULLING
{
	UINT cullingCount;

	D3DXVECTOR3 dummy;

	D3DXVECTOR4 cullingCenterPos[4];

	D3DXVECTOR4 cullingPos[8];

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
	/*static D3DXMATRIX m_World;
	static D3DXMATRIX m_View;
	static D3DXMATRIX m_Proj;*/
	/*static D3DXMATRIX GetWorldMatrix() { return m_World; }
	static D3DXMATRIX GetViewMatrix() { return m_View; }
	static D3DXMATRIX GetProjectionMatrix() { return m_Proj; }*/
	static EFFECT m_Effect;
	static void SetEffect();

	static void CreateRenderFormat(DXGI_FORMAT dxgi_format, ID3D11Texture2D** pTexture, ID3D11RenderTargetView** pRTV, ID3D11ShaderResourceView** pSRV);
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
	//���[�V����
	static ID3D11Texture2D* m_pMotionTexture;
	static ID3D11RenderTargetView* m_pMotion_RTV;
	static ID3D11ShaderResourceView* m_pMotion_SRV;
	//�f�v�X
	static ID3D11Texture2D* m_pDepthTexture;
	static ID3D11RenderTargetView* m_pDepth_RTV;
	static ID3D11ShaderResourceView* m_pDepth_SRV;



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
	//======================================================

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

	RENDERER() {}	//�@�R���X�g���N�^
	~RENDERER() {}	//�@�f�X�g���N�^

	//�����o�֐�
	static HRESULT Init(D3D_INIT*);//�@������
	static void Uninit();
	static void ShowFPS();			//�@FPS�\��
	static void Clear();			//�@�N���A


	static void CreateVertexShader(ID3D11VertexShader**,ID3D11InputLayout**, D3D11_INPUT_ELEMENT_DESC* ,UINT , const char* );
	static void CreatePixelShader(ID3D11PixelShader**,const char* FileName);
	static void CreateComputeShader(const char*, ID3D11ComputeShader**);
	static void CreateStructuredBuffer(UINT elementSize,UINT count,void* pInitData,ID3D11Buffer** ppBufferOut);

	static void Deferred();//�f�B�t�@�[�h
	static void DirectionlLighting();//���C�e�B���O
	static void PointLighting();//���C�e�B���O
	static void EffectDraw();
	static void Present();		//�@��ʍX�V
};
