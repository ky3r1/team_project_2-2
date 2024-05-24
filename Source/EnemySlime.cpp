#include "EnemySlime.h"

//�R���X�g���N�^
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    //���A�����ݒ�
    radius = 0.5f;
    height = 1.0f;

    enemy_category = 0;
}

//�f�X�g���N�^
EnemySlime::~EnemySlime()
{
    delete model;
}

//�X�V����
void EnemySlime::Update(float elapsedTime)
{

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V
    UpdateInvincibleTime(elapsedTime);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

void EnemySlime::MoveEnemy(Player* player)
{
    float enemy_movespeed = 0.05f;
    DirectX::XMFLOAT3 player_position = player->GetPosition();

    //if (position.x < 0)
    //{
    //    position.x += 0.1f;
    //}
    //else
    //{
    //    position.x -= 0.1f;
    //}
    if (position.x > player_position.x)
    {
        //position.x -= 0.1f;
        position.x -= enemy_movespeed;
        if (position.x < player_position.x)
        {
            position.x = player_position.x;
        }
    }
    if (position.x < player_position.x)
    {
        //position.x += 0.1f;
        position.x += enemy_movespeed;
        if (position.x > player_position.x)
        {
            position.x = player_position.x;
        }
    }
    if (position.z > player_position.z)
    {
        position.z -= enemy_movespeed;
        if (position.z < player_position.z)
        {
            position.z = player_position.z;
        }
    }
    if (position.z < player_position.z)
    {
        position.z += enemy_movespeed;
        if (position.z > player_position.z)
        {
            position.z = player_position.z;
        }
    }
}

//���S�����Ƃ��ɌĂ΂��
void EnemySlime::OnDead()
{
    Destroy();
}

void EnemySlime::DrawDebugGUI()
{
    if (ImGui::TreeNode("EnemySlime"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
        ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}
