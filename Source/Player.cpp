#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"

//TODO:弾のDelayTime
#define DELAYAUTOTIME 40
int a = 0;
#define DELAYALLANGLETIME 60
#define DELAYFRONTTIME 60

//TODO:PlayerDamageの当たり判定のDelayTime
#define DELAYPLAYERVSENEMY 60

//TODO:Enemyのスコア
#define ENEMY01_SCORE 10
#define ENEMY02_SCORE 20
#define ENEMY03_SCORE 30
#define ENEMY04_SCORE 40
#define ENEMY05_SCORE 50


extern float enemy_score[5];
extern float total_score;

Player::Player()
{
    //TODO:プレイヤーのステータス設定
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    projectile_auto.time = DELAYAUTOTIME;
    projectile_allangle.time = DELAYALLANGLETIME;
    projectile_front.time = DELAYFRONTTIME;

    hit_delay.time = DELAYPLAYERVSENEMY;
    moveSpeed = 5.0f;

    for (int i = 0; i < 5; i++)enemy_score[i] = 0;
    total_score   = 0;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/sample_01.efk");
}

Player::~Player()
{
    delete hitEffect;
    hitEffect = nullptr;
    delete model;
    model = nullptr;
}

void Player::Update(float elapsedTime)
{
    //移動入力処理
    InputMove(elapsedTime);

    //ジャンプ処理
#ifdef JUMPFRAG
    InputJump();
#endif
    //弾丸入力処理
    InputProjectile();

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

    //弾丸と敵の衝突処理
    CollisionProjectilesVsEnemies();

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

    //当たり判定のdelay
    if (!hit_delay.checker)
    {
        hit_delay.time--;
    }
    if (hit_delay.time < 0)
    {
        hit_delay.checker = true;
        hit_delay.time = DELAYAUTOTIME;
    }

    //オートで出てる弾のdelay
    if (!projectile_auto.checker)
    {
        projectile_auto.time--;
    }
    if (projectile_auto.time < 0)
    {
        projectile_auto.checker = true;
        projectile_auto.time = DELAYAUTOTIME;
    }

    //前方の弾のdelay
    if (!projectile_front.checker)
    {
        projectile_front.time--;
    }
    if (projectile_front.time < 0)
    {
        projectile_front.checker = true;
        projectile_front.time = DELAYFRONTTIME;
    }

    //周囲に出す弾のdelay
    if (!projectile_allangle.checker)
    {
        projectile_allangle.time--;
    }
    if (projectile_allangle.time < 0)
    {
        projectile_allangle.checker = true;
        projectile_allangle.time = DELAYALLANGLETIME;
    }
}

void Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    //弾丸描画処理
    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::SliderFloat3("position", &position.x, -5, 5);
            ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
            ImGui::SliderInt("health", &health, 0.0f, 10.0f);
            ImGui::SliderFloat("movespeed", &moveSpeed, 0.0f, 10.0f);

            ImGui::SliderInt("delay_auto_time", &projectile_auto.time, 0.0f, DELAYAUTOTIME);
            ImGui::SliderInt("delay_front_time", &projectile_front.time, 0.0f, DELAYFRONTTIME);
            ImGui::SliderInt("delay_allangle_time", &projectile_allangle.time, 0.0f, DELAYALLANGLETIME);

            ImGui::SliderFloat("enemy_score_red", &enemy_score[0], 0.0f, 10.0f);
            ImGui::SliderFloat("totoal_socre", &total_score, 0.0f, 10.0f);

            ImGui::TreePop();
        }
    }
    ImGui::End();

    projectileManager.DrawDebugGUI();
}

