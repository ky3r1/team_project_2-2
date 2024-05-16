#include "Graphics/Graphics.h"
#include "scene_game.h"

#include "camera.h"
#include "enemy_manager.h"
#include "enemy_slime.h"
#include "effect_manager.h"
#include "Input/Input.h"
//#include "Collision.h"

// ������
void SceneGame::Initialize()
{
	//�X�e�[�W������
	stage = new Stage();
	//�v���C���[�̏�����
	player = new Player();
	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SerLookAt(
		DirectX::XMFLOAT3( 0, 10, -10 )	// �J�����̎��_�i�ʒu�j
		,DirectX::XMFLOAT3( 0, 0, 0 )	// �J�����̒����_�i�^�[�Q�b�g�j
		,DirectX::XMFLOAT3( 0, 1, 0 )	// �J�����̏����
	);
	camera.SetPerspectivrFov(
		DirectX::XMConvertToRadians(45)							// ����p
		, graphics.GetScreenWidth() / graphics.GetScreenHeight()	// ��ʏc���䗦
		, 0.1f													// �J�������f���o���̍ŋߋ���
		, 1000.0f												// �J�������f���o���̍ŉ�����
	);
	//�J�����R���g���[���[�̏�����
	cameraController = new CameraController();
	//�G�l�~�[������
	EnemyManager& enemyManager = EnemyManager::Instance();
	for (int i = 0; i < 3; i++) 
	{
		EnemySlime* slime = new EnemySlime();
		slime->SetPosition(DirectX::XMFLOAT3(i*3.0f, 0, 5));
		enemyManager.Register(slime);
	}
	//�Q�[�W�X�v���C�g
	gauge = new Sprite();
}

// �I����
void SceneGame::Finalize()
{
		//�Q�[�W�X�v���C�g�I����
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}
	EnemyManager::Instance().Clear();
	//�J�����R���g���[���[�I����
	if (cameraController != nullptr) {
		delete cameraController;
		cameraController = nullptr;
	}
	//�v���C���[�I����
	if (player != nullptr) {
		delete player;
		player = nullptr;
	}
	//�X�e�[�W�I����
	if (stage != nullptr) {
		delete stage;
		stage = nullptr;
	}
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	//�X�e�[�W�X�V����
	stage->Update(elapsedTime);
	//�v���C���[�X�V����
	player->Update(elapsedTime);
	//�G�l�~�[�X�V����
	EnemyManager& enemyManager = EnemyManager::Instance();
	enemyManager.Update(elapsedTime);
	//�G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}

void enemyAdd()
{
	EnemySlime* slime = new EnemySlime();
	slime->SetPosition(DirectX::XMFLOAT3(rand()%10 * 3.0f, rand() % 10 * 3.0f, 2.0f));
	EnemyManager::Instance().Register(slime);
}

// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.2f, 0.2f, 0.2f, 1.0f };	// RGBA(0.0�`1.0)
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

	////�G�l�~�[�`��ݒ�
	//EnemyManager& enemyManager = EnemyManager::Instance();
	
	// 3D���f���`��
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);

		//�X�e�[�W�`��
		stage->Render(dc, shader);
		//�v���C���[�`��
		player->Render(dc, shader);
		//�G�l�~�[�`��
		EnemyManager::Instance().Render(dc, shader);

		shader->End(dc);
	}

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 3D�f�o�b�O�`��
	{
		//�v���C���[�f�o�b�O�v���~�e�B�u�`��
		player->DrawDebugPrimitive();
		//�G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		EnemyManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 1.0f));
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

		//�v���C���[�f�o�b�O�`��
		ImGui::Begin("DebugMenu");

		ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
		if (ImGui::TreeNode("Player")) {
			player->DrawDebugGUI();
			//�J�����f�o�b�O�`��
			camera.DrawDebugGUI();
			//�J�����R���g���[���[�f�o�b�O�p�`��	
			cameraController->DrawDebugGUI();

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Enemy")) {
			//�G�l�~�[�f�o�b�O�p�`��
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
	int enemyCount = EnemyManager::Instance().GetEnemyCount();

	for (int i = 0; i < enemyCount;++i)
	{
		Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
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
	//�G�l�~�[�z�u����
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//�}�E�X�J�[�\�����W�擾
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
