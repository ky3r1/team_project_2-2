#include "EnemySlime.h"

//コンストラクタ
EnemySlime::EnemySlime()
{
    model = new Model("Data/Model/Slime/Slime.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.01f;
    //幅、高さの設定
    radius = 0.5f;
    height = 1.0f;
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
    //オブジェクト行列更新
    UpdateTransform();
    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);
    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//死亡した時に呼ばれる
void EnemySlime::OnDead()
{
    // 自信を破棄（Enemy クラス（基底クラス）の Destroy 関数を呼び出す）
    Destroy();
}
