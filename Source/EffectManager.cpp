#include "Graphics/Graphics.h"
#include "EffectManager.h"

void EffectManager::Initialize()
{
    Graphics& graphics = Graphics::Instance();

    //Effekseerレンダラ生成
    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphics.GetDevice(),
        graphics.GetDeviceContext(), 2048);

    //Effekseerマネージャー生成
    effekseerManager = Effekseer::Manager::Create(2048);

    //Effekseerレンダラの各種設定（特別なカスタマイズをしない場合は定期的に以下の設定でOK）
    effekseerManager->SetSpriteRenderer(effekseerRenderer->CreateSpriteRenderer());
    effekseerManager->SetRibbonRenderer(effekseerRenderer->CreateRibbonRenderer());
    effekseerManager->SetRingRenderer(effekseerRenderer->CreateRingRenderer());
    effekseerManager->SetTrackRenderer(effekseerRenderer->CreateTrackRenderer());
    effekseerManager->SetModelRenderer(effekseerRenderer->CreateModelRenderer());
    //Effekseer内でローダーの設定
    effekseerManager->SetTextureLoader(effekseerRenderer->CreateTextureLoader());
    effekseerManager->SetModelLoader(effekseerRenderer->CreateModelLoader());
    effekseerManager->SetMaterialLoader(effekseerRenderer->CreateMaterialLoader());

    //Effekseerを左手座標系で計算する
    effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

void EffectManager::Finalize()
{
    //EffekseerManagerなどはスマートポインタによって破棄されるので何もしない
}

void EffectManager::Update(float elapsedTime)
{
    effekseerManager->Update(elapsedTime * 60.0f);
}

void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    //ビュープロジェクション行列をEffekseerレンダラに設定
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const
        Effekseer::Matrix44*>(&projection));

    //Effekseer描画開始
    effekseerRenderer->BeginRendering();

    //Effekseer描画実行
    //マネージャー単位で描画するので描画順を制御する場合はマネージャーを複数個作成し、
    //Draw()関数を実行する順序で制御できそう
    effekseerManager->Draw();

    effekseerRenderer->EndRendering();
}
