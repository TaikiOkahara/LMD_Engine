/*---------------------------------------
*�@floor.h
*
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "gameobject.h"
#include "instanceObject.h"
#include "tile.h"

class CFloor : public CInstanceGameObject
{
private:
	Tile* m_pField = nullptr;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();

	//�C�ӂ̍��W�Œn�ʂ̍������v�Z����֐�
	float GetHeight(D3DXVECTOR3 pos) { return m_pField->GetHeight(pos); }
};