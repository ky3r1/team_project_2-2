#pragma once
#include "Graphics/Model.h"
#include "Projectile.h"

//弾丸直進
class ProjectileStraight : public Projectile
{
public:
    //ProjectileStraight();
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive()override;
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 3.0f;
};