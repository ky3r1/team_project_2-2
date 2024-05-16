#include"ProjectileStraight.h"

//�R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Sword/Sword.mdl");
    //�\���T�C�Y�̒���
    //scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
}
//�f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

//�X�V����
void ProjectileStraight::Update(float elapsedTime)
{
    //��������
    lifeTimer-=elapsedTime;
    if (lifeTimer < 0)
    {
        //�������폜
        Destroy();
    }
    //�ړ�
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;// x �̈ړ������i�ʒu += ���� * �����j
    position.y += direction.y * speed;// y �̈ړ�����
    position.z += direction.z * speed;// z �̈ړ�����

    //�I�u�W�F�N�g�s��X�V
    UpdateTransform();
    //���f���s��X�V
    model->UpdateTransform(transform);
}

//�`�揈��
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);
}

//����
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}
