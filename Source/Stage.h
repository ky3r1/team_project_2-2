#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"

//�X�e�[�W
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;
    //�`�揈��
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    //���C�L���X�g
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

    //�f�o�b�O�pGUI
    virtual void DrawDebugGUI()=0;

    //position
    void SetPosition(DirectX::XMFLOAT3 p) { position = p; }
    const DirectX::XMFLOAT3 GetPosition() const { return position; }

    //angle
    void SetAngle(DirectX::XMFLOAT3 a) { angle = a; }
    const DirectX::XMFLOAT3 GetAngle() const { return angle; }

    //scale
    void SetScale(DirectX::XMFLOAT3 s) { scale = s; }
    const DirectX::XMFLOAT3 GetScale() const { return scale; }

    //tarnsform
    void SetTransform(DirectX::XMFLOAT4X4 t) { transform = t; }
    const DirectX::XMFLOAT4X4 GetTransform() const { return transform; }

    //�s��X�V����
    void UpdateTransform();
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4 color = { 1,1,1,1 };
    DirectX::XMFLOAT4X4 transform =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    Model* model = nullptr;
};