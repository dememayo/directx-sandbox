#pragma once

#include <windows.h>
#ifdef SANDBOX_RENDERER_DX12
#   include <d3d12.h>
#   include <d3dx12.h>
#   include <dxgi1_6.h>
#   include <wrl.h>
#   include <wrl/client.h>
#else
#   include <d3d11_1.h>
#endif
#include <DirectXColors.h>
#include <stdexcept>
#include <memory>

#define SAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }

inline std::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), g_hr(hr) {}
    HRESULT Error() const { return g_hr; }
private:
    const HRESULT g_hr;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}