#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//�ړ����X�e�[�W
class StageMoveFloor :public Stage
{
public:
    StageMoveFloor();
    ~StageMoveFloor();

    //�X�V����
    void Update(float elapsedTime)override;
    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
    //�f�o�b�O�pGUI
    void DrawDebugGUI()override;

    //�X�^�[�g�ʒu
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }
    //�S�[���ʒu
    void SetGoalPoint(const DirectX::XMFLOAT3& end) { this->goal = end; }
    //�g���N�ݒ�
    void SetTrque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

private:
    DirectX::XMFLOAT4X4 oldTransform =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    DirectX::XMFLOAT3 torque = { 0,0,0 };
    DirectX::XMFLOAT3 start = { 0,0,0 };
    DirectX::XMFLOAT3 goal = { 0,0,0 };

    float moveSpeed = 2.0f;
    float moveRate = 0.0f;

    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};