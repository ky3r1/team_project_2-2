#pragma once
#include "Stage.h"

class StageWall :public Stage
{
public:
    StageWall();
    ~StageWall()override;

    //更新処理
    void Update(float elapsedTime)override;
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;

    //デバッグ用GUI
    void DrawDebugGUI()override;

};