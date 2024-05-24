#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

// 更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //破棄処理
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        delete enemy;
    }

    removes.clear();


    //敵同士の衝突処理
    CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//　エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    //破棄リストに追加
    removes.insert(enemy);
}

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ球を描画
    for (Enemy* enemy : enemies)
    {
        debugRenderer->DrawCylinder(enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(), DirectX::XMFLOAT4(0, 0, 0, 1));
    }
}

void EnemyManager::DrawDebugGUI()
{
    ImGui::Begin("Enemy");
    ImGui::SetNextTreeNodeOpen(true);
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugGUI();
    }
    ImGui::End();
}

void EnemyManager::EnemyMove(Player* player)
{
    for (Enemy* enemy : enemies)
    {
        enemy->MoveEnemy(player);
    }
}

DirectX::XMFLOAT3 EnemyManager::GetPosition()
{
    DirectX::XMFLOAT3 reposition;
    for (Enemy* enemy : enemies)
    {
        reposition=enemy->GetPosition();
    }
    return reposition;
}

//エネミー全削除
void EnemyManager::clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

void EnemyManager::CollisionEnemyVsEnemies()
{
    //すべての敵と総当たりで衝突判定
    int enemyCount = GetEnemyCount();
    for (int i=0;i<enemyCount;i++)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = i+1; j < enemyCount; j++)
        {
            Enemy* enemy2 = GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsSphere(
                enemy1->GetPosition(), enemy1->GetRadius(),enemy1->GetHeight(),
                enemy2->GetPosition(), enemy2->GetRadius(),enemy2->GetHeight(),
                outPosition))
            {
                enemy2->SetPosition(outPosition);
            }
        }
    }
}