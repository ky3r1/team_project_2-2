#include "StageWall.h"

int timer;

StageWall::StageWall(int category)
{
    //position.y = 5.0f;
    switch (category)
    {
    case 0:
        position.z = 10.0f;
        scale.x = scale.y = 3.0f;
        scale.z = 0.5f;
        break;
    case 1:
        position.z = 10.0f;
        position.x = 10.0f;
        scale.x = scale.y = 3.0f;
        scale.z = 0.5f;
        break;
    }
    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageWall::~StageWall()
{
    delete model;
}

void StageWall::Update(float elapsedTime)
{
    position.z -= 2.0f / 60.0f;
    //�s��X�V
    UpdateTransform();
    //���f���s��X�V
    model->UpdateTransform(transform);
    timer++;
}

void StageWall::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�̃��f���`��
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
