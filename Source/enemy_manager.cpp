#include<imgui.h>
#include "enemy_manager.h"
#include"collision.h"

//ƒfƒoƒbƒO—pGUI
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

//ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies) {
        enemy->DrawDebugPrimitive();
    }
}

//XVˆ—
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies) {
        enemy->Update(elapsedTime);
    }
	// ”jŠüˆ—
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

	// “G“¯m‚ÌÕ“Ëˆ—
    CollisionEnemyVsEnemies();
}

//•`‰æˆ—
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Enemy* enemy : enemies) 
	{
        enemy->Render(context, shader);
    }
}

//ƒGƒlƒ~[“o˜^
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//ƒGƒlƒ~[‘Síœ
void EnemyManager::Clear()
{
    enemies.clear();
}

//ƒGƒlƒ~[íœ
void EnemyManager::Remove(Enemy* enemy)
{
	removes.insert(enemy);
}

void EnemyManager::CollisionEnemyVsEnemies()
{
	// “G‚P‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^‚Í int i = 0j
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		// “G‚P‚ğæ“¾
		Enemy* enemy1 = GetEnemy(i);

		// “G‚Q‚ğŒJ‚è•Ô‚µ•¶‚Å‰ñ‚·iƒJƒEƒ“ƒ^ int j = i + 1j
		for (int j = i + 1; j < enemyCount; ++j)
		{
			// “G‚Q‚ğæ“¾
			Enemy* enemy2 = GetEnemy(j);

			DirectX::XMFLOAT3 outPosition;
			// IntersectSphereVsSphere ‚ğŒÄ‚Ño‚·
			if (Collision::IntersectCylinderVsCylinder(
				enemy1->GetPosition(),	// “G‚P‚ÌˆÊ’u
				enemy1->GetRadius(),	// “G‚P‚Ì”¼Œa
				enemy1->GetHeight(), // “G‚P‚Ì‚‚³
				enemy2->GetPosition(),	// “G‚Q‚ÌˆÊ’u
				enemy2->GetRadius(),	// “G‚Q‚Ì”¼Œa
				enemy2->GetHeight(), // “G‚Q‚Ì‚‚³
				outPosition				// outPosition
			))
			{
				// “–‚½‚Á‚Ä‚½‚ç
				// “G‚Q‚ÌˆÊ’u‚É outPosition ‚ğİ’è
				enemy2->SetPosition(outPosition);
			}
		}
	}
}
