#include "SceneTutorial.h"

//CameraInclude
#include "Camera.h"

//CharacterInclude
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"

//SceneIncldue
#include "SceneManager.h"
#include "SceneLoading.h"
#include "SceneResult.h"

//StageIncldue
#include "StageManager.h"
#include "StageMain.h"
//#include "StageMoveFloor.h"
//#include "StageWall.h"

#include "Input/Input.h"

#define TUTORIAL_DELAYTIME 120

// 初期化
void SceneTutorial::Initialize()
{
	//ステージ初期化
#ifdef ALLSTAGE
	//Main
	StageManager& stageManager = StageManager::Instance();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

#ifdef STAGEMOVE
	for (int index = 0; index < 1; ++index)
	{
		StageMoveFloor* stageMoveFloor = new StageMoveFloor(index);
		stageManager.Register(stageMoveFloor);
	}
#endif // STAGEMOVE

#ifdef STAGEWALL
	for (int index = 0; index < 2; ++index)
	{
		StageWall* stageWall = new StageWall(index);
		stageManager.Register(stageWall);
	}
#endif // STAGEWALL

#endif // ALLSTAGE

#ifdef HPGAUGE
	gauge = new Sprite;
#endif // HPGAUGE

#ifdef  ALLPLAYER
	player = new Player();
#endif //  ALLPLAYER

	game_timer = 0;
	delay_timer = TUTORIAL_DELAYTIME;
	delay_check = false;

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(90),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	//カメラコントローラー初期化
	cameraController = new CameraController();
#ifdef ALLENEMY
#ifdef ENEMYSLIME
#endif // ENEMYSLIME
	

#endif // ALLENEMY
	for (int index = 0; index < 2; index++)
	{
		EnemySlime* slime = new EnemySlime(RED, index);
		EnemyManager::Instance().Register(slime);
	}
}

// 終了化
void SceneTutorial::Finalize()
{
	//エネミー終了化
	EnemyManager::Instance().clear();

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}
	
	StageManager::Instance().Clear();
}

// 更新処理
void SceneTutorial::Update(float elapsedTime)
{
	Mouse& mouse = Input::Instance().GetMouse();
	GamePad& gamePad = Input::Instance().GetGamePad();

	//カメラコントローラー更新処理
#ifdef  ALLPLAYER
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
#endif //  ALLPLAYER
	cameraController->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

#ifdef  ALLPLAYER
	player->Update(elapsedTime);
	if (player->PlayerDead())SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
#endif //  ALLPLAYER


	//エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	EnemyManager::Instance().EnemyMove(player);

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	if (delay_check == true)
	{
		delay_timer--;
	}

	switch (game_timer)
	{
	case 0:
		if (gamePad.GetButtonDown() & (GamePad::BTN_UP | GamePad::BTN_RIGHT | GamePad::BTN_DOWN | GamePad::BTN_LEFT))
		{
			delay_check = true;
			if (delay_timer < 0)
			{
				delay_check = false;
				delay_timer = TUTORIAL_DELAYTIME;
				game_timer++;
			}
		}
	case 1:
		if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			delay_check = true;
			if (delay_timer < 0)
			{
				delay_check = false;
				delay_timer = TUTORIAL_DELAYTIME;
				game_timer++;
			}
		}
	case 2:
		if (mouse.GetButton() & Mouse::BTN_RIGHT)
		{
			delay_check = true;
			if (delay_timer < 0)
			{
				delay_check = false;
				delay_timer = TUTORIAL_DELAYTIME;
				game_timer++;
			}
		}
	case 3:
		if (gamePad.GetButtonDown() & GamePad::BTN_A)
		{
			delay_check = true;
			if (delay_timer < 0)
			{
				delay_check = false;
				delay_timer = TUTORIAL_DELAYTIME;
				game_timer++;
			}
		}
	case 4:
		break;
	};
}

// 描画処理
void SceneTutorial::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.4f, 0.4f, 0.4f, 1.0f };	// RGBA(0.0〜1.0)
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

	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		StageManager::Instance().Render(dc, shader);

		//エネミー描画
		EnemyManager::Instance().Render(dc, shader);

		//プレイヤー描画
#ifdef  ALLPLAYER
		player->Render(dc, shader);
