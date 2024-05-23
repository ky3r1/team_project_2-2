#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"

#include "StageManager.h"
#include "StageMain.h"
#include "StageMoveFloor.h"
#include "StageWall.h"

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
#endif // HPGAUGE

#ifdef  ALLPLAYER
	player = new Player();
#endif //  ALLPLAYER



	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);
	//�J�����R���g���[���[������
	cameraController = new CameraController();
#ifdef ALLENEMY
#ifdef ENEMYSLIME
	for (int index = 0; index < 2; index++)
	{
		EnemySlime* slime = new EnemySlime();
		switch (index)
		{
		case 0:
			slime->SetPosition(DirectX::XMFLOAT3(2.0f, 0, 5));
			break;
		case 1:
			slime->SetPosition(DirectX::XMFLOAT3(4.0f, 0, 5));
			break;
		}
		EnemyManager::Instance().Register(slime);
	}
#endif // ENEMYSLIME


#endif // ALLENEMY
}

// �I����
void SceneGame::Finalize()
{
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

	StageManager::Instance().Clear();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
#ifdef  ALLPLAYER
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
#endif //  ALLPLAYER
	cameraController->Update(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

#ifdef  ALLPLAYER
	player->Update(elapsedTime);
#endif //  ALLPLAYER


	//�G�l�~�[�X�V����
	EnemyManager::Instance().Update(elapsedTime);

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

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
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
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
#ifdef  ALLPLAYER
		player->DrawDebugPrimitive();
#endif //  ALLPLAYER

		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{		
#ifdef HPGAUGE
		RenderEnemyGauge(dc, rc.view, rc.projection);
#endif // HPGAUGE
#ifdef EnemyAdd
		CrickEnemyAdd(dc, rc.view, rc.projection);
#endif // EnemyAdd

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
	//�r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//�ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//���ׂĂ̓G�̓����HP�Q�[�W��\��
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&enemy->GetPosition());

		//���[���h���W����X�N���[�����W�֕ϊ�����֐�
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
		DirectX::XMFLOAT3 screen_pos;
		DirectX::XMStoreFloat3(&screen_pos, ScreenPosition);

		for (int i = 0; i < enemy->GetHealth(); ++i)
		{
			gauge->Render(
				dc,
				screen_pos.x - 25 + i * 10, screen_pos.y - 70,
				9, 10,
				100, 100,
				25, 10,
				0,
				1, 0, 0, 1);
			gauge->Render(
				dc,
				screen_pos.x - 25 + i * 10, screen_pos.y - 70,
				1, 10,
				100, 100,
				25, 10,
				0,
				0, 0, 0, 1);
		}

	}
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
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W�擾
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

		screenPosition.z = 1;
		ScreenCursor = DirectX::XMLoadFloat3(&screenPosition);
		WorldPosition = DirectX::XMVector3Unproject
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
		DirectX::XMFLOAT3 world_position_end;
		DirectX::XMStoreFloat3(&world_position_end, WorldPosition);

		HitResult hit;
		StageMain stage_main;
		if (stage_main.RayCast(world_position_start, world_position_end, hit))
		{
			EnemyManager& enemyManager = EnemyManager::Instance();
			EnemySlime* slime = new EnemySlime();
			slime->SetPosition(DirectX::XMFLOAT3(hit.position.x, hit.position.y, hit.position.z));
			enemyManager.Register(slime);
		}
	}
}
