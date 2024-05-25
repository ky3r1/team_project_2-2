#include "Enemy.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"

//デバッグプリミティブ描画
void Enemy::DrewDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    switch (enemy_category)
    {
    case RED:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
        break;
    case GREEN:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    case BLUE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));
        break;
    case YELLOW:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 0, 1));
        break;
    default:
        break;
    }
}

//破棄
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}