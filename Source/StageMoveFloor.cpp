#include "StageMoveFloor.h"

//�R���X�g���N�^
StageMoveFloor::StageMoveFloor()
{
    scale.x = scale.z = 3.0f;
    scale.y = 0.5f;

    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/Cube/Cube.mdl");
}

//�f�X�g���N�^
StageMoveFloor::~StageMoveFloor()
{
    //�X�e�[�W���f����ǂݍ���
    delete model;
}

//�X�V����
void StageMoveFloor::Update(float elapsedTime)
{
    //�O��̏���ۑ�
    oldTransform = transform;
    oldAngle = angle;

    //�X�^�[�g����S�[���܂ł̋������Z�o
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    //�X�^�[�g����S�[���܂ł̊Ԃ���b�ԂŐi�ފ���(0.0�`1.0)���Z�o����
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;

    //�S�[���ɓ��B�A�܂��̓X�^�[�g�ɖ߂����ꍇ�A�ړ������𔽓]������
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }

    //���`�⊮�ňʒu���Z�o����
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    //��]
    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    //�s��X�V
    UpdateTransform();
#ifdef Unit16_OnCharacter
    //���C�L���X�g�p�Ƀ��f����ԍs��ɂ��邽�߂̒P�ʍs���n��
    const DirectX::XMFLOAT4X4 transformIdentity =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    model->UpdateTransform(transformIdentity);
#else
    //���f���s��X�V
    model->UpdateTransform(transform);
#endif
}

//�`�揈��
void StageMoveFloor::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�\���p�̂��߃��[���h�s��ɍX�V����
    model->UpdateTransform(transform);
    shader->Draw(dc, model);
}

//���C�L���X�g
bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
#ifdef Unit16_OnCharacter
    //�O��̃��[���h�s��̋t�s������߂�
    DirectX::XMMATRIX oldTransform_inverse_m;
    oldTransform_inverse_m = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&oldTransform));

    //�O��̃��[�J����Ԃł̃��C�ɕϊ�
    
    //���[���h���
    DirectX::XMVECTOR world_start_v = DirectX::XMLoadFloat3(&start);//�n�_
    DirectX::XMVECTOR world_end_v = DirectX::XMLoadFloat3(&end);//�I�_

    //���C�̎n�_
    DirectX::XMVECTOR local_start_v = DirectX::XMVector3TransformCoord(world_start_v, oldTransform_inverse_m);
    //���C�̏I�_
    DirectX::XMVECTOR local_end_v = DirectX::XMVector3TransformCoord(world_end_v, oldTransform_inverse_m);

    //���[�J����Ԃł̃��C�Ƃ̌�_�����߂�
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, local_start_v);
    DirectX::XMStoreFloat3(&localEnd, local_end_v);

    

    HitResult localHit;
    if (Collision::IntersectRayVsModel(localStart, localEnd, model, localHit))
    //if(Collision::IntersectRayVsModel(start, end, model, hit))
    {
        //�O��̃��[�J����Ԃ��獡��̃��[���h��Ԃւ̕ϊ�
        //�O�񂩂獡��ɂ����ĕύX���ꂽ���e������Ă���I�u�W�F�N�g�ɔ��f�����B
        DirectX::XMMATRIX transform_m = DirectX::XMLoadFloat4x4(&transform);

        DirectX::XMVECTOR local_character_position_v = DirectX::XMVector3TransformCoord(local_start_v, transform_m);

        //world_character_position_v = DirectX::XMVector3TransformCoord(local_character_position_v, transform_m);

        //DirectX::XMStoreFloat3(&hit.position, world_character_position_v);



        //��]�������Z�o
        //hit.rotation.x = oldAngle.x;
        return true;
    }
    return false;
#else
    return Collision::IntersectRayVsModel(start, end, model, hit);
#endif
}

void StageMoveFloor::DrawDebugGUI()
{
    if (ImGui::TreeNode("StageMoveFloor"))
    {
        ImGui::SliderFloat3("position", &position.x, -5, 5);
        ImGui::SliderFloat3("scale", &scale.x, 0.001f, 4.0f);
        ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
        ImGui::TreePop();
    }
}

