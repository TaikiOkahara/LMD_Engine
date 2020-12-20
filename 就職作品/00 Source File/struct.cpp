#include "director.h"
#include "struct.h"
#include "renderer.h"


void ConstantBuffer::CreateBuffer(unsigned int slot)
{
	m_pBuffer = nullptr;
	m_slotNum = slot;


	//　コンスタントバッファ生成
	D3D11_BUFFER_DESC buffer_desc;

	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	buffer_desc.ByteWidth = m_StructSize;
	RENDERER::m_pDevice->CreateBuffer(&buffer_desc, NULL, &m_pBuffer);
	RENDERER::m_pDeviceContext->VSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);
	RENDERER::m_pDeviceContext->PSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);
	RENDERER::m_pDeviceContext->CSSetConstantBuffers(m_slotNum, 1, &m_pBuffer);

}

void ToggleBuffer::Set()
{
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void CullingBuffer::Set(UINT count, D3DXVECTOR4* pos) {
	
	str.cullingCount = count;

	for (int i = 0; i < 8; i++)
	{
		str.cullingPos[i] = pos[i];
	}
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}

void EffectBuffer::Set()
{
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &str, 0, 0);
}


void AnimationBuffer::Set(ANIMATIONMATRIX set)
{
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &set, 0, 0);
}

void EyeBuffer::Set(EYE set)
{
	
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &set, 0, 0);
}

void DirectionalLightBuffer::Set(D3DXVECTOR4 setDir, D3DXVECTOR4 setPos, D3DXVECTOR4 setCol)
{
	D3DXVECTOR4 set[3];
	set[0] = setDir;
	set[1] = setPos;
	set[2] = setCol;

	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, &set, 0, 0);
}

void PointLightBuffer::Set(POINTLIGHT* set)
{
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);
}

void WorldBuffer::Set(D3DXMATRIX mat)
{
	D3DXMATRIX set[2];

	D3DXMatrixTranspose(&mat, &mat);
	set[0] = mat;
	
	D3DXMatrixInverse(&mat, NULL, &mat);
	D3DXMatrixTranspose(&mat, &mat);
	set[1] = mat;


	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);
}

void ViewBuffer::Set(D3DXMATRIX mat, D3DXMATRIX old)
{
	D3DXMATRIX set[2];
	D3DXMatrixTranspose(&mat, &mat);
	D3DXMatrixTranspose(&old, &old);

	set[0] = mat;
	set[1] = old;
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);
}

void ProjBuffer::Set(D3DXMATRIX mat, D3DXMATRIX old)
{
	D3DXMATRIX set[2];
	D3DXMatrixTranspose(&mat, &mat);
	D3DXMatrixTranspose(&old, &old);

	set[0] = mat;
	set[1] = old;
	
	RENDERER::m_pDeviceContext->UpdateSubresource(m_pBuffer, 0, NULL, set, 0, 0);

}
