#pragma once
#include "Graphics/Model.h"
#include "Projectile.h"

//�e�ے��i
class ProjectileStraight : public Projectile
{
public:
    //ProjectileStraight();
    ProjectileStraight(ProjectileManager* manager);
    ~ProjectileStraight() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive()override;
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 3.0f;
};