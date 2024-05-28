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

    //enemy
    float GetEnemy01(){ return player_score.enemy01; }
    void SetEnemy01(float e) { player_score.enemy01 =e; }
    float GetEnemy02() { return player_score.enemy02; }
    void SetEnemy02(float e) { player_score.enemy02 = e; }
    float GetEnemy03() { return player_score.enemy03; }
    void SetEnemy03(float e) { player_score.enemy03 = e; }
    float GetEnemy04() { return player_score.enemy04; }
    void SetEnemy04(float e) { player_score.enemy04 = e; }
    float GetEnemy05() { return player_score.enemy05; }
    void SetEnemy05(float e) { player_score.enemy05 = e; }
    float GetTotalScore() { return player_score.total_score; }
    void SetToralScore(float s) { player_score.total_score = s; }

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

    //前方弾発射
    void ProjectileStraightFront(int category,float angle);

    //後方弾発射
    void ProjectileStraightBack(int category, float angle);
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
    DelayTime projectile_front;

    //score
    struct Score
    {
        float enemy01;
        float enemy02;
        float enemy03;
        float enemy04;
        float enemy05;
        float total_score;
    };
    Score player_score;

    ProjectileManager projectileManager;

    Effect*     hitEffect = nullptr;
};