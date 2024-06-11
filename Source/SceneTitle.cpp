#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Input/Input.h"
#include "SceneResult.h"

void SceneTitle::Initialize()
{
    //スプライト初期化
    sprite = new Sprite("Data/Sprite/tital.png");
    start = new Sprite("Data/Sprite/STRAT.png");
    tutorial = new Sprite("Data/Sprite/tutorial.png");
    mouse_cursor = new Sprite("Data/Sprite/mousecursor1.png");
}

void SceneTitle::Finalize()
{
    //スプライト終了化
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }
    if (start != nullptr)
    {
        delete start;
        start = nullptr;
    }
    if (mouse_cursor != nullptr)
    {
        delete mouse_cursor;
        mouse_cursor = nullptr;
    }
    if (tutorial != nullptr)
    {
        delete tutorial;
        tutorial = nullptr;
    }
}

void SceneTitle::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    GamePadButton anyButton =
          GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;
    if (mouse.GetPositionX() > 300 &&
        mouse.GetPositionX() < 700 &&
        mouse.GetPositionY() > 600 &&
        mouse.GetPositionY() < 721
        )
    {
        if (mouse.GetButton() & mouse.BTN_LEFT)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

        }
    }
    if (mouse.GetPositionX() > 850 &&
        mouse.GetPositionX() < 1340 &&
        mouse.GetPositionY() > 600 &&
        mouse.GetPositionY() < 721
        )
    {
        if (mouse.GetButton() & mouse.BTN_LEFT)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));

        }
    }
    /*if (gamePad.GetButtonDown() & anyButton)
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
    }*/
}

void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };    //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        //タイトルスプライト描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        start->Render(dc,
            300, 600, 400, 121,
            0, 0, 490, 121,
            0,
            1, 1, 1, 1);
        tutorial->Render(dc,
            850, 600, 490, 121,
            0, 0, 709, 121,
            0,
            1, 1, 1, 1);
        mouse_cursor->Render(dc,
            mouse.GetPositionX() - 50, mouse.GetPositionY() - 50, 100, 100,
            0, 0, 100, 100,
            0,
            1, 1, 1, 1);
    }
}
