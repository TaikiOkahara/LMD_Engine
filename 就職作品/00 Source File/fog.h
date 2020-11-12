#pragma once
#pragma once
#include "gameobject.h"
#include "collision.h"

class CFog : public CGameObject
{
private:
	EFFECT m_Effect;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11ShaderResourceView* m_FogTexture = NULL;
	bool isEnableCollision = true;
public:
	CFog() {}
	~CFog() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();
};