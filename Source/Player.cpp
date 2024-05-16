#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"

Player::Player()
{
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");
}

Player::~Player()
{
    delete hitEffect;

    delete model;
}

void Player::Update(float elapsedTime)
{
    ////進行ベクトル取得
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //
    ////移動処理
    //float moveSpeed = this->moveSpeed * elapsedTime;
    //position.x += moveVec.x * moveSpeed;
    //position.z += moveVec.z * moveSpeed;

    //移動入力処理
    InputMove(elapsedTime);

    ////入力情報の取得
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //float ax = gamePad.GetAxisLX();
    //float ay = gamePad.GetAxisLY();
    //
    ////移動操作
    //float moveSpeed = 5.0f * elapsedTime;
    //{
    //    if (ax <0)
    //    {
    //        position.x -= moveSpeed;
    //    }
    //    if (ax >0)
    //    {
    //        position.x += moveSpeed;
    //    }
    //    if (ay >0)
    //    {
    //        position.z += moveSpeed;
    //    }
    //    if (ay <0)
    //    {
    //        position.z -= moveSpeed;
    //    }
    //}
    //
    //float rotateSpeed = DirectX::XMConvertToRadians(360) * elapsedTime;
    //if (gamePad.GetButton() & GamePad::BTN_A)
    //{
    //    angle.x += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_B)
    //{
    //    angle.y += rotateSpeed;
    //}
    //if (gamePad.GetButton() & GamePad::BTN_X)
    //{
    //    angle.z += rotateSpeed;
    //}


    //ジャンプ処理
    InputJump();

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
}

//void Player::Move(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//    position.x += vx * speed;
//    position.z += vz * speed;
//}

void Player::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

//void Player::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//
//    //進行ベクトルがゼロベクトルの場合は処理する必要なし
//    float length = sqrtf(vx * vx + vz * vz);
//    if (length<0.0001f) return;
//
//    //進行ベクトルを単位ベクトル化
//    vx /= length;
//    vz /= length;
//
//
//    //自身の回転値から前方向を求める
//    float frontX = sinf(angle.y);
//    float frontZ = cosf(angle.y);
//
//    //回転角を求めるため、2つの単位ベクトルの内積を計算する
//    float dot = (vx * frontX) + (vz * frontZ);
//
//    //内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
//    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する
//    float rot = 1.0 - dot;
//    if (rot > speed) { rot = speed; }
//    //speed *= rot;
//    //左右判定を行うために2つの単位ベクトルの外積を計算する
//    float cross = (vz * frontX) - (vx * frontZ);
//
//    //2Dの外積値が正の場合か負の場合によって左右判定が行える
//    //左右判定を行うことによって左右回転を選択する
//
//    if (cross < 0.0f)
//    {
//        angle.y += rot;
//    }
//    else
//    {
//        angle.y -= rot;
//    }
//
//}

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
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position", &position.x);
            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
            ImGui::InputFloat3("Scale", &scale.x);
        }
    }
    ImGui::End();
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::instance();

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
            if (position.y >= (enemy->GetPosition().y + enemy->GetHeight())-0.1f)
            {
                Jump(jumpSpeed);
                enemy->ApplyDamage(1, 0.5f);
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::instance();

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

////ジャンプ処理
//void Player::Jump(float speed)
//{
//    velocity.y = speed;
//}
//
////速力処理更新
//void Player::UpdateVelocity(float elapsedTime)
//{
//    //経過フレーム
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //重力処理
//    velocity.y += gravity * elapsedFrame;
//
//    //移動処理
//    position.y += velocity.y * elapsedTime;
//
//    //地面判定
//    if (position.y < 0.0f)
//    {
//        position.y = 0.0f;
//        velocity.y = 0.0f;
//    }
//}

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

    //直進弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = transform._31 * 100.0f;
        dir.y = 0.0f;
        dir.z = transform._33 * 100.0f;
        //発射位置（プレイヤーの腰当たり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;
        //発射
        ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        projectile->Launch(dir, pos);
        //projectileManager.Register(projectile);
    }
    //追尾弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = transform._31 * 100.0f;
        dir.y = 0.0f;
        dir.z = transform._33 * 100.0f;
        //発射位置（プレイヤーの腰当たり）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //ターゲット（デフォルトではプレイヤーの前方
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //一番近くの敵をターゲットにする
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; i++)
        {
            //敵との距離判定
            Enemy* enemy = EnemyManager::instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;

            }
        }

        //発射
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
}