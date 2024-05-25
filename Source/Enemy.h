#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"

//�G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void MoveEnemy(Player* player) = 0;

    void DrewDebugPrimitive();

    //�j��
    void Destroy();

    //�G�l�~�[�̎��
    int GetEnemyCategory() { return enemy_category; }
protected:
    int enemy_category;
    DirectX::XMFLOAT2 enemy_movespeed;
};