#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
//#include "Player.h"
#include <cmath>

//�^�C�g���V�[��
class SceneResult : public Scene
{
public:
    SceneResult() {}
    ~SceneResult() override {}

    //������
    void Initialize() override;

    //�I����
    void Finalize() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
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