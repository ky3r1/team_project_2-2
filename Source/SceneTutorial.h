#pragma once

#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "EnemySlime.h"

// ゲームシーン
class SceneTutorial : public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
private:
	void RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	void RenderPlayerGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	void CharacterGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, DirectX::XMFLOAT3 position, float health, DirectX::XMFLOAT4 gaugecolor);
	void CrickEnemyAdd(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:
	Player* player = nullptr;
	CameraController* cameraController = nullptr;
	Sprite* gauge = nullptr;
	Sprite* sprite = nullptr;
	EnemySlime* slime = nullptr;
	int game_timer = 0;
	bool delay_check = false;
	int delay_timer = 0;
	bool enemyAdd = false;
};