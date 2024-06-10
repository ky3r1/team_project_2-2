#include"StageMain.h"

//static Stage* instance = nullptr;

//�R���X�g���N�^
StageMain::StageMain()
{
    //instance = this;
    //�X�e�[�W���f���̓ǂݍ���
    model = new Model("Data/Model/Stage/Stage.mdl");
    scale = { 2.0f,2.0f,2.0f };
    color = { 1,1,1,1 };
    UpdateTransform();
    model->UpdateTransform(transform);
}

//�f�X�g���N�^
StageMain::~StageMain()
{
    //�X�e�[�W���f���̔j��
    delete model;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //���ɂ�邱�ƂȂ�
}

//�`�揈��
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�̃��f���`��
    shader->Draw(dc, model,color);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::DrawDebugGUI()
{

}
