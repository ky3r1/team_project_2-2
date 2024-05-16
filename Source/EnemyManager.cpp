#include<imgui.h>
#include "EnemyManager.h"
#include"Collision.h"

//fobOpGUI
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

//fobOv~eBu`æ
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies) {
        enemy->DrawDebugPrimitive();
    }
}

//XV
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies) {
        enemy->Update(elapsedTime);
    }
	// jü
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

	// G¯mÌÕË
    CollisionEnemyVsEnemies();
}

//`æ
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies) 
	{
        enemy->Render(context, shader);
    }
}

//Gl~[o^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//Gl~[Sí
void EnemyManager::Clear()
{
    enemies.clear();
}

//Gl~[í
void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}

void EnemyManager::CollisionEnemyVsEnemies()
{
	// GPðJèÔµ¶Åñ·iJE^Í int i = 0j
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// GPðæ¾
		Enemy* enemy1 = GetEnemy(i);

		// GQðJèÔµ¶Åñ·iJE^ int j = i + 1j
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// GQðæ¾
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ðÄÑo·
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),	// GPÌÊu
				enemy1->GetRadius(),	// GPÌ¼a
				enemy1->GetHeight(), // GPÌ³
				enemy2->GetPosition(),	// GQÌÊu
				enemy2->GetRadius(),	// GQÌ¼a
				enemy2->GetHeight(), // GQÌ³
				outPosition				// outPosition
			))
			{
				// ½ÁÄ½ç
				// GQÌÊuÉ outPosition ðÝè
				enemy2->SetPosition(outPosition);
			}
		}
	}
}
