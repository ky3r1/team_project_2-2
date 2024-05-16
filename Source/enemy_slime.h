#pragma once
#include"Graphics/Model.h"
#include"enemy.h"

//�X���C�� 
class EnemySlime :public Enemy {
public:
    EnemySlime();
    ~EnemySlime()override;

    //�X�V����
    void Update(float elapsedTime)override;
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;
private:
    //���S�������ɌĂ΂��
    void OnDead()override;
    Model* model = nullptr;
};