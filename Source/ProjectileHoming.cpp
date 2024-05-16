#include"ProjectileHoming.h"

//�R���X�g���N�^
ProjectileHoming::ProjectileHoming(ProjectileManager* manager):Projectile(manager)
{
    model = new Model("Data/Model/Sword/Sword.mdl");
    //�X�P�[�����O
    scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
ProjectileHoming::~ProjectileHoming()
{
    delete model;
}

//�X�V����
void ProjectileHoming::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer < 0)
    {
        //�������폜
        Destroy();
    }
    //�ړ�
    {
        float moveSpeed = this->moveSpeed * elapsedTime;
        position.x += direction.x * moveSpeed;// x �̈ړ������i�ʒu += ���� * �����j
        position.y += direction.y * moveSpeed;// y �̈ړ�����
        position.z += direction.z * moveSpeed;// z �̈ړ�����
    }
    //����
    {
        float turnSpeed = this->turnSpeed * elapsedTime;
        //�^�[�Q�b�g�܂ł̃x�N�g���Z�o
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);	// position �� XMVECTOR �ɕϊ�
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target); // target �� XMVECTOR �ɕϊ�
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position); // target - position

        //�[���x�N�g���Ŗ����Ȃ��]����
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        if (lengthSq > 0.00001f)
        {
            //�^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);
            //�����Ă�������x�N�g�����Z�o
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            //�O�������x�N�g���̃^�[�Q�b�g�܂ł̃x�N�g������(�p�x)���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Direction);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);
            //2�̒P�ʃx�N�g���̊p�x���������ق�
            //1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���𒲐�����
            float rot = 1.0f - dot;
            if (rot > DirectX::XMConvertToRadians(180))
            {
                rot = DirectX::XMConvertToRadians(180);
            }
            // ��]�p�x������Ȃ��]����������
            if (fabsf(rot) > 0.0001f)
            {
                // ��]�����v�Z
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);

                // ��]���Ɖ�]�ʂ����]�s����v�Z
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);

                // ���݂̍s�����]������
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation);

                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);

            }
        }
    }
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();
    //���f���s��X�V
    model->UpdateTransform(transform);

}

//�`�揈��
void ProjectileHoming::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//����
void ProjectileHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target)
{
    this->direction = direction;
    this->position = position;
    this->target = target;
    UpdateTransform();
}
