#include "EnemySlime.h"

//コンストラクタ
EnemySlime::EnemySlime(int category)
{
    //TODO:エネミースライムのステータス設定
    model = new Model("Data/Model/Slime/Slime.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;

    radius = 0.5f;//当たり判定の幅、半径
    height = 1.0f;//当たり判定の高さ

    enemy_movespeed = { 0.05f,0.05f };//X:エネミーのX軸スピード、Y:エネミーのZ軸スピード
    enemy_category = category;
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新
    UpdateInvincibleTime(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
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

//死亡したときに呼ばれる
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
