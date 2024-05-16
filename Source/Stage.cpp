#include"stage.h"

static Stage* instance = nullptr;

//インスタンス取得
Stage& Stage::Instance()
{
    return *instance;
}

//コンストラクタ
Stage::Stage()
{
    instance = this;
    //ステージモデルの読み込み
    model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}

//デストラクタ
Stage::~Stage()
{
    //ステージモデルの破棄
    delete model;
}

//更新処理
void Stage::Update(float elapsedTime)
{
    //特にやることなし
}

//描画処理
void Stage::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //シェーダーのモデル描画
    shader->Draw(dc, model);
}

bool Stage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}
