#include "ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager, int category)
    : Projectile(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Bullet/Bullet.mdl");

    color = { 0,1,0,1 };

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.006f;
    radius = 0.5f;
    projectile_category = category;
    ChangeColor(color, projectile_category);
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0)
    {
        //�������폜
        Destroy();
    }
    //�ړ�
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;  //(�ʒu�{������������)
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model,color);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
                                const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}

void ProjectileStraight::DrawDebugGUI()
{
    if (ImGui::TreeNode("ProjectileStraight"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.001f, 4.0f);
        ImGui::SliderFloat3("direction", &direction.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}
