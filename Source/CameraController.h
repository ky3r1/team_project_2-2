#pragma once

#include "All.h"

class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    //更新処理
    void Update(float elapsedTime);

    //ターゲット位置設定
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    void DrawDebugGUI();

private:
    DirectX::XMFLOAT3       target = { 0,0,0 };
    DirectX::XMFLOAT3       angle = { /*1.59*/0.403f,0,0 };
    float                   rollSpeed = DirectX::XMConvertToRadians(90);
    float                   range = 15.0f;

    float                   maxAngleX = DirectX::XMConvertToRadians(90);
    float                   minAngleX = DirectX::XMConvertToRadians(-45);
};