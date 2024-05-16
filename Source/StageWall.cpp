#include "StageWall.h"

int timer;

StageWall::StageWall()
{
    //position.y = 5.0f;
    position.x = 1.0f;

    scale.x = scale.y = 3.0f;
    scale.z = 0.5f;

    //ステージモデルを読み込み
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageWall::~StageWall()
{
    delete model;
}

void StageWall::Update(float elapsedTime)
{
    if (timer% 60 == 0)
    {
        position.x += 1.0f;
    }
    if (position.x > 7.0f)
    {
        position.x = 7.0f;
    }
    //行列更新
    UpdateTransform();
    //モデル行列更新
    model->UpdateTransform(transform);
    timer++;
}

void StageWall::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーのモデル描画
    shader->Draw(dc, model);
}

bool StageWall::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageWall::DrawDebugGUI()
{
    if (ImGui::TreeNode("StageWall"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.001f, 4.0f);
        ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}
