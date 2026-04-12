#include "pch.h"
#include "GraphicsBuffer.h"

#ifdef SANDBOX_RENDERER_DX12

bool GraphicsBuffer::CreateBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes)
{
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(m_pBuffer.GetAddressOf())
    ));

    UINT8* pDataBegin;
    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(m_pBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pDataBegin)));
    memcpy(pDataBegin, initialData, bufferSize);
    m_pBuffer->Unmap(0, nullptr);

    return true;
}

bool GraphicsBuffer::CreateVertexBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes)
{
    if (!CreateBuffer(device, bufferSize, initialData, strideInBytes)) return false;

    m_VertexBufferView.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
    m_VertexBufferView.StrideInBytes = strideInBytes;
    m_VertexBufferView.SizeInBytes = bufferSize;
    return true;
}

bool GraphicsBuffer::CreateIndexBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes)
{
    if (!CreateBuffer(device, bufferSize, initialData, strideInBytes)) return false;

    m_IndexBufferView.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
    m_IndexBufferView.Format = (strideInBytes == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_IndexBufferView.SizeInBytes = bufferSize;
    return true;
}

bool GraphicsBuffer::CreateConstantBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes)
{
    if (!CreateBuffer(device, bufferSize, initialData, strideInBytes)) return false;

    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    ThrowIfFailed(device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_CbvHeap)));

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = bufferSize;
    device->CreateConstantBufferView(&cbvDesc, m_CbvHeap->GetCPUDescriptorHandleForHeapStart());

    CD3DX12_RANGE readRange(0, 0);
    ThrowIfFailed(m_pBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
    memcpy(m_pCbvDataBegin, initialData, bufferSize);

    return true;
}

#else

bool GraphicsBuffer::CreateBuffer(ID3D11Device* device, UINT bufferSize, void* initialData, D3D11_BIND_FLAG bindFlags)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = bufferSize;
    bd.BindFlags = bindFlags;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    D3D11_SUBRESOURCE_DATA* pInitData = nullptr;
    if (initialData)
    {
        initData.pSysMem = initialData;
        pInitData = &initData;
    }
    HRESULT hr = device->CreateBuffer(&bd, pInitData, m_pBuffer.GetAddressOf());

    return !FAILED(hr);
}

bool GraphicsBuffer::CreateVertexBuffer(ID3D11Device* device, UINT bufferSize, void* initialData)
{
    return CreateBuffer(device, bufferSize, initialData, D3D11_BIND_VERTEX_BUFFER);
}

bool GraphicsBuffer::CreateIndexBuffer(ID3D11Device* device, UINT bufferSize, void* initialData)
{
    return CreateBuffer(device, bufferSize, initialData, D3D11_BIND_INDEX_BUFFER);
}

bool GraphicsBuffer::CreateConstantBuffer(ID3D11Device* device, UINT bufferSize, void* initialData)
{
    return CreateBuffer(device, bufferSize, initialData, D3D11_BIND_CONSTANT_BUFFER);
}

#endif