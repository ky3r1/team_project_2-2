#include"projectile.h"
#include"projectile_manager.h"
#include"Graphics/Graphics.h"

//コンストラクタ
Projectile::Projectile(ProjectileManager* manager)
    : manager(manager)
{
    manager->Register(this);
}

//デバックプリミティブ描画
void  Projectile::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突用のデバッグ球描画
    debugRenderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

//破棄
void Projectile::Destroy()
{
    manager->Remove(this);
}

//行列更新処理
void  Projectile::UpdateTransform()
{
    //DirectX::XMVECTOR Front, Up, Right;
    //// 前ベクトルを算出
    //Front = DirectX::XMLoadFloat3(&direction);
    //Front = DirectX::XMVector3Normalize(Front);

    //// 仮の上のベクトル算出
    //Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
    //Up = DirectX::XMVector3Normalize(Up);

    //// 右ベクトルを算出
    //Right = DirectX::XMVector3Cross(Up, Front);
    //Right = DirectX::XMVector3Normalize(Right);

    //// 上ベクトルを算出
    //Up = DirectX::XMVector3Cross(Front, Right);

    ////計算結果を取り出し
    //DirectX::XMFLOAT3 right, up, front;
    //DirectX::XMStoreFloat3(&right, Right);
    //DirectX::XMStoreFloat3(&up, Up);
    //DirectX::XMStoreFloat3(&front, Front);
    ////算出した軸ベクトルから行列を作成
    //transform._11 = right.x * scale.x;		// 右方向ベクトルの x（X-x）
    //transform._12 = right.y * scale.x;		// 右方向ベクトルの y（X-y）
    //transform._13 = right.z * scale.x;		// 右方向ベクトルの z（X-z）
    //transform._14 = 0.0f;
    //transform._21 = up.x * scale.y;		// 上方向ベクトルの x（Y-x）
    //transform._22 = up.y * scale.y;		// 上方向ベクトルの y（Y-y）
    //transform._23 = up.z * scale.y;		// 上方向ベクトルの z（Y-z）
    //transform._24 = 0.0f;
    //transform._31 = front.x * scale.z;		// 前方向ベクトルの x（Z-x）
    //transform._32 = front.y * scale.z;		// 前方向ベクトルの y（Z-y）
    //transform._33 = front.z * scale.z;		// 前方向ベクトルの z（Z-z）
    //transform._34 = 0.0f;
    //transform._41 = position.x;	// 位置 x（P-x）
    //transform._42 = position.y;	// 位置 y（P-y）
    //transform._43 = position.z;	// 位置 z（P-z）
    //transform._44 = 1.0f;
    ////発射方向
    //this->direction = front;
}