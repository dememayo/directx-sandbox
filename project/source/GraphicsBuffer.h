#pragma once

#ifdef SANDBOX_RENDERER_DX12
#   include <d3d12.h>
#else
#   include <d3d11_1.h>
#endif
#include <wrl.h>

class GraphicsBuffer
{
#ifdef SANDBOX_RENDERER_DX12
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_CbvHeap;
    UINT8* m_pCbvDataBegin;
private:
    bool CreateBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes);
public:
    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() { return &m_VertexBufferView; }
    D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() { return &m_IndexBufferView; }
    ID3D12DescriptorHeap* GetDescriptorHeap() const { return m_CbvHeap.Get(); }
    UINT8* GetDataBegin() const { return m_pCbvDataBegin; }
    bool CreateVertexBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes);
    bool CreateIndexBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes);
    bool CreateConstantBuffer(ID3D12Device* device, UINT bufferSize, void* initialData, UINT strideInBytes);
#else
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;
private:
    bool CreateBuffer(ID3D11Device* device, UINT bufferSize, void* initialData, D3D11_BIND_FLAG bindFlags);
public:
    ID3D11Buffer* GetBuffer() const { return m_pBuffer.Get(); }
    bool CreateVertexBuffer(ID3D11Device* device, UINT bufferSize, void* initialData = nullptr);
    bool CreateIndexBuffer(ID3D11Device* device, UINT bufferSize, void* initialData = nullptr);
    bool CreateConstantBuffer(ID3D11Device* device, UINT bufferSize, void* initialData = nullptr);
#endif
};