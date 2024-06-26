#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{
    //更新処理
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    //破棄処理
    //※projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
    //  更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
    for (Projectile* projectile : removes)
    {
        //std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(),
            projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);
        }

        //弾丸の破棄処理
        delete projectile;
    }
    //破棄リストをクリア
    removes.clear();
}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(dc, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{
    ImGui::Begin("Enemy");
    ImGui::SetNextTreeNodeOpen(true);
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugPrimitive();
    }
    ImGui::End();

}

void ProjectileManager::DrawDebugGUI()
{
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugGUI();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();
}

void ProjectileManager::Remove(Projectile* projectile)
{
    //破棄リストに追加
    removes.insert(projectile);
}
