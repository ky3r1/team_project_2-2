#include "Character.h"
#include "Stage.h"

//更新処理
void Character::UpdateTransform()
{
    //スケール行列作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //位置行列作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3つの行列の組み合わせ、ワールド行列生成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

bool Character::ApplyDamage(int damage,float invincibleTime)
{
    //ダメージが0の場合は健康状態を変更する必要がない
    if (damage <= 0)return false;
    //死亡している場合は健康状態を変更しない
    if (health < 0)return false;

    // 無敵時間中はダメージを与えない
    if (this->invincibleTimer > 0.0f) return false;
    // 無敵時間を再設定
    this->invincibleTimer = invincibleTime;

    //ダメージ処理
    health = health - damage;

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

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

//移動処理
void Character::Move(float vx, float vz, float speed)
{
    //移動方向ベクトルを設定
    moveVecX = vx;
    moveVecZ = vz;
    //最大速度設定
    maxMoveSpeed = speed;
}
//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    //進行ベクトルが0ベクトルの場合は処理する必要なし
    float vl = sqrt(vx * vx + vz * vz);
    if (vl < 0.001f)return;
    //進行ベクトルを単位ベクトル化
    vx /= vl;
    vz /= vl;
    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    float dot = (frontX * vx) + (frontZ * vz);//内積
    //内積値は-1.0～1.0で表現されており、2つの単位ベクトルの角度が
    //小さいほど1.0に近づくという性質を利用して回転速度を調整する
    dot = 1.0f - dot;//補正
    if (dot > speed) { dot = speed; }
    //左右判定を行うために2つの単位ベクトルの外算を計算する
    float cross = (frontZ * vx) - (frontX * vz);//外積
    //2Dの外積値が正の場合か負の場合によって左右判定が行える
    //左右判定を行うことによって左右回転を選択する
    if (cross < 0.0f) angle.y -= dot;
    else angle.y += dot;
}
//jump処理
void Character::Jump(float speed)
{
    //上方向の力を設定
    velocity.y = speed;
}
//速力処理更新
void Character::UpdateVelocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力処理更新
    UpdateVerticalVelocity(elapsedFrame);
    //水平水力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);
    //水平移動更新更新
    UpdateHorizontalMove(elapsedTime);
}

//垂直速力処理更新
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    velocity.y += gravity * elapsedFrame;
}

//水平水力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // 速度の平面である XZ の長さを計算
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };
    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    //XZ平面の速力を減速する
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elapsedFrame;
        if (isGround == false) friction *= airControl;

        //摩擦による横方向の減速処理
        if (length > friction)
        {
            // 速度を単位ベクトル化
            auto v = DirectX::XMVector2Normalize(velVel);

            // 単位ベクトルかした速度を friction 分スケーリング
            v = DirectX::XMVectorScale(v, friction);

            // スケーリングしたベクトルを速度に減算
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
    //XZ速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロベクトルでないなら加速する
        float moveVexLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVexLength > 0.0f)
        {
            //加速度
            float acceleration = this->acceleration * elapsedFrame;
            //空中にいるときは加速力を減らす
            if (isGround == false) acceleration -= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //最大速度制度
            if (length > maxMoveSpeed)
            {
                velocity.x = velocity.x / length * maxMoveSpeed;
                velocity.z = velocity.z / length * maxMoveSpeed;
            }
            //下り坂でがたがたしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                // 斜面での落下速度 = 横移動の長さ * 傾斜率 * 経過フレーム
                velocity.y -= length * slopeRate * elapsedFrame;
            }

        }
    }
    //移動方向ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
    ////移動処理
    //position.y += velocity.y * elapsedTime;
    ////地面判定
    //if (position.y < 0.0f)
    //{
    //    position.y = 0.0f;
    //    velocity.y = 0.0f;
    //    //着地した
    //    if (!isGround)
    //    {
    //        OnLanding();
    //    }
    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}

    //垂直方向の移動量
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //落下中
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //地面に接地している
            position.y = hit.position.y;
            //着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;

            // 傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0 - (hit.normal.y / (normalLengthXZ + hit.normal.y));

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

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTiem)
{
    //水平速力計算
    float velocityLengthXZ = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velocity)));
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTiem;
        float mz = velocity.z * elapsedTiem;

        //レイの始点
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z};
        //レイの終点
        DirectX::XMFLOAT3 end = { position.x+mx,position.y + stepOffset,position.z+mz };

        //レイキャストによる壁の判定
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //壁の法線
            DirectX::XMVECTOR Normal= DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            // 補正位置の計算 
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, DirectX::XMVectorGetX(Dot));
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));


            position.x = p.x;
            position.z = p.z;
        }
        else
        {
            //移動
            position.x += mx;
            position.z += mz;
        }
    }
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
