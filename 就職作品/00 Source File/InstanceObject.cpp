#include "InstanceObject.h"


void CInstanceGameObject::InitInstance()
{
    RENDERER::CreateComputeShader("InstanceComputeShader.cso", &m_pComputeShader);


    {//�}�g���N�X�\���̃o�b�t�@

        RENDERER::CreateStructuredBuffer(sizeof(D3DXMATRIX), m_MatrixList.size(), &m_MatrixList[0], &m_pMatrixBuffer);


        //�V�F�[�_���\�[�X�r���[�����K�v������
        //�܂��AFormat�͕K��DXGI_FORMAT_UNKNOWN�ɂ��Ȃ��Ƃ����Ȃ�
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement = 0;
        srvDesc.Buffer.NumElements = m_MatrixList.size();

        RENDERER::m_pDevice->CreateShaderResourceView(m_pMatrixBuffer, &srvDesc, &m_pMatrixBufferSRV);


        RENDERER::m_pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);
        RENDERER::m_pDeviceContext->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);
    }

    {//�X�^�b�N������s���o�b�t�@�쐬
        //AppendStructuredBuffer��ConsumeStructuredBuffer��StructuredBuffer�Ɠ����ݒ�Ńo�b�t�@���쐬����
        D3D11_BUFFER_DESC desc = {};
        desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.ByteWidth = sizeof(UINT) * m_MatrixList.size();//�C���f�b�N�X���X�g
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

    //������J�����O�p�R���s���[�g�V�F�[�f�B���O
    /*
    RENDERER::m_pDeviceContext->CSSetShader(m_pComputeShader, nullptr, 0);
    RENDERER::m_pDeviceContext->CSSetShaderResources(0, 1, &m_pMatrixBufferSRV);



    RENDERER::m_pDeviceContext->CSSetUnorderedAccessViews(0, 1, &m_pIndexBufferUAV, nullptr);
    RENDERER::m_pDeviceContext->Dispatch(m_MatrixList.size(), 1, 1);

    //�C���f�b�N�X����CPU���Ŏ擾����
    RENDERER::m_pDeviceContext->CopyStructureCount(m_pCounterBuffer, 0, m_pIndexBufferUAV);



    HRESULT hr = S_OK;
    D3D11_MAPPED_SUBRESOURCE mapped;

    //�C���f�b�N�X���擾
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