/*---------------------------------------
*　struct.h
*
* 構造体定義群
*@author：Okahara Taiki
----------------------------------------*/
#pragma once

#define	LIGHT_MAX		(6)	//ライトの数を有限にする
#define	ANIMATION_MATRIX_MAX	(64)

//GameObjectクラスのコンポーネント
struct TRANSFORM
{
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 rotation = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 scale = D3DXVECTOR3(1, 1, 1);
};

//頂点構造体
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexturePos;

	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
};

//Deferred用ビルボード頂点構造体
struct DEFERRED_VERTEX
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR2 TexturePos;
};

//アニメーション頂点構造体
struct ANIMVERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR2 TexturePos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;

	UINT		BoneIndex[4]{
		ANIMATION_MATRIX_MAX - 1,//エラー検知のため要素最大値代入(VertexShader側で検知)
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1,
		ANIMATION_MATRIX_MAX - 1 };

	D3DXVECTOR4 BoneWeight{ 0.0f,0.0f,0.0f,0.0f };
};


//GBuffer生成構造体
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


struct ANIMATIONMATRIX
{
	D3DXMATRIX Animation[ANIMATION_MATRIX_MAX];
};

struct POINTLIGHT
{
	D3DXVECTOR3 color;
	FLOAT		intensity;
	D3DXVECTOR3 calc;
	FLOAT		size;
	D3DXVECTOR3 pos;
	UINT		index;
};

//struct CAMERA
//{
//	D3DXVECTOR4 eyePos;
//
//	D3DXVECTOR4 worldCamera[4];
//};

struct FOG
{
	D3DXVECTOR2 fogOffset[2];
	FLOAT texScale;
	FLOAT maxHeight;
	FLOAT minHeight;
	FLOAT dummy0;

	D3DXVECTOR3 color;
	BOOL fogEnable;
};


class ConstantBuffer 
{
protected:
	ID3D11Buffer* m_pBuffer;
	unsigned int m_slotNum;
	unsigned int m_StructSize;

public:
	ConstantBuffer():m_pBuffer(nullptr),m_slotNum(0),m_StructSize(0) {}
	virtual ~ConstantBuffer() 
	{
		m_pBuffer->Release();
	}

	void CreateBuffer(unsigned int slot);


};

class ConstantBufferList
{
private:
	std::list<ConstantBuffer*> m_CBufferList;

public:

	void Uninit()
	{
		for (ConstantBuffer* buff : m_CBufferList)
		{
			delete buff;
		}
		m_CBufferList.clear();
	}

	template <typename T>
	T* AddConstantBuffer(unsigned int slot)
	{
		T* buff = new T();
		m_CBufferList.push_back(buff);
		buff->CreateBuffer(slot);

		return buff;
	};

	template <typename T>
	T* GetStruct(void)
	{
		for (ConstantBuffer* buff : m_CBufferList)
		{
			if (typeid(*buff) == typeid(T))
			{
				return (T*)buff;
			}
		}
		return nullptr;
	};
};

//ConstantBuffer構造体---------------------------------------------------------------------------


class ToggleBuffer : public ConstantBuffer
{
private:
	struct Struct
	{
		BOOL frustumCullingEnable;
		D3DXVECTOR3 dummy;
	};


	Struct str;

public:
	void SetFrustumCullingEnable(bool set) { str.frustumCullingEnable = set; }
	void Set();
	bool GetFrustumCullingEnable() { return str.frustumCullingEnable; }

	ToggleBuffer() {m_StructSize = sizeof(Struct);}
	~ToggleBuffer() {}
};

class CullingBuffer : public ConstantBuffer
{
private:
	struct Struct
	{
		UINT cullingCount;

		D3DXVECTOR3 dummy;

		D3DXVECTOR4 cullingPos[8];

		D3DXVECTOR4 worldCamera[4];
	};

	Struct str;
public:
	
	void SetCulling(UINT count, D3DXVECTOR4* pos);
	void SetWorldCamera(D3DXVECTOR4* worldCamera);
	CullingBuffer()
	{
		m_StructSize = sizeof(Struct);
	}
	~CullingBuffer() {}
};

