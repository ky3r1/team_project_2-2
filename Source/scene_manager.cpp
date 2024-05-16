#include "scene_manager.h"

//�X�V����
void SceneManager::Update(float elapsedTime)
{
	// ����ւ���ׂ����̃V�[������������
	if (nextScene != nullptr)
	{
		// �Â��V�[���̏I���������s��
		Clear();

		// �V�����V�[����ݒ�
		currentScene = nextScene;
		nextScene = nullptr;

		// ����ւ�����V�������݂̃V�[���̏���������
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

//�`�揈��
void SceneManager::Render()
{
    if (currentScene != nullptr)
    {
        currentScene->Render();
    }
}

//�V�[���N���A
void SceneManager::Clear()
{
    if (currentScene != nullptr)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
    nextScene = scene;
}
