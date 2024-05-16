#include"projectile.h"
#include"projectile_manager.h"
#include"Graphics/Graphics.h"

//�R���X�g���N�^
Projectile::Projectile(ProjectileManager* manager)
    : manager(manager)
{
    manager->Register(this);
}

//�f�o�b�N�v���~�e�B�u�`��
void  Projectile::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˗p�̃f�o�b�O���`��
    debugRenderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�j��
void Projectile::Destroy()
{
    manager->Remove(this);
}

//�s��X�V����
void  Projectile::UpdateTransform()
{
    //DirectX::XMVECTOR Front, Up, Right;
    //// �O�x�N�g�����Z�o
    //Front = DirectX::XMLoadFloat3(&direction);
    //Front = DirectX::XMVector3Normalize(Front);

    //// ���̏�̃x�N�g���Z�o
    //Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
    //Up = DirectX::XMVector3Normalize(Up);

    //// �E�x�N�g�����Z�o
    //Right = DirectX::XMVector3Cross(Up, Front);
    //Right = DirectX::XMVector3Normalize(Right);

    //// ��x�N�g�����Z�o
    //Up = DirectX::XMVector3Cross(Front, Right);

    ////�v�Z���ʂ����o��
    //DirectX::XMFLOAT3 right, up, front;
    //DirectX::XMStoreFloat3(&right, Right);
    //DirectX::XMStoreFloat3(&up, Up);
    //DirectX::XMStoreFloat3(&front, Front);
    ////�Z�o�������x�N�g������s����쐬
    //transform._11 = right.x * scale.x;		// �E�����x�N�g���� x�iX-x�j
    //transform._12 = right.y * scale.x;		// �E�����x�N�g���� y�iX-y�j
    //transform._13 = right.z * scale.x;		// �E�����x�N�g���� z�iX-z�j
    //transform._14 = 0.0f;
    //transform._21 = up.x * scale.y;		// ������x�N�g���� x�iY-x�j
    //transform._22 = up.y * scale.y;		// ������x�N�g���� y�iY-y�j
    //transform._23 = up.z * scale.y;		// ������x�N�g���� z�iY-z�j
    //transform._24 = 0.0f;
    //transform._31 = front.x * scale.z;		// �O�����x�N�g���� x�iZ-x�j
    //transform._32 = front.y * scale.z;		// �O�����x�N�g���� y�iZ-y�j
    //transform._33 = front.z * scale.z;		// �O�����x�N�g���� z�iZ-z�j
    //transform._34 = 0.0f;
    //transform._41 = position.x;	// �ʒu x�iP-x�j
    //transform._42 = position.y;	// �ʒu y�iP-y�j
    //transform._43 = position.z;	// �ʒu z�iP-z�j
    //transform._44 = 1.0f;
    ////���˕���
    //this->direction = front;
}