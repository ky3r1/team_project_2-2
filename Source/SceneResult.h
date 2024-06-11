#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
//#include "Player.h"
#include <cmath>

//タイトルシーン
class SceneResult : public Scene
{
public:
    SceneResult() {}
    ~SceneResult() override {}

    //初期化
    void Initialize() override;

    //終了化
    void Finalize() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

    float easeOutElastic(float x) {
        const float c4 = (2 * 3.14f) / 3;

        if (x == 0) {
            return 0;
        }
        else if (x == 1) {
            return 1;
        }
        else {
            return pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
        }
    }


private:
    Sprite* sprite = nullptr;
    Sprite* score = nullptr;
    Sprite* back_ground = nullptr;
    std::string Score;
    int posX;
    int totalTime;
    int currentTime;
    bool ease;
};