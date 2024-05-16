#include "Camera.h"
#include<imgui.h>

//�w�����������
void Camera::SerLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    //���_�A�����_�A���������r���[�s��
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	//�r���[���t�s�񉻂��A���[���h�s��ɖ߂�
	DirectX::XMMATRIX	World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4	world;
	DirectX::XMStoreFloat4x4(&world, World);
	//�J�����̕��������߂�
	this->right.x	= world._11;
	this->right.y	= world._12;
	this->right.z	= world._13;

	this->up.x		= world._21;
	this->up.y		= world._22;
	this->up.z		= world._23;

	this->front.x	= world._31;
	this->front.y	= world._32;
	this->front.z	= world._33;

	//���_�A�����_��ۑ�
	this->eye = eye;
	this->focus = focus;
}

//�f�o�b�O�pGUI
void Camera::DrawDebugGUI()
{
	//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	//if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None)) {
		//�g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
			//�ʒu    
			ImGui::InputFloat3("Eye", &eye.x);
			//��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(focus.x);
			a.y = DirectX::XMConvertToDegrees(focus.y);
			a.z = DirectX::XMConvertToDegrees(focus.z);
			ImGui::InputFloat3("Focus", &a.x);
			focus.x = DirectX::XMConvertToRadians(a.x);
			focus.y = DirectX::XMConvertToRadians(a.y);
			focus.z = DirectX::XMConvertToRadians(a.z);
		}
	//}
	//ImGui::End();
}

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectivrFov(float fovY, float aspect, float nearZ, float farZ)
{
    //��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}
