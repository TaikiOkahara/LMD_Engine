#pragma once

#define	LIGHT_MAX		(128)	//ライトの数を有限にする
#define	ANIMATION_MATRIX_MAX	(64)

struct TRANSFORM
{
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
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
	D3DXVECTOR2 TexturePos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;

	UINT		BoneIndex[4]{
		ANIMATION_MATRIX_MAX - 1,//エラー検知のため要素最大値代入
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1 };

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
	D3DXVECTOR4 LightPosition;
	D3DXVECTOR4 Color;

};

struct PointLightInfo
{
	D3DXVECTOR3 color;
	FLOAT		intensity;
	D3DXVECTOR3 calc;
	FLOAT		specular;
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

	D3DXVECTOR4 cullingCameraPos[4];

	D3DXVECTOR4 cullingPos[8];

};

struct GBuffer
{
	ID3D11Texture2D* m_pTexture = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11ShaderResourceView* m_pShaderResourceView = nullptr;

	void Release() {
		SAFE_RELEASE(m_pTexture);
		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pShaderResourceView);
	}
};