#include <imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"

//更新処理
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //float ax = gamePad.GetAxisRX();
    //float ay = gamePad.GetAxisRY();
    //カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    //スティックの入力値に合わせてX軸とY軸を回転
    //angle.x += speed * ay;
    //angle.y += speed * ax;


    ////X軸のカメラの回転制御
    //if (angle.x > maxAngleX)
    //{
    //    angle.x = maxAngleX;
    //}
    //if (angle.x < minAngleX)
    //{
    //    angle.x = minAngleX;
    //}

    ////Y軸の回転値を-3.14~3.14
    //if (angle.y < -DirectX::XM_PI)
    //{
    //    angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //    angle.y -= DirectX::XM_2PI;
    //}

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range / 1.0f;
    eye.y = target.y - front.y * range / 1.0f;
    eye.z = target.z - front.z * range / 1.0f;

    //カメラの視点と注視点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(310, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat3("target", &target.x, -5, 5);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f * 0.5f);
            ImGui::TreePop();
        }
    }
    ImGui::End();
}