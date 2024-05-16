#include "enemy.h"
#include"Graphics/Graphics.h"
#include"enemy_manager.h"

//破棄
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////衝突用のデバッグ球描画
    //debugRenderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
