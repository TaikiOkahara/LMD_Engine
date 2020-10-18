#include "InstanceObject.h"


void CInstanceGameObject::InitInstance()
{
    RENDERER::CreateComputeShader("InstanceComputeShader.cso", &m_pComputeShader);


    {//マトリクス構造体バッファ

        RENDERER::CreateStructuredBuffer(sizeof(D3DXMATRIX), m_MatrixList.size(), &m_MatrixList[0], &m_pMatrixBuffer);


        //シェーダリソースビューも作る必要がある
        //また、Formatは必ずDXGI_FORMAT_UNKNOWNにしないといけない
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_MatrixList.size();

        RENDERER::m_pDevice->CreateShaderResourceView(m_pMatrixBuffer, &srvDesc, &m_pMatrixBufferSRV);


        RENDERER::m_pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);
        RENDERER::m_pDeviceContext->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
    }

    {//スタック操作を行うバッファ作成
        //AppendStructuredBufferとConsumeStructuredBufferはStructuredBufferと同じ設定でバッファを作成する
        D3D11_BUFFER_DESC desc = {};
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.ByteWidth = sizeof(UINT) * m_MatrixList.size();//インデックスリスト
        desc.StructureByteStride = sizeof(UINT);
        desc.Usage = D3D11_USAGE_DEFAULT;
        auto hr = RENDERER::m_pDevice->CreateBuffer(&desc, nullptr, &m_pIndexBuffer);
        assert(SUCCEEDED(hr));

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        uavDesc.Format = DXGI_FORMAT_UNKNOWN;
        uavDesc.Buffer.FirstElement = 0;
        uavDesc.Buffer.NumElements = m_MatrixList.size();
        uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
        hr = RENDERER::m_pDevice->CreateUnorderedAccessView(m_pIndexBuffer, &uavDesc, &m_pIndexBufferUAV);
        assert(SUCCEEDED(hr));
    }



    {//
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = sizeof(UINT);
        desc.Usage = D3D11_USAGE_STAGING;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        auto hr = RENDERER::m_pDevice->CreateBuffer(&desc, nullptr, &m_pCounterBuffer);
        assert(SUCCEEDED(hr));

    }

    m_MeshCount = m_MatrixList.size();
}

void CInstanceGameObject::UninitInstance()
{
    SAFE_RELEASE(m_pComputeShader);

    SAFE_RELEASE(m_pMatrixBuffer);
    SAFE_RELEASE(m_pMatrixBufferSRV);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pIndexBufferUAV);
    SAFE_RELEASE(m_pCounterBuffer);
}

void CInstanceGameObject::UpdateInstance()
{

    //視錐台カリング用コンピュートシェーディング
    /*
    RENDERER::m_pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);
    RENDERER::m_pDeviceContext->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);



    RENDERER::m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pIndexBufferUAV, nullptr);
    RENDERER::m_pDeviceContext->Dispatch(m_MatrixList.size(), 1, 1);

    //インデックス数をCPU側で取得する
    RENDERER::m_pDeviceContext->CopyStructureCount(m_pCounterBuffer, 0, m_pIndexBufferUAV);



    HRESULT hr = S_OK;
    D3D11_MAPPED_SUBRESOURCE mapped;

    //インデックス数取得
    hr = RENDERER::m_pDeviceContext->Map(m_pCounterBuffer, 0, D3D11_MAP_READ, 0, &mapped);
    if (SUCCEEDED(hr)) {
        auto* pData = static_cast<UINT*>(mapped.pData);

        m_MeshCount = pData[0];
        RENDERER::m_pDeviceContext->Unmap(m_pCounterBuffer, 0);
    }
    */
}

void CInstanceGameObject::DrawInstance()
{
    RENDERER::m_pDeviceContext->VSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
}