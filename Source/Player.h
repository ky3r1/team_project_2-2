#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"

class Player : public Character
{
public:
    Player();
    ~Player() override;

    //更新処理
    void Update(float olapsedTime);

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //デバッグ用GUI
    void DrawDebugGUI() override;

    //死亡判定
    bool PlayerDead();
private:
    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //移動処理
    //void Move(float elapsedTime, float vx, float vz, float speed);

    //移動入力処理
    void InputMove(float elapsedTime);

    //旋回処理
    //void Turn(float elapsedTime, float vx, float vz, float speed);

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    //弾丸入力処理
    void InputProjectile();

protected:

    void OnLanding() override;

    //ジャンプ入力処理
    void InputJump();

    void ProjectileStraightWay(int category,float angle);
private:
    Model* model = nullptr;
    float       moveSpeed = 5.0f;
    float       turnSpeed = DirectX::XMConvertToRadians(720);
    float       jumpSpeed = 20.0f;
    /*float       gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };*/

    int         jumpCount = 0;
    int         jumpLimit = 2;
    DelayTime projectile_allangle;
    DelayTime projectile_auto;

    ProjectileManager projectileManager;

    Effect*     hitEffect = nullptr;
};