#include "enemy.h"
#include"Graphics/Graphics.h"
#include"enemy_manager.h"

//�j��
void Enemy::Destroy()
{
    EnemyManager::Instance().Remove(this);
}

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////�Փ˗p�̃f�o�b�O���`��
    //debugRenderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}