#endif //  ALLPLAYER
		shader->End(dc);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3Dデバッグ描画
	{
#ifdef  DEBUGIMGUI
		//プレイヤーデバッグプリミティブ描画
		player->DrawDebugPrimitive();
		//エネミーデバッグプリミティブ描画
		EnemyManager::Instance().DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
#endif //DEBUGIMGUI
	}

	// 2Dスプライト描画
	{
#ifdef HPGAUGE
		RenderEnemyGauge(dc, rc.view, rc.projection);
		RenderPlayerGauge(dc, rc.view, rc.projection);
#endif // HPGAUGE
#ifdef ENEMYADD
		CrickEnemyAdd(dc, rc.view, rc.projection);
#endif // ENEMYADD
	}

#ifdef DEBUGIMGUI
	player->DrawDebugGUI();
	cameraController->DrawDebugGUI();
	EnemyManager::Instance().DrawDebugGUI();
	StageManager::Instance().DrawDebugGUI();
#endif // DebugImGui
}

//エネミーHPゲージ描画
void SceneTutorial::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//すべての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	DirectX::XMFLOAT4 color = { 1,0,1,1 }; //ゲージの色
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		CharacterGauge(dc, view, projection, enemy->GetPosition(), enemy->GetHealth(), color);
	}
}

//プレイヤーHPゲージ描画
void SceneTutorial::RenderPlayerGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{

	DirectX::XMFLOAT3 player_position = player->GetPosition();
	player_position.y = 1.0f;
	DirectX::XMVECTOR PlayerPosition = DirectX::XMLoadFloat3(&player_position);
	DirectX::XMFLOAT4 color = { 1,0.5,0,1 };//ゲージの色
	CharacterGauge(dc, view, projection, player_position, player->GetHealth(), color);
}

void SceneTutorial::CharacterGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, DirectX::XMFLOAT3 position, float health, DirectX::XMFLOAT4 gaugecolor)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 player_position = player->GetPosition();
	player_position.y = 1.0f;
	DirectX::XMVECTOR PlayerPosition = DirectX::XMLoadFloat3(&player_position);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

	//ワールド座標からスクリーン座標へ変換する関数
	Position = DirectX::XMVector3Project(
		Position,
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
	DirectX::XMStoreFloat3(&position, Position);

	for (int i = 0; i < health; ++i)
	{
		gauge->Render(
			dc,
			position.x - 25 + i * 10, position.y - 70,
			9, 10,
			100, 100,
			25, 10,
			0,
			gaugecolor.x, gaugecolor.y, gaugecolor.z, gaugecolor.w);
		gauge->Render(
			dc,
			position.x - 25 + i * 10, position.y - 70,
			1, 10,
			100, 100,
			25, 10,
			0,
			0, 0, 0, 1);
	}
}

void SceneTutorial::CrickEnemyAdd(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//エネミー配置処理
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouse = Input::Instance().GetMouse();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//マウスカーソル座標取得
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = static_cast<float>(mouse.GetPositionY());
		screenPosition.z = 0;

		DirectX::XMVECTOR ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);

		DirectX::XMVECTOR WorldPosition = DirectX::XMVector3Unproject
		(
			ScreenCursor,
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
		DirectX::XMFLOAT3 world_position_start;
		DirectX::XMStoreFloat3(&world_position_start, WorldPosition);

		//screenPosition.z = 1;
		//ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);
		//WorldPosition = DirectX::XMVector3Unproject
		//(
		//	ScreenCursor,
		//	viewport.TopLeftX,
		//	viewport.TopLeftY,
		//	viewport.Width,
		//	viewport.Height,
		//	viewport.MinDepth,
		//	viewport.MaxDepth,
		//	Projection,
		//	View,
		//	World
		//);
		//DirectX::XMFLOAT3 world_position_end;
		//DirectX::XMStoreFloat3(&world_position_end, WorldPosition);

		//HitResult hit;
		//StageMain stage_main;
		//if (stage_main.RayCast(world_position_start, world_position_end, hit))
		//{
		EnemyManager& enemyManager = EnemyManager::Instance();
		EnemySlime* slime = new EnemySlime(GREEN, 0);
		slime->SetPosition(DirectX::XMFLOAT3(world_position_start.x, world_position_start.y, world_position_start.z));
		enemyManager.Register(slime);
		//}
	}
}
