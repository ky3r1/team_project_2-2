#include"ProjectileStraight.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Sword/Sword.mdl");
    //表示サイズの調節
    //scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
}
//デストラクタ
ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer-=elapsedTime;
    if (lifeTimer < 0)
    {
        //自分を削除
        Destroy();
    }
    //移動
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;// x の移動処理（位置 += 向き * 速さ）
    position.y += direction.y * speed;// y の移動処理
    position.z += direction.z * speed;// z の移動処理

    //オブジェクト行列更新
    UpdateTransform();
    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}
