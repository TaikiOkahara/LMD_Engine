#pragma once
#include "gameobject.h"
#include "collision.h"
#include "effect.h"




class CEffectSample : public CEffect
{
private:
	
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11ComputeShader* m_pComputeShader;


	ID3D11UnorderedAccessView* m_TextureUAV = nullptr;
	ID3D11Texture2D* m_Texture = nullptr;

	bool m_Enable = true;
public:
	CEffectSample(){}
	~CEffectSample(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};
