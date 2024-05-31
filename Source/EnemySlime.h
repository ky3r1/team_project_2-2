#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

//�X���C��
class EnemySlime : public Enemy
{
public:
    EnemySlime(int category,int index);
    ~EnemySlime() override;

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    void MoveEnemy(Player* player)override;
protected:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�f�o�b�O�pGUI
    void DrawDebugGUI() override;

private:
    Model* model = nullptr;
    int elapestime;
};