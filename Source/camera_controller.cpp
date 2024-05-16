#include<imgui.h>
#include "camera_controller.h"
#include"camera.h"
#include"Input/Input.h"

//�f�o�b�O�pGUI
void CameraController::DrawDebugGUI()
{
    if (ImGui::CollapsingHeader("CameraController", ImGuiTreeNodeFlags_DefaultOpen)) {
        //�ʒu    
        ImGui::InputFloat3("Target", &target.x);
        //��]
        DirectX::XMFLOAT3 a;
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        ImGui::InputFloat("Range", &range);
    }
}

//�X�V����
void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    //�X�e�C�B�b�N�̓��͂ɍ��킹��X����Y������]
    switch ((int)ax) {
    case -1:    angle.y -= speed;    break;
    case  1:    angle.y += speed;    break;
    default:                         break;
    }
    switch ((int)ay) {
    case -1:    angle.x -= speed;    break;
    case  1:    angle.x += speed;    break;
    default:                         break;
    }
    //X���̃J������]����
    if (angle.x < minAngleX) {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX) {
        angle.x = maxAngleX;
    }

    //Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
    if (angle.y < -DirectX::XM_PI) {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI) {
        angle.y -= DirectX::XM_2PI;
    }

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range / 2.0f;
    eye.y = target.y - front.y * range / 2.0f;
    eye.z = target.z - front.z * range / 2.0f;

    //�J�����̎��_�ƒ����_��ݒ�
    Camera::Instance().SerLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
