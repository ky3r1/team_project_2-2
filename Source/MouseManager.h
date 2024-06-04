#pragma once

#include "Input/Input.h"
#include"StageMain.h"


class MouseManager
{
    void MouseTransform(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);


private:
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
};