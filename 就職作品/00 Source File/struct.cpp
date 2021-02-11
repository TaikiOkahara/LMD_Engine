/*---------------------------------------
*　struct.cpp
*
* 構造体定義群
*@author：Okahara Taiki
----------------------------------------*/
#include "director.h"
#include "struct.h"
#include "renderer.h"


void ConstantBuffer::CreateBuffer(unsigned int slot)
{
	m_pBuffer = nullptr;
	m_slotNum = slot;


	//　コンスタントバッファ生成
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	buffer_desc.ByteWidth = m_StructSize;
	RENDERER::GetDevice()->CreateBuffer(&buffer_desc, NULL, &m_pBuffer);
	RENDERER::GetDeviceContext()->VSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);
	RENDERER::GetDeviceContext()->PSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);
	RENDERER::GetDeviceContext()->CSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);

}

void ToggleBuffer::Set()
{
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void CullingBuffer::SetCulling(UINT count, D3DXVECTOR4* pos)
{
	str.cullingCount = count;
	for (int i = 0; i < 8; i++)
	{
		str.cullingPos[i] = pos[i];
	}

	

	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void CullingBuffer::SetWorldCamera(D3DXVECTOR4* worldCamera)
{
	for (int i = 0; i < 4; i++)
	{
		str.worldCamera[i] = worldCamera[i];
	}

	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}


void EffectBuffer::Set()
{
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}


void AnimationBuffer::Set(ANIMATIONMATRIX set)
{
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &set, 0, 0);
}

void CameraBuffer::SetCamera(D3DXVECTOR4 cameraPos)
{
	str.cameraPosition = cameraPos;
	
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void CameraBuffer::SetCullingCameraPos(D3DXVECTOR4 cullCamera)
{
	str.cullingCameraPosition = cullCamera;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void DirectionalLightBuffer::SetDirectinalLight(D3DXVECTOR4 setDir, D3DXVECTOR4 setPos, D3DXVECTOR4 setCol)
{
	str.dir = setDir;
	str.pos = setPos;
	str.col = setCol;

	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void DirectionalLightBuffer::SetMatrix(D3DXMATRIX set)
{
	str.projView = set;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);

}

void PointLightBuffer::Set(POINTLIGHT set)
{
	str.pointlight = set;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void PointLightBuffer::SetIndex(int index)
{
	str.pointlight.index = index;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void PointLightBuffer::SetLightVP(D3DXMATRIX mat, int num)
{
	D3DXMATRIX set = mat;
	D3DXMatrixTranspose(&set, &set);
	str.lightVP[num] = set;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void PointLightBuffer::SetPlayerPos(D3DXVECTOR4 pos)
{
	str.playerPos = pos;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void WorldBuffer::Set(D3DXMATRIX mat)
{
	D3DXMATRIX set[2];

	D3DXMatrixTranspose(&mat, &mat);
	set[0] = mat;
	
	D3DXMatrixInverse(&mat, NULL, &mat);
	D3DXMatrixTranspose(&mat, &mat);
	set[1] = mat;


	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);
}

void ViewBuffer::Set(D3DXMATRIX mat, D3DXMATRIX old)
{
	D3DXMATRIX set[2];
	D3DXMatrixTranspose(&mat, &mat);
	D3DXMatrixTranspose(&old, &old);

	set[0] = mat;
	set[1] = old;
	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);
}

void ProjBuffer::Set(D3DXMATRIX mat, D3DXMATRIX old)
{
	D3DXMATRIX set[3];
	D3DXMatrixTranspose(&set[0], &mat);
	D3DXMatrixTranspose(&set[1], &old);


	D3DXMatrixInverse(&set[2], NULL, &mat);
	D3DXMatrixTranspose(&set[2], &set[2]);

	RENDERER::GetDeviceContext()->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);

}