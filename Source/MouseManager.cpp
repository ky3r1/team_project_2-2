#include "MouseManager.h"

void MouseManager::MouseTransform(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビューポート
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    //変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


    //エネミー配置処理
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    /*if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {*/
    //マウスカーソル座標取得
    DirectX::XMFLOAT3 screenPosition;
    screenPosition.x = static_cast<float>(mouse.GetPositionX());
    screenPosition.y = 0;
    screenPosition.z = static_cast<float>(mouse.GetPositionY());

    DirectX::XMVECTOR ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);

    DirectX::XMVECTOR WorldPosition = DirectX::XMVector3Unproject
    (
        ScreenCursor,
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.Width,
        viewport.Height,
        viewport.MinDepth,
        viewport.MaxDepth,
        Projection,
        View,
        World
    );
    DirectX::XMFLOAT3 world_position_start;
    DirectX::XMStoreFloat3(&world_position_start, WorldPosition);

    screenPosition.z = 1;
    ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);
    WorldPosition = DirectX::XMVector3Unproject
    (
    	ScreenCursor,
    	viewport.TopLeftX,
    	viewport.TopLeftY,
    	viewport.Width,
    	viewport.Height,
    	viewport.MinDepth,
    	viewport.MaxDepth,
    	Projection,
    	View,
    	World
    );
    DirectX::XMFLOAT3 world_position_end;
    DirectX::XMStoreFloat3(&world_position_end, WorldPosition);

    HitResult hit;
    StageMain stage_main;
    if (stage_main.RayCast(world_position_start, world_position_end, hit))
    {
    /*EnemyManager& enemyManager = EnemyManager::Instance();
    EnemySlime* slime = new EnemySlime(GREEN, 0);
    slime->SetPosition(DirectX::XMFLOAT3(world_position_start.x, world_position_start.y, world_position_start.z));
    enemyManager.Register(slime);*/
    }
    //}
}