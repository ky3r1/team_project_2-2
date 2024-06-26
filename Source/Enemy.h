#pragma once
#include "Graphics/Shader.h"
#include "Character.h"
#include "Player.h"

//エネミー
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    virtual void MoveEnemy(Player* player) = 0;

    void DrewDebugPrimitive();

    //破棄
    void Destroy();

    //エネミーの種類
    int GetEnemyCategory() { return enemy_category; }
protected:
    int enemy_category;
    int enemy_num;
    DirectX::XMFLOAT2 enemy_speed;
};