#pragma once
#include "gameobject.h"
#include "InstanceObject.h"
#include "tile.h"

class CFloor : public CInstanceGameObject
{
private:
	Tile* m_pField = nullptr;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	//ID3D11InputLayout* m_VertexLayout;
public:
	CFloor(){}
	~CFloor(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();

	float GetHeight(D3DXVECTOR3 pos) { return m_pField->GetHeight(pos); }
};