#include "Character.h"
#include "Stage.h"

//�X�V����
void Character::UpdateTransform()
{
    //�X�P�[���s��쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //��]�s��쐬
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    //�ʒu�s��쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3�̍s��̑g�ݍ��킹�A���[���h�s�񐶐�
    DirectX::XMMATRIX W = S * R * T;
    //�v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
}

bool Character::ApplyDamage(int damage,float invincibleTime)
{
    //�_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage <= 0)return false;
    //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health < 0)return false;

    // ���G���Ԓ��̓_���[�W��^���Ȃ�
    if (this->invincibleTimer > 0.0f) return false;
    // ���G���Ԃ��Đݒ�
    this->invincibleTimer = invincibleTime;

    //�_���[�W����
    health = health - damage;

    //���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }
    //���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

//�ړ�����
void Character::Move(float vx, float vz, float speed)
{
    //�ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;
    //�ő呬�x�ݒ�
    maxMoveSpeed = speed;
}
//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    //�i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    float vl = sqrt(vx * vx + vz * vz);
    if (vl < 0.001f)return;
    //�i�s�x�N�g����P�ʃx�N�g����
    vx /= vl;
    vz /= vl;
    //���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);//����
    //���ϒl��-1.0�`1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
    //�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    dot = 1.0f - dot;//�␳
    if (dot > speed) { dot = speed; }
    //���E������s�����߂�2�̒P�ʃx�N�g���̊O�Z���v�Z����
    float cross = (frontZ * vx) - (frontX * vz);//�O��
    //2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    //���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f) angle.y -= dot;
    else angle.y += dot;
}
//jump����
void Character::Jump(float speed)
{
    //������̗͂�ݒ�
    velocity.y = speed;
}
//���͏����X�V
void Character::UpdateVelocity(float elapsedTime)
{
    //�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    //�������͏����X�V
    UpdateVerticalVelocity(elapsedFrame);
    //�������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);

    //�����ړ��X�V����
    UpdateVerticalMove(elapsedTime);
    //�����ړ��X�V�X�V
    UpdateHorizontalMove(elapsedTime);
}

//�������͏����X�V
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    velocity.y += gravity * elapsedFrame;
}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // ���x�̕��ʂł��� XZ �̒������v�Z
    DirectX::XMFLOAT2 vel = { velocity.x, velocity.z };
    DirectX::XMVECTOR velVel = DirectX::XMLoadFloat2(&vel);
    //XZ���ʂ̑��͂���������
    float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(velVel));
    if (length > 0.0f)
    {
        //���C��
        float friction = this->friction * elapsedFrame;
        if (isGround == false) friction *= airControl;

        //���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            // ���x��P�ʃx�N�g����
            auto v = DirectX::XMVector2Normalize(velVel);

            // �P�ʃx�N�g�����������x�� friction ���X�P�[�����O
            v = DirectX::XMVectorScale(v, friction);

            // �X�P�[�����O�����x�N�g���𑬓x�Ɍ��Z
            DirectX::XMStoreFloat2(&vel, DirectX::XMVectorSubtract(velVel, v));

            velocity.x = vel.x;
            velocity.z = vel.y;
        }
        //�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }
    //XZ���͂���������
    if (length <= maxMoveSpeed)
    {
        //�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        float moveVexLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVexLength > 0.0f)
        {
            //�����x
            float acceleration = this->acceleration * elapsedFrame;
            //�󒆂ɂ���Ƃ��͉����͂����炷
            if (isGround == false) acceleration -= airControl;

            //�ړ��x�N�g���ɂ���������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //�ő呬�x���x
            if (length > maxMoveSpeed)
            {
                velocity.x = velocity.x / length * maxMoveSpeed;
                velocity.z = velocity.z / length * maxMoveSpeed;
            }
            //�����ł����������Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                // �Ζʂł̗������x = ���ړ��̒��� * �X�Η� * �o�߃t���[��
                velocity.y -= length * slopeRate * elapsedFrame;
            }

        }
    }
    //�ړ������x�N�g�������Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//�����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
    ////�ړ�����
    //position.y += velocity.y * elapsedTime;
    ////�n�ʔ���
    //if (position.y < 0.0f)
    //{
    //    position.y = 0.0f;
    //    velocity.y = 0.0f;
    //    //���n����
    //    if (!isGround)
    //    {
    //        OnLanding();
    //    }
    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}

    //���������̈ړ���
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //������
    if (my < 0.0f)
    {
        //���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

        //���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //�n�ʂɐڒn���Ă���
            position.y = hit.position.y;
            //���n����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;

            // �X�Η��̌v�Z
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0 - (hit.normal.y / (normalLengthXZ + hit.normal.y));

            velocity.y = 0.0f;
        }
        else
        {
            //�󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    //�㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTiem)
{
    //�������͌v�Z
    float velocityLengthXZ = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velocity)));
    if (velocityLengthXZ > 0.0f)
    {
        //�����ړ��l
        float mx = velocity.x * elapsedTiem;
        float mz = velocity.z * elapsedTiem;

        //���C�̎n�_
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z};
        //���C�̏I�_
        DirectX::XMFLOAT3 end = { position.x+mx,position.y + stepOffset,position.z+mz };

        //���C�L���X�g�ɂ��ǂ̔���
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            //�ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Start, End);

            //�ǂ̖@��
            DirectX::XMVECTOR Normal= DirectX::XMLoadFloat3(&hit.normal);

            //���˃x�N�g����@���Ɏˉe
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            // �␳�ʒu�̌v�Z 
            DirectX::XMVECTOR S = DirectX::XMVectorScale(Normal, DirectX::XMVectorGetX(Dot));
            DirectX::XMFLOAT3 p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));


            position.x = p.x;
            position.z = p.z;
        }
        else
        {
            //�ړ�
            position.x += mx;
            position.z += mz;
        }
    }
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
