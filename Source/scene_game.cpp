#include "Graphics/Graphics.h"
#include "scene_game.h"

#include "camera.h"
#include "enemy_manager.h"
#include "enemy_slime.h"
#include "effect_manager.h"
#include "Input/Input.h"
//#include "Collision.h"

// 初期化
void SceneGame::Initialize()
{
	//ステージ初期化
	stage = new Stage();
	//プレイヤーの初期化
	player = new Player();
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SerLookAt(
		DirectX::XMFLOAT3( 0, 10, -10 )	// カメラの視点（位置）
		,DirectX::XMFLOAT3( 0, 0, 0 )	// カメラの注視点（ターゲット）
		,DirectX::XMFLOAT3( 0, 1, 0 )	// カメラの上方向
	);
	camera.SetPerspectivrFov(
		DirectX::XMConvertToRadians(45)							// 視野角
		, graphics.GetScreenWidth() / graphics.GetScreenHeight()	// 画面縦横比率
		, 0.1f													// カメラが映し出すの最近距離
		, 1000.0f												// カメラが映し出すの最遠距離
	);
	//カメラコントローラーの初期化
	cameraController = new CameraController();
	//エネミー初期化
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 3; i++) 
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i*3.0f, 0, 5));
		enemyManager.Register(slime);
	}
	//ゲージスプライト
	gauge = new Sprite();
}

// 終了化
void SceneGame::Finalize()
{
		//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}
	EnemyManager::Instance().Clear();
	//カメラコントローラー終了化
	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}
	//プレイヤー終了化
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
	//ステージ終了化
	if (stage != nullptr) {
		delete stage;
		stage = nullptr;
	}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//ステージ更新処理
	stage->Update(elapsedTime);
	//プレイヤー更新処理
	player->Update(elapsedTime);
	//エネミー更新処理
	EnemyManager& enemyManager = EnemyManager::Instance();
	enemyManager.Update(elapsedTime);
	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
}

void enemyAdd()
{
	EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(rand()%10 * 3.0f, rand() % 10 * 3.0f, 2.0f));
	EnemyManager::Instance().Register(slime);
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.2f, 0.2f, 0.2f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	////エネミー描画設定
	//EnemyManager& enemyManager = EnemyManager::Instance();
	
	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//ステージ描画
		stage->Render(dc, shader);
		//プレイヤー描画
		player->Render(dc, shader);
		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();
		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2Dスプライト描画
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

		//プレイヤーデバッグ描画
		ImGui::Begin("DebugMenu");

		ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
		if (ImGui::TreeNode("Player")) {
			player->DrawDebugGUI();
			//カメラデバッグ描画
			camera.DrawDebugGUI();
			//カメラコントローラーデバッグ用描画	
			cameraController->DrawDebugGUI();

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Enemy")) {
			//エネミーデバッグ用描画
			EnemyManager::Instance().DrawDebugGUI();
			if (ImGui::Button("Add", ImVec2(60, 25)))
			{
				enemyAdd();
			}
			ImGui::TreePop();
		}

		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}

//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//すべての敵の頭上にHPゲージを表示
	int enemyCount = EnemyManager::Instance().GetEnemyCount();

	for (int i = 0; i < enemyCount;++i)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
		DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//ワールド座標からスクリーン座標へ変換する関数
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			enemyPos,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 ScreenPos_f;
		DirectX::XMStoreFloat3(&ScreenPos_f, ScreenPosition);

		for (int i = 0; i < enemy->GetHealth(); ++i)
		{
			gauge->Render(
				dc,
				ScreenPos_f.x - 25+i*10, ScreenPos_f.y - 70,
				9, 10,
				100, 100,
				25, 10,
				0,
				1, 0, 0, 1);
			gauge->Render(
				dc,
				ScreenPos_f.x - 25+i*10, ScreenPos_f.y - 70,
				1, 10,
				100, 100,
				25, 10,
				0,
				0, 0, 0, 1);
		}

	}
#if false
	//エネミー配置処理
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//マウスカーソル座標取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());

		DirectX::XMVECTOR screenpos_vec = DirectX::XMLoadFloat3(&screenPosition);

		DirectX::XMVECTOR WorldPosition = DirectX::XMVector3Unproject
		(
			screenpos_vec,
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 WorldPosition_f;
		DirectX::XMStoreFloat3(&WorldPosition_f, WorldPosition);

		EnemyManager& enemyManager = EnemyManager::Instance();
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(WorldPosition_f.x, WorldPosition_f.y, WorldPosition_f.z));
		enemyManager.Register(slime);
	}
#endif
}
