#include "StageWall.h"

StageWall::StageWall()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;
    angle.x = 1.57;

    //ステージモデルを読み込み
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageWall::~StageWall()
{
}

void StageWall::Update(float elapsedTime)
{
}

void StageWall::Render(ID3D11DeviceContext* dc, Shader* shader)
{
}

bool StageWall::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return false;
}
