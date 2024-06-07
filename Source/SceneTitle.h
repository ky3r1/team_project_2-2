#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Input/Input.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

private:
    Sprite* sprite = nullptr;
    Sprite* start = nullptr;
    Sprite* tutorial = nullptr;
    Sprite* mouse_cursor = nullptr;
    Mouse& mouse = Input::Instance().GetMouse();

};