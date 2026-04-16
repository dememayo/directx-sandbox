#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
    XMFLOAT3 position = {0, 0, -5};

    float yaw = 0.0f;
    float pitch = 0.0f;

    // Unity風
    XMFLOAT3 target = {0, 0, 0};
    float distance = 5.0f;

    float moveSpeed = 5.0f;
    float mouseSensitivity = 0.002f;

    XMMATRIX GetViewMatrix() const
    {
        XMVECTOR pos = XMLoadFloat3(&position);
        XMVECTOR tgt = XMLoadFloat3(&target);
        XMVECTOR up  = XMVectorSet(0,1,0,0);

        return XMMatrixLookAtLH(pos, tgt, up);
    }
};