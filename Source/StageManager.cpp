#include "StageManager.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}
}
//�`�揈��
void StageManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(dc, shader);
	}
}
//�X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}
void StageManager::Remove(Stage* stage)
{
	//�j�����X�g�ɒǉ�
	removes.insert(stage);
}

void StageManager::DrawDebugGUI()
{
	ImGui::Begin("Stage");
	ImGui::SetNextTreeNodeOpen(true);
	for (Stage* stage : stages)
	{
		stage->DrawDebugGUI();
	}
	ImGui::End();
}

//�X�e�[�W�S�폜
void StageManager::Clear()
{
	for (Stage* enemy : stages)
	{
		delete enemy;
	}
	stages.clear();
}

//���C�L���X�g
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	float distance = FLT_MAX;
	for (Stage* stage : stages)
	{
		if (!stage->RayCast(start, end, hit)) continue;

		if (hit.distance < distance)
		{
			distance = hit.distance;
			result = true;
		}
	}
	return result;
}