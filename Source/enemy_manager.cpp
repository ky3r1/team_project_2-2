#include<imgui.h>
#include "enemy_manager.h"
#include"collision.h"

//�f�o�b�O�pGUI
void EnemyManager::DrawDebugGUI()
{
    if (ImGui::CollapsingHeader("EnemyManager", ImGuiTreeNodeFlags_DefaultOpen))
	{
        if (ImGui::Button("Clear", ImVec2(60, 25))) 
		{
            EnemyManager::Clear();
        }
    }
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies) {
        enemy->DrawDebugPrimitive();
    }
}

//�X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies) {
        enemy->Update(elapsedTime);
    }
	// �j������
	for (Enemy* enemy : removes)
	{
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		delete enemy;
	}

	removes.clear();

	// �G���m�̏Փˏ���
    CollisionEnemyVsEnemies();
}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies) 
	{
        enemy->Render(context, shader);
    }
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//�G�l�~�[�S�폜
void EnemyManager::Clear()
{
    enemies.clear();
}

//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}

void EnemyManager::CollisionEnemyVsEnemies()
{
	// �G�P���J��Ԃ����ŉ񂷁i�J�E���^�� int i = 0�j
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// �G�P���擾
		Enemy* enemy1 = GetEnemy(i);

		// �G�Q���J��Ԃ����ŉ񂷁i�J�E���^ int j = i + 1�j
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// �G�Q���擾
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ���Ăяo��
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),	// �G�P�̈ʒu
				enemy1->GetRadius(),	// �G�P�̔��a
				enemy1->GetHeight(), // �G�P�̍���
				enemy2->GetPosition(),	// �G�Q�̈ʒu
				enemy2->GetRadius(),	// �G�Q�̔��a
				enemy2->GetHeight(), // �G�Q�̍���
				outPosition				// outPosition
			))
			{
				// �������Ă���
				// �G�Q�̈ʒu�� outPosition ��ݒ�
				enemy2->SetPosition(outPosition);
			}
		}
	}
}
