#include"StageMain.h"

//static Stage* instance = nullptr;

//コンストラクタ
StageMain::StageMain()
{
    //instance = this;
    //ステージモデルの読み込み
    model = new Model("Data/Model/Stage/Stage.mdl");
    scale = { 2.0f,2.0f,2.0f };
    color = { 1,1,1,1 };
    UpdateTransform();
    model->UpdateTransform(transform);
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルの破棄
    delete model;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //特にやることなし
}

//描画処理
void StageMain::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーのモデル描画
    shader->Draw(dc, model,color);
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageMain::DrawDebugGUI()
{

}
