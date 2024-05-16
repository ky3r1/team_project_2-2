#include "Camera.h"
#include<imgui.h>

//指定方向を向く
void Camera::SerLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    //視点、中視点、上方向からビュー行列
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	//ビューを逆行列化し、ワールド行列に戻す
	DirectX::XMMATRIX	World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4	world;
	DirectX::XMStoreFloat4x4(&world, World);
	//カメラの方向を取り戻す
	this->right.x	= world._11;
	this->right.y	= world._12;
	this->right.z	= world._13;

	this->up.x		= world._21;
	this->up.y		= world._22;
	this->up.z		= world._23;

	this->front.x	= world._31;
	this->front.y	= world._32;
	this->front.z	= world._33;

	//視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

//デバッグ用GUI
void Camera::DrawDebugGUI()
{
	//ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	//ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	//if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None)) {
		//トランスフォーム
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
			//位置    
			ImGui::InputFloat3("Eye", &eye.x);
			//回転
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

//パースペクティブ設定
void Camera::SetPerspectivrFov(float fovY, float aspect, float nearZ, float farZ)
{
    //画角、画面比率、クリップ距離からプロジェクション行列作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}
