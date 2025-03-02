/*---------------------------------------
*　instanceObject.cpp
*
*@author：Okahara Taiki
----------------------------------------*/
#include "instanceObject.h"
#include <array>
#include "base.h"
#include "camera.h"

ID3D11ComputeShader* CInstanceGameObject::m_pComputeShader = nullptr;
ID3D11VertexShader* CInstanceGameObject::m_pInstanceVertexShader = NULL;

void CInstanceGameObject::InitInstance()
{
   
    std::vector<D3DXMATRIX> matrix;

    m_MeshMax = m_TransformList.size();
    
    for (int i = 0; i < m_MeshMax; i++)
    {

        //　マトリクス設定
        D3DXMATRIX world, scale, rot, trans;
        D3DXMatrixScaling(&scale, m_TransformList[i].scale.x, m_TransformList[i].scale.y, m_TransformList[i].scale.z);
        D3DXMatrixRotationYawPitchRoll(&rot, m_TransformList[i].rotation.y, m_TransformList[i].rotation.x, m_TransformList[i].rotation.z);
        D3DXMatrixTranslation(&trans, m_TransformList[i].position.x, m_TransformList[i].position.y, m_TransformList[i].position.z);
        world = scale * rot * trans;
        D3DXMatrixTranspose(&world, &world);

        matrix.push_back(world);
    }
    {//マトリクス構造体

        RENDERER::CreateStructuredBuffer(sizeof(D3DXMATRIX), m_MeshMax, &matrix[0], &m_pMatrixBuffer);


        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_MeshMax;

        RENDERER::GetDevice()->CreateShaderResourceView(m_pMatrixBuffer, &srvDesc, &m_pMatrixBufferSRV);


        RENDERER::GetDeviceContext()->CSSetShader(m_pComputeShader, nullptr, 0);
        RENDERER::GetDeviceContext()->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
    }

    std::vector<D3DXMATRIX>().swap(matrix);
    matrix.clear();


    {//スタックバッファ
        
        D3D11_BUFFER_DESC desc = {};
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.ByteWidth = sizeof(UINT) * m_MeshMax;//インデックスリスト
        desc.StructureByteStride = sizeof(UINT);
        desc.Usage = D3D11_USAGE_DEFAULT;
        auto hr = RENDERER::GetDevice()->CreateBuffer(&desc, nullptr, &m_pIndexBuffer);
        assert(SUCCEEDED(hr));

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = m_MeshMax;
        uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
        hr = RENDERER::GetDevice()->CreateUnorderedAccessView(m_pIndexBuffer, &uavDesc, &m_pIndexBufferUAV);
        assert(SUCCEEDED(hr));
    }
    {//出力インデックスバッファ


        D3D11_BUFFER_DESC desc = {};
        memset(&desc, 0, sizeof(desc));
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = sizeof(UINT);
        desc.ByteWidth = sizeof(UINT) * m_TransformList.size();
     
        RENDERER::GetDevice()->CreateBuffer(&desc, nullptr, &m_pOutIndexBuffer);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_TransformList.size();

        RENDERER::GetDevice()->CreateShaderResourceView(m_pOutIndexBuffer, &srvDesc, &m_pOutIndexBufferSRV);

    }
    {// 描画インスタンス数バッファ
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = sizeof(UINT);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        auto hr = RENDERER::GetDevice()->CreateBuffer(&desc, nullptr, &m_pCounterBuffer);
        assert(SUCCEEDED(hr));

    }

    RENDERER::GetDeviceContext()->CSSetShader(m_pComputeShader, nullptr, 0);
    RENDERER::GetDeviceContext()->CSSetShaderResources(1, 1, &m_pOutIndexBufferSRV);

  
    //コリジョン
    m_CullingCount = m_MeshCount = m_MeshMax;
    
    m_CullingPos[0] = D3DXVECTOR4( m_Collision.GetVertex()[0].x ,m_Collision.GetVertex()[0].y,m_Collision.GetVertex()[0].z ,0);
    m_CullingPos[1] = D3DXVECTOR4( m_Collision.GetVertex()[1].x ,m_Collision.GetVertex()[1].y,m_Collision.GetVertex()[1].z ,0);
    m_CullingPos[2] = D3DXVECTOR4( m_Collision.GetVertex()[2].x ,m_Collision.GetVertex()[2].y,m_Collision.GetVertex()[2].z ,0);
    m_CullingPos[3] = D3DXVECTOR4( m_Collision.GetVertex()[3].x ,m_Collision.GetVertex()[3].y,m_Collision.GetVertex()[3].z ,0);
    m_CullingPos[4] = D3DXVECTOR4( m_Collision.GetVertex()[4].x ,m_Collision.GetVertex()[4].y,m_Collision.GetVertex()[4].z ,0);
    m_CullingPos[5] = D3DXVECTOR4( m_Collision.GetVertex()[5].x ,m_Collision.GetVertex()[5].y,m_Collision.GetVertex()[5].z ,0);
    m_CullingPos[6] = D3DXVECTOR4( m_Collision.GetVertex()[6].x ,m_Collision.GetVertex()[6].y,m_Collision.GetVertex()[6].z ,0);
    m_CullingPos[7] = D3DXVECTOR4( m_Collision.GetVertex()[7].x ,m_Collision.GetVertex()[7].y,m_Collision.GetVertex()[7].z ,0);
}

