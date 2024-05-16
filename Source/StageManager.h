#pragma once

#include <vector>
#include "Stage.h"

//ステージマネージャー
class StageManager
{
private:
    StageManager(){}
    ~StageManager(){}
public:
    //唯一のインスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }
    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //ステージ登録
    void Register(Stage* stage);

    //ステージ全削除
    void Clear();
    
    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

    ////ステージ数取得
    //int GetStageCount() const { return static_cast<int>(stages.size()); }
    ////ステージ取得
    //Stage* GetStage(int index) { return stages.at(index); }

private:
    std::vector<Stage*> stages;
};