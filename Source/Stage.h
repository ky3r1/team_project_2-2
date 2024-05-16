#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"

class Stage
{
public:
    Stage();
    ~Stage();

    void Update(float elapsedTime);

    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

    //インスタンス取得
    static Stage& Instance();

private:
    Model* model = nullptr;
};