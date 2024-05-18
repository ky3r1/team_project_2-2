#include "Character.h"
#include "StageManager.h"

//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

//ジャンプ処理
void Character::Jump(float speed)
{
    velocity.y = speed;
}

void Character::Move(float vx, float vz, float speed)
{
    /*speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;*/

    //移動方向ベクトル
    moveVecX = vx;
    moveVecZ = vz;

    //最大速度設定
    maxMoveSpeed = speed;
}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.0001f) return;

    //進行ベクトルを単位ベクトル化
    vx /= length;
    vz /= length;


    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (vx * frontX) + (vz * frontZ);

    //内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する
    float rot = 1.0 - dot;
    if (rot > speed) { rot = speed; }
    //speed *= rot;
    //左右判定を行うために2つの単位ベクトルの外積を計算する
    float cross = (vz * frontX) - (vx * frontZ);

    //2Dの外積値が正の場合か負の場合によって左右判定が行える
    //左右判定を行うことによって左右回転を選択する

    if (cross < 0.0f)
    {
        angle.y += rot;
    }
    else
    {
        angle.y -= rot;
    }

}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    //水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    //水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}

void Character::UpdateInvincibleTime(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

bool Character::ApplyDamage(int damage, float invincibleTime)
{
    //　ダメージが０の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    //死亡している場合は健康状態を変更しない
    if (health <= 0) return false;
    if (invincibleTimer <= 0)
    {
        //ダメージ処理
        health -= damage;
        invincibleTimer = invincibleTime;
        //死亡通知
        if (health <= 0)
        {
            OnDead();
        }
        else
        {
            OnDamaged();
        }

        //健康状態が変更した場合はtrueを返す
        return true;
    }
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //速力に力を加える
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

//垂直速力処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //重力処理
    velocity.y += gravity * elapsedFrame;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my ,position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //地面に接地している

            position = hit.position;
            if (position.y > 0.5)angle = hit.rotation;

            position.y = hit.position.y;


            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0 - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            //接地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    //上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //　XZ平面の速力を減速する
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };

    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elapsedFrame;

        //空中にいるときは摩擦力を減らす
        if (isGround == false) friction *= airControl;

        //摩擦による横方向の減速処理
        if (length > friction)
        {
            //速度を単位ベクトル化
            auto v = DirectX::XMVector2Normalize(velVel);

            //単位ベクトル化した速度を　friction 分スケーリング
            v = DirectX::XMVectorScale(v, friction);

            //スケーリングしたベクトルを速度に減算
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        //横方向の速力が摩擦力以下になったので速力を無効化
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロベクトルでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elapsedFrame;

            if (isGround == false) acceleration *= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;



            //最大速度制限
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    //移動ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
    float velocityLengthXZ = sqrtf(velocity.z * velocity.z + velocity.x * velocity.x);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset, position.z };
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset,position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            // 補正位置の計算
            DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(End, DirectX::XMLoadFloat3(&hit.position));

            DirectX::XMVECTOR R = DirectX::XMVectorAdd(PB, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));

            DirectX::XMVECTOR O = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(DirectX::XMVector3Normalize(Normal), DirectX::XMVectorGetX(Dot)));
            DirectX::XMFLOAT3 o;
            DirectX::XMStoreFloat3(&o, O);

            DirectX::XMFLOAT3 collectPosition = position;
            collectPosition.x = o.x;
            collectPosition.z = o.z;
            HitResult hit2;
            if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                position.x = o.x;
                position.z = o.z;
            }
        }
        else
        {
            //移動
            position.x += mx;
            position.z += mz;
        }
    }
}
