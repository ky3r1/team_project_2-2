#pragma once
#include"Graphics/Shader.h"

#include "all_include.h"


//前方宣言 (相互インクルードしないように)
class ProjectileManager;

//弾丸
class Projectile
{
public:
    //登録されるマネージャーを保持するように
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;
    //描画処理
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    //デバックプリミティブ描画
    virtual void DrawDebugPrimitive();
    //破棄
    void Destroy();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition()const { return position; }
    //方向取得
    const DirectX::XMFLOAT3& GetDirection()const { return direction; }
    //スケール取得
    const DirectX::XMFLOAT3& GetScale()const { return scale; }
    //半径取得
    float GetRadius()const { return radius; }
protected:
    //行列更新処理
    void UpdateTransform();
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 direction = { 0,0,1 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    ProjectileManager* manager = nullptr;
    float radius = 0.5f;
};