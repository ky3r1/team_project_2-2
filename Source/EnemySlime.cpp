#include "EnemySlime.h"

//�R���X�g���N�^
EnemySlime::EnemySlime(int category,int index)
{
    //TODO:�G�l�~�[�X���C���̃X�e�[�^�X�ݒ�
    model = new Model("Data/Model/Slime/Slime.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;//�����蔻��̕��A���a
    height = 1.0f;//�����蔻��̍���

    enemy_num = index;
    switch (index)
    {
    case 0:
        position = (DirectX::XMFLOAT3(2.0f, 0, 5));
        break;
    case 1:
        position = (DirectX::XMFLOAT3(4.0f, 0, 5));
        break;
    }

    enemy_movespeed = { 0.05f,0.05f };//X:�G�l�~�[��X���X�s�[�h�AY:�G�l�~�[��Z���X�s�[�h
    enemy_category = category;
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
    DirectX::XMFLOAT3 player_position = player->GetPosition();

    if (position.x > player_position.x)
    {
        //position.x -= 0.1f;
        position.x -= enemy_movespeed.x;
        if (position.x < player_position.x)
        {
            position.x = player_position.x;
        }
    }
    if (position.x < player_position.x)
    {
        //position.x += 0.1f;
        position.x += enemy_movespeed.x;
        if (position.x > player_position.x)
        {
            position.x = player_position.x;
        }
    }
    if (position.z > player_position.z)
    {
        position.z -= enemy_movespeed.y;
        if (position.z < player_position.z)
        {
            position.z = player_position.z;
        }
    }
    if (position.z < player_position.z)
    {
        position.z += enemy_movespeed.y;
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
    if (enemy_num != 0)    ImGui::Separator();
    std::string p = std::string("position") + std::to_string(enemy_num);
    ImGui::SliderFloat3(p.c_str(), &position.x, -5, 5);
    std::string s = std::string("scale") + std::to_string(enemy_num);
    ImGui::SliderFloat3(s.c_str(), &scale.x, 0.01f, 4.0f);
    std::string a = std::string("angle") + std::to_string(enemy_num);
    ImGui::SliderFloat3(a.c_str(), &angle.x, -3.14f, 3.14f);
}
