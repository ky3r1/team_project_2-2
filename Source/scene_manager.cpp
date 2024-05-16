#include "scene_manager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
	// 入れ替えるべき次のシーンがあったら
	if (nextScene != nullptr)
	{
		// 古いシーンの終了処理を行う
		Clear();

		// 新しいシーンを設定
		currentScene = nextScene;
		nextScene = nullptr;

		// 入れ替わった新しい現在のシーンの初期化処理
		if (currentScene->IsReady() == false)
		{
			currentScene->Initialize();
		}
	}

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//描画処理
void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

//シーンクリア
void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

//シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
    nextScene = scene;
}