bool Player::PlayerDead()
{
    if (health <= 0)return true;
    return false;
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵と総当たりで衝突判定
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsSphere(
            position, radius,height,
            enemy->GetPosition(), enemy->GetRadius(),enemy->GetHeight(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
#ifdef ENEMYHITTINGDAMAGE
            if (hit_delay.checker)
            {
                //吹き飛ばす
                {
                    //吹き飛ばす力
                    const float power = 10.0f;

                    //敵の位置
                    DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                    //プレイヤーの位置
                    DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&position);
                    //弾から敵への方向ベクトルを計算（敵 - 弾）
                    auto v = DirectX::XMVectorSubtract(pVec, eVec);
                    //方向ベクトルを正規化
                    v = DirectX::XMVector3Normalize(v);

                    DirectX::XMFLOAT3 vec;
                    DirectX::XMStoreFloat3(&vec, v);

                    velocity.x += power * vec.x;
                    velocity.y += power * 0.5f;
                    velocity.z += power * vec.z;
                }

                health--;
                hit_delay.checker = false;
                //ヒットエフェクト再生
                {
                    hitEffect->Play(position);
                }
            }
#endif // ENEMYHITTINGDAMAGE
            if (position.y >= (enemy->GetPosition().y + enemy->GetHeight())-0.1f)
            {
                Jump(jumpSpeed);
#ifdef JUMPDAMAGE
                enemy->ApplyDamage(1, 0.5f);
#endif//JUMPDAMAGE
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての弾丸とすべての敵をそう当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                if (projectile->GetProectileCategory() == enemy->GetEnemyCategory())
                {
                    enemy_score[enemy->GetEnemyCategory()] += 1;
                    switch (enemy->GetEnemyCategory())
                    {
                    case RED:
                        total_score += ENEMY01_SCORE;
                        break;
                    case GREEN:
                        total_score += ENEMY02_SCORE;
                        break;
                    case BLUE:
                        total_score += ENEMY03_SCORE;
                        break;
                    case YELLOW:
                        total_score += ENEMY04_SCORE;
                        break;
                    case PURPLE:
                        total_score += ENEMY05_SCORE;
                        break;
                    default:
                        break;
                    }
                    //弾丸破棄
                    projectile->Destroy();
#ifdef PROJECTILEDAMAGE
                    //ダメージを与える
                    if (enemy->ApplyDamage(1, 0.5f))
                    {
                        //吹き飛ばす
                        {
                            DirectX::XMFLOAT3 impulse;
                            //吹き飛ばす力
                            const float power = 10.0f;

                            //敵の位置
                            DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                            //弾の位置
                            DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                            //弾から敵への方向ベクトルを計算（敵 - 弾）
                            auto v = DirectX::XMVectorSubtract(eVec, pVec);
                            //方向ベクトルを正規化
                            v = DirectX::XMVector3Normalize(v);

                            DirectX::XMFLOAT3 vec;
                            DirectX::XMStoreFloat3(&vec, v);

                            impulse.x = power * vec.x;
                            impulse.y = power * 0.5f;
                            impulse.z = power * vec.z;

                            enemy->AddImpulse(impulse);
                        }

                        //ヒットエフェクト再生
                        {
                            DirectX::XMFLOAT3 e = enemy->GetPosition();
                            e.y += enemy->GetHeight() * 0.5f;
                            hitEffect->Play(e);
                        }
                    }
#endif // PROJECTILEDAMAGE
                }
                else
                {
                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //弾丸デバッグプリミティブ描画
    projectileManager.DrawDebugPrimitive();

}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //入力情報の取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //カメラ方向とスティックの入力によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXY平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /=  cameraRightLength;
        cameraRightZ /=  cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラの右方向に反映し、
    //スティックの垂直入力値をカメラの前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX*ax) + (cameraFrontX*ay);
    vec.z = (cameraRightZ*ax) + (cameraFrontZ*ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

void Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount == jumpLimit)
        {
            return;
        }
        jumpCount++;
        Jump(jumpSpeed);
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    //直進弾丸発射
    if (mouse.GetButton() & Mouse::BTN_RIGHT &&projectile_allangle.checker)
    {
        for (int index = 0; index < 10; index++)
        {
            switch (index)
            {
            case 0:
                ProjectileStraightFront(BLUE, 0.0f);
                break;
            case 1:
                ProjectileStraightFront(BLUE, 0.9);
                break;
            case 2:
                ProjectileStraightFront(BLUE, 3.0);
                break;
            case 3:
                ProjectileStraightFront(BLUE, -0.9);
                break;
            case 4:
                ProjectileStraightFront(BLUE, -3.0);
                break;
            case 5:
                ProjectileStraightBack(BLUE, 0.0f);
                break;
            case 6:
                ProjectileStraightBack(BLUE, 0.9f);
                break;
            case 7:
                ProjectileStraightBack(BLUE, 3.0f);
                break;
            case 8:
                ProjectileStraightBack(BLUE, -0.9f);
                break;
            case 9:
                ProjectileStraightBack(BLUE, -3.0f);
                break;
            default:
                break;
            }
        }
        projectile_allangle.checker = false;
    }
    
    //前方弾丸発射
    if (mouse.GetButton() & Mouse::BTN_LEFT)
    {
        if (projectile_front.checker)
        {
            ProjectileStraightFront(GREEN, 0.0f);
            projectile_front.checker = false;
        }
    }
    if (projectile_auto.checker)
    {
        for (int index = 0; index < 3; index++)
        {
            switch (index)
            {
            case 0:
                ProjectileStraightFront(RED,0.0f);
                break;
            case 1:
                ProjectileStraightFront(RED,0.3f);
                break;
            case 2:
                ProjectileStraightFront(RED,-0.3f);
                break;
            default:
                break;
            }
        }
        projectile_auto.checker = false;
    }
}

void Player::ProjectileStraightFront(int category,float angle)//category:弾のタイプ、angle:弾の角度
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    dir.x = transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = transform._33 * 100.0f;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f;
    //発射位置（プレイヤーの腰当たり）
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, angle);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    projectile = new ProjectileStraight(&projectileManager, category);
    projectile->Launch(dir, pos);
}

void Player::ProjectileStraightBack(int category, float angle)
{
    //発射
    ProjectileStraight* projectile{};
    //前方向
    DirectX::XMFLOAT3 dir;
    dir.x = -transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = -transform._33 * 100.0f;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f;
    //発射位置（プレイヤーの腰当たり）
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, angle);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    projectile = new ProjectileStraight(&projectileManager, category);
    projectile->Launch(dir, pos);
}
