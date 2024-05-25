#pragma once
#include "Graphics/Model.h"
#include "Projectile.h"

//�e�ے��i
class ProjectileStraight : public Projectile
{
public:
    //ProjectileStraight();
    ProjectileStraight(ProjectileManager* manager,int category);
    ~ProjectileStraight() override;

    //�X�V����
    virtual void Update(float elapsedTime) override;

    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    //����
    virtual void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugGUI()override;
private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 3.0f;
    DirectX::XMFLOAT3 rotate{};
};