#include "EnemySlime.h"

//コンストラクタ
EnemySlime::EnemySlime(int category,int index)
{
    //TODO:エネミースライムのステータス設定
    model = new Model("Data/Model/Enemy/enemy.mdl");

    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.006f;

    radius = 0.5f;//当たり判定の幅、半径
    height = 1.0f;//当たり判定の高さ
    health = 1.0f;
    //color = { 1,0,0,1 };

    enemy_num = index;
    switch (index)
    {
    case 0:
        position = (DirectX::XMFLOAT3(2.0f, 0, 5));
        break;
    case 1:
        position = (DirectX::XMFLOAT3(4.0f, 0, 5));
        break;
    case 2:
        position = (DirectX::XMFLOAT3(-2.0f, 0, 5));
        break;
    case 3:
        position = (DirectX::XMFLOAT3(-4.0f, 0, 5));
        break;
    case 4:
        position = (DirectX::XMFLOAT3(0.0f, 0, 5));
        break;
    case 5:
        position = (DirectX::XMFLOAT3(10.0f, 0, 5));
        break;
    case 6:
        position = (DirectX::XMFLOAT3(-20.0f, 0, 20));
        break;
    case 7:
        position = (DirectX::XMFLOAT3(30.0f, 0, 50));
        break;

    }

    enemy_speed = { 6.0f,2.0f };//X:エネミーのスピード、Y:エネミーのターンスピード
    enemy_category = category;
    ChangeColor(color, enemy_category);
}

//デストラクタ
EnemySlime::~EnemySlime()
{
    delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
    elapestime = elapsedTime;
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
    shader->Draw(dc, model, color);
}

void EnemySlime::MoveEnemy(Player* player)
{
    // ターゲット方向への進行ベクトルを算出
    DirectX::XMFLOAT3 targetPosition = player->GetPosition();
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // 移動処理
    Move(vx, vz, enemy_speed.x * 0.5f);
    Turn(elapestime, vx, vz, enemy_speed.y * 0.5f);
}

//死亡したときに呼ばれる
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
