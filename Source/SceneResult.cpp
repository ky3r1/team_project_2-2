#include "SceneResult.h"

#include "Graphics/Graphics.h"
#include "Input/Input.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"

float enemy_score[5];
int total_score;

void SceneResult::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/Title.png");
    score = new Sprite("Data/Font/font2.png");
}

void SceneResult::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void SceneResult::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;
    
    if (gamePad.GetButtonDown() & anyButton)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
    }
}

void SceneResult::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.5f,0.0f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    Score = std::to_string(total_score);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, 1920, 1080,
            0, 0, textureWidth, textureHeight,
            0,
            0,0, 0, 1);
        score->textout(dc, "score:", 100, 200, 100, 100, { 1,1,1,1 });
        score->textout(dc, Score, 800, 200, 100, 100, { 1,1,1,1 });
    }
}