class EffectBuffer : public ConstantBuffer
{
private:
	struct Struct
	{
		FLOAT deferred;
		BOOL shadow;
		FLOAT dummy;
		BOOL GBufferDrawEnable;

		FOG	fog;

		FLOAT aoEnable;
		FLOAT aoHemRedius;
		FLOAT aoZfar;
		FLOAT aoPower;
	};
	Struct str;
public:
	void Set();
	void SetFog(FOG set) { str.fog = set; }
	void SetAO(FLOAT enable,FLOAT redius,FLOAT zfar, FLOAT power) 
	{ 
		str.aoEnable = enable;
		str.aoHemRedius = redius;
		str.aoZfar = zfar;
		str.aoPower = power;
	}
	void SetDeferredParam(FLOAT setDefferd,BOOL shadow, BOOL enable) { 
		str.deferred = setDefferd;
		str.shadow = shadow;
		str.GBufferDrawEnable = enable; }
	FLOAT GetDeferredParam() { return str.deferred; }

	EffectBuffer()	{
		m_StructSize = sizeof(Struct);
	}
	~EffectBuffer() {}
};

class AnimationBuffer : public ConstantBuffer
{
public:
	void Set(ANIMATIONMATRIX set);

	AnimationBuffer(){m_StructSize = sizeof(ANIMATIONMATRIX);}
	~AnimationBuffer() {}
};

class CameraBuffer : public ConstantBuffer
{
	struct Struct
	{
		D3DXVECTOR4 cameraPosition;
		D3DXVECTOR4 cullingCameraPosition;//デモ用仮想カメラ
	};
	Struct str;
public:
	void SetCamera(D3DXVECTOR4 cameraPos);
	void SetCullingCameraPos(D3DXVECTOR4 cullCamera);

	CameraBuffer() { m_StructSize = sizeof(Struct); }
	~CameraBuffer() {}
};

class DirectionalLightBuffer : public ConstantBuffer
{
	struct Struct
	{
		D3DXVECTOR4 dir;
		D3DXVECTOR4 pos;
		D3DXVECTOR4 col;
		
		D3DXMATRIX projView;
	};
	Struct str;
public:
	void SetDirectinalLight(D3DXVECTOR4 setDir, D3DXVECTOR4 setPos, D3DXVECTOR4 setCol);
	void SetMatrix(D3DXMATRIX set);


	DirectionalLightBuffer() { m_StructSize = sizeof(Struct); }
	~DirectionalLightBuffer() {}
};

class PointLightBuffer : public ConstantBuffer
{
	struct Struct
	{
		POINTLIGHT pointlight;
		D3DXMATRIX lightVP[8];
		D3DXVECTOR4 playerPos;
	};
	Struct str;
public:
	void Set(POINTLIGHT set);
	void SetIndex(int index);
	void SetLightVP(D3DXMATRIX mat, int num);
	void SetPlayerPos(D3DXVECTOR4 pos);
	PointLightBuffer() { m_StructSize = sizeof(Struct); }
	~PointLightBuffer() {}
};

class WorldBuffer : public ConstantBuffer
{
public:
	void Set(D3DXMATRIX mat);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }
	WorldBuffer() { m_StructSize = sizeof(D3DXMATRIX) * 2; }
	~WorldBuffer() {}
};

class ViewBuffer : public ConstantBuffer
{
public:
	void Set(D3DXMATRIX mat, D3DXMATRIX old);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }

	ViewBuffer() { m_StructSize = sizeof(D3DXMATRIX) * 2; }
	~ViewBuffer() {}
};

class ProjBuffer : public ConstantBuffer
{
public:
	void Set(D3DXMATRIX mat, D3DXMATRIX old);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }

	ProjBuffer() { m_StructSize = sizeof(D3DXMATRIX) * 3; }
	~ProjBuffer() {}
};



//ConstantBuffer構造体---------------------------------------------------------------------------