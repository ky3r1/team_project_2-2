#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//スライム
class EnemySlime : public Enemy
{
public:
    EnemySlime(int category,int index);
    ~EnemySlime() override;

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    void MoveEnemy(Player* player)override;
protected:
    //死亡したときに呼ばれる
    void OnDead() override;

    //デバッグ用GUI
    void DrawDebugGUI() override;

private:
    Model* model = nullptr;
    int elapestime;
};