void CInstanceGameObject::UninitInstance()
{

    SAFE_RELEASE(m_pMatrixBuffer);
    SAFE_RELEASE(m_pMatrixBufferSRV);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pIndexBufferUAV);
    SAFE_RELEASE(m_pOutIndexBuffer);
    SAFE_RELEASE(m_pOutIndexBufferSRV);

    SAFE_RELEASE(m_pCounterBuffer);
    SAFE_RELEASE(m_pCullingBuffer);

    std::vector<TRANSFORM>().swap(m_TransformList);
    m_TransformList.clear();

}

void CInstanceGameObject::UpdateInstance()
{
    //視錐台カリング実行判定
    if (!RENDERER::GetConstantList().GetStruct<ToggleBuffer>()->GetFrustumCullingEnable())
    {
        m_MeshCount = m_MeshMax;
        return;
    }

    RENDERER::GetConstantList().GetStruct<CullingBuffer>()->SetCulling(m_CullingCount,m_CullingPos);


    //視錐台カリング用コンピュートシェーディング
    
    RENDERER::GetDeviceContext()->CSSetShader(m_pComputeShader, nullptr, 0);
    RENDERER::GetDeviceContext()->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
    RENDERER::GetDeviceContext()->CSSetShaderResources(1, 1, &m_pOutIndexBufferSRV);

    std::array<UINT, 2> countPointer{ 0,0 };

    RENDERER::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_pIndexBufferUAV,countPointer.data());

    //インスタンス数から最適なスレッド数を計算
    const int ThreadGroup = 32;
    UINT dcount = (m_TransformList.size() + ThreadGroup - 1) / ThreadGroup;

    RENDERER::GetDeviceContext()->Dispatch(dcount, 1, 1);

    //インデックス数をCPU側で取得する
    RENDERER::GetDeviceContext()->CopyStructureCount(m_pCounterBuffer, 0, m_pIndexBufferUAV);

    RENDERER::GetDeviceContext()->CopyResource(m_pOutIndexBuffer, m_pIndexBuffer);

    HRESULT hr = S_OK;
    D3D11_MAPPED_SUBRESOURCE mapped;

    //インデックス数取得
    hr = RENDERER::GetDeviceContext()->Map(m_pCounterBuffer, 0, D3D11_MAP_READ, 0, &mapped);
    if (SUCCEEDED(hr)) {
        auto pData = static_cast<UINT*>(mapped.pData);
        m_MeshCount = pData[0];
        RENDERER::GetDeviceContext()->Unmap(m_pCounterBuffer, 0);
    }
    
}

void CInstanceGameObject::DrawInstance()
{
    RENDERER::GetDeviceContext()->VSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
    RENDERER::GetDeviceContext()->VSSetShaderResources(1, 1, &m_pOutIndexBufferSRV);
}

void CInstanceGameObject::Load()
{
    RENDERER::CreateComputeShader("instanceCS.cso", &m_pComputeShader);
    assert(m_pComputeShader);
    RENDERER::CreateVertexShader(&m_pInstanceVertexShader, nullptr, nullptr, 0, "instanceVS.cso");
    assert(m_pInstanceVertexShader);
}

void CInstanceGameObject::Unload()
{
    m_pComputeShader->Release();
    m_pInstanceVertexShader->Release();
}
