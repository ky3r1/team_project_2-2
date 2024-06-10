#include "SceneGame.h"

//CameraInclude
#include "Camera.h"

//CharacterInclude
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "MouseManager.h"

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


// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
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
	Mouse_Cursor = new sprite_batch(L"Data\\Sprite\\mousecursor2.png", 1);

#endif // HPGAUGE

#ifdef  ALLPLAYER
	player = new Player();
#endif //  ALLPLAYER

	bgm = Audio::Instance().LoadAudioSource("Data/Audio/Neon_Garden.wav");

	//�J���������ݒ�
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
	//�J�����R���g���[���[������
	cameraController = new CameraController();
#ifdef ALLENEMY
#ifdef ENEMYSLIME
	int index = 0;
	enemy_timer = 0;
	while (index < 3)
	{
		//Blue
		EnemySlime* slime = new EnemySlime(BLUE, index);
		EnemyManager::Instance().Register(slime);
		index++;
	}
	//Red
	while (index < 5)
	{
		//Blue
		EnemySlime* slime = new EnemySlime(RED, index);
		EnemyManager::Instance().Register(slime);
		index++;
	}
#endif // ENEMYSLIME


#endif // ALLENEMY
}

// �I����
void SceneGame::Finalize()
{
	bgm->Stop();

	//�G�l�~�[�I����
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
	delete Mouse_Cursor;
	Mouse_Cursor = nullptr;
	StageManager::Instance().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	bgm->Play(true);

	//�J�����R���g���[���[�X�V����
#ifdef  ALLPLAYER
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
#endif //  ALLPLAYER
	cameraController->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	MouseManager::GetInstance().MouseTransform(dc, Camera::Instance().GetView(), Camera::Instance().GetProjection());

#ifdef  ALLPLAYER
	player->Update(elapsedTime);
	if(player->PlayerDead())SceneManager::Instance().ChangeScene(new SceneLoading(new SceneResult));
#endif //  ALLPLAYER

	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);
	EnemyManager::Instance().EnemyMove(player);
	EnemySlime* slime;
	enemy_timer++;
	switch (enemy_timer)
	{
	case 60:
		slime = new EnemySlime(BLUE, rand() % 8);
		EnemyManager::Instance().Register(slime);
		break;
	case 120:
		slime = new EnemySlime(RED, rand() % 8);
		EnemyManager::Instance().Register(slime);
		break;
	case 180:
		slime = new EnemySlime(GREEN, rand() % 8);
		EnemyManager::Instance().Register(slime);
		break;
	case 240:
		enemy_timer = 0;
		break;
	}
	

	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();


	Mouse& mouse = Input::Instance().GetMouse();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.4f, 0.4f, 0.4f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//�X�e�[�W�`��
		StageManager::Instance().Render(dc, shader);

		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		//�v���C���[�`��
#ifdef  ALLPLAYER
		player->Render(dc, shader);
#endif //  ALLPLAYER
		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
#ifdef  DEBUGIMGUI
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();
		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);
		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
#endif //DEBUGIMGUI
	}

	// 2D�X�v���C�g�`��
	{
#ifdef HPGAUGE
		RenderEnemyGauge(dc, rc.view, rc.projection);
		RenderPlayerGauge(dc, rc.view, rc.projection);
		Mouse_Cursor->begin(graphics.GetDeviceContext(), 0);
		Mouse_Cursor->render(graphics.GetDeviceContext(),
			mouse.GetPositionX() - 50, mouse.GetPositionY() - 50, 100, 100, 1, 1, 1, 1, 0);
		Mouse_Cursor->end(graphics.GetDeviceContext());
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

//�G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//���ׂĂ̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	DirectX::XMFLOAT4 color = { 1,0,1,1 }; //�Q�[�W�̐F
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		CharacterGauge(dc, view, projection, enemy->GetPosition(), enemy->GetHealth(), color);
	}
}

//�v���C���[HP�Q�[�W�`��
void SceneGame::RenderPlayerGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{

	DirectX::XMFLOAT3 player_position = player->GetPosition();
	player_position.y = 1.0f;
	DirectX::XMVECTOR PlayerPosition = DirectX::XMLoadFloat3(&player_position);
	DirectX::XMFLOAT4 color = { 1,0.5,0,1 };//�Q�[�W�̐F
	CharacterGauge(dc, view, projection, player_position, player->GetHealth(), color);
}

void SceneGame::CharacterGauge(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, DirectX::XMFLOAT3 position,float health, DirectX::XMFLOAT4 gaugecolor)
{
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 player_position = player->GetPosition();
	player_position.y = 1.0f;
	DirectX::XMVECTOR PlayerPosition = DirectX::XMLoadFloat3(&player_position);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

	//���[���h���W����X�N���[�����W�֕ϊ�����֐�
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

	player->SetScreenPos(position);

	//for (int i = 0; i < health; ++i)
	//{
	//	gauge->Render(
	//		dc,
	//		position.x - 25 + i * 10, position.y - 70,
	//		9, 10,
	//		100, 100,
	//		25, 10,
	//		0,
	//		gaugecolor.x, gaugecolor.y, gaugecolor.z, gaugecolor.w);
	//	gauge->Render(
	//		dc,
	//		position.x - 25 + i * 10, position.y - 70,
	//		1, 10,
	//		100, 100,
	//		25, 10,
	//		0,
	//		0, 0, 0, 1);
	//}
}

void SceneGame::CrickEnemyAdd(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//�G�l�~�[�z�u����
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W�擾
		DirectX::XMFLOAT3 screenPosition;
		screenPosition.x = static_cast<float>(mouse.GetPositionX());
		screenPosition.y = 1;
		screenPosition.z = static_cast<float>(mouse.GetPositionY());

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
