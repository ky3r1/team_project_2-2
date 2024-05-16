#include "StageMoveFloor.h"

//コンストラクタ
StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    //ステージモデルを読み込み
    model = new Model("Data/Model/Cube/Cube.mdl");
}

//デストラクタ
StageMoveFloor::~StageMoveFloor()
{
    //ステージモデルを読み込み
    delete model;
}

//更新処理
void StageMoveFloor::Update(float elapsedTime)
{
    //前回の情報を保存
    oldTransform = transform;
    oldAngle = angle;

    //スタートからゴールまでの距離を算出
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    //スタートからゴールまでの間を一秒間で進む割合(0.0～1.0)を算出する
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //線形補完で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //回転
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    //行列更新
    UpdateTransform();
#ifdef Unit16_OnCharacter
    //レイキャスト用にモデル空間行列にするための単位行列を渡す
    const DirectX::XMFLOAT4X4 transformIdentity =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    model->UpdateTransform(transformIdentity);
#else
    //モデル行列更新
    model->UpdateTransform(transform);
#endif
}

//描画処理
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //表示用のためワールド行列に更新する
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

//レイキャスト
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
#ifdef Unit16_OnCharacter
    //前回のワールド行列の逆行列を求める
    DirectX::XMMATRIX oldTransform_inverse_m;
    oldTransform_inverse_m = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));

    //前回のローカル空間でのレイに変換
    
    //ワールド空間
    DirectX::XMVECTOR world_start_v = DirectX::XMLoadFloat3(&start);//始点
    DirectX::XMVECTOR world_end_v = DirectX::XMLoadFloat3(&end);//終点

    //レイの始点
    DirectX::XMVECTOR local_start_v = DirectX::XMVector3TransformCoord(world_start_v, oldTransform_inverse_m);
    //レイの終点
    DirectX::XMVECTOR local_end_v = DirectX::XMVector3TransformCoord(world_end_v, oldTransform_inverse_m);

    //ローカル空間でのレイとの交点を求める
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, local_start_v);
    DirectX::XMStoreFloat3(&localEnd, local_end_v);

    

    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    //if(Collision::IntersectRayVsModel(start, end, model, hit))
    {
        //前回のローカル空間から今回のワールド空間への変換
        //前回から今回にかけて変更された内容が乗っているオブジェクトに反映される。
        DirectX::XMMATRIX transform_m = DirectX::XMLoadFloat4x4(&transform);

        DirectX::XMVECTOR local_character_position_v = DirectX::XMVector3TransformCoord(local_start_v, transform_m);

        //world_character_position_v = DirectX::XMVector3TransformCoord(local_character_position_v, transform_m);

        //DirectX::XMStoreFloat3(&hit.position, world_character_position_v);



        //回転差分を算出
        //hit.rotation.x = oldAngle.x;
        return true;
    }
    return false;
#else
    return Collision::IntersectRayVsModel(start, end, model, hit);
#endif
}

void StageMoveFloor::DrawDebugGUI()
{
    if (ImGui::TreeNode("StageMoveFloor"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.001f, 4.0f);
        ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}

