#include "StageManager.h"

//更新処理
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}
	//// 破棄処理
	//for (Stage* stage : removes)
	//{
	//	std::vector<Stage*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

	//	if (it != enemies.end())
	//	{
	//		enemies.erase(it);
	//	}

	//	delete enemy;
	//}
}
//描画処理
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(dc, shader);
	}
}
//ステージ登録
void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}
//ステージ全削除
void StageManager::Clear()
{
	stages.clear();
}

//レイキャスト
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	float distance=hit.distance+100;
	for (Stage* stage : stages)
	{
		stage->RayCast(start, end, hit);

		//ヒット情報保存
		if (hit.distance < distance)
		{
			distance = hit.distance;
			result = stage->RayCast(start, end, hit);
		}
	}
	return result;
}