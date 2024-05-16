#pragma once
#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"projectile_manager.h"
#include"Effect.h"



//プレイヤー
class Player :public Character {
public:
    Player();
    ~Player() override;

    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //デバッグ用GUI描画
    void DrawDebugGUI();
    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    ////移動処理
    //void Move(float elapsedTime, float vx,float vz, float speed);
    ////旋回処理
    //void Turn(float elapsedTime, float vx, float vz, float speed);

    //移動入力処理
    void InputMove(float elapsedTime);
private:
    //着地した時に呼ばれる
    void OnLanding()override;
    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;
    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();
    //弾丸と敵の衝突処理
    void CollisionProjectileVsEnemies();
    ////jump処理
    //void Jump(float speed);
    ////速力処理更新
    //void UpdateVelocity(float elapsedTime);

    //jump入力処理
    void InputJump();

    //弾丸発射処理
    void InputProjectile();

private:
    Model* model = nullptr;
    float moveSpeed = 7.5f;
    float turnSpeed = DirectX::XMConvertToRadians(720);

    float jumpSpeed = 20.0f;
    //float gravity = -1.0f;
    //DirectX::XMFLOAT3 velocity = { 0,0,0 };

    int jumpCount = 0;
    int jumpLimit = 2;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
};