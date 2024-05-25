#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"

#define DELAYAUTOTIME 40
#define DELAYALLANGLETIME 60

#define DELAYPLAYERVSENEMY 60

Player::Player()
{
    //TODO:�v���C���[�̃X�e�[�^�X�ݒ�
    model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");

    scale.x = scale.y = scale.z = 0.01f;

    projectile_auto.time = DELAYAUTOTIME;
    projectile_allangle.time = DELAYALLANGLETIME;
    hit_delay.time = DELAYPLAYERVSENEMY;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/Hit.efk");
}

Player::~Player()
{
    delete hitEffect;
    hitEffect = nullptr;
    delete model;
    model = nullptr;
}

void Player::Update(float elapsedTime)
{
    //�ړ����͏���
    InputMove(elapsedTime);

    //�W�����v����
    InputJump();

    //�e�ۓ��͏���
    InputProjectile();

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    //�v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();

    //�e�ۂƓG�̏Փˏ���
    CollisionProjectilesVsEnemies();

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);

    //�����蔻���delay
    if (!hit_delay.checker)
    {
        hit_delay.time--;
    }
    if (hit_delay.time < 0)
    {
        hit_delay.checker = true;
        hit_delay.time = DELAYAUTOTIME;
    }

    //�I�[�g�ŏo�Ă�e��delay
    if (!projectile_auto.checker)
    {
        projectile_auto.time--;
    }
    if (projectile_auto.time < 0)
    {
        projectile_auto.checker = true;
        projectile_auto.time = DELAYAUTOTIME;
    }

    //���͂ɏo���e��delay
    if (!projectile_allangle.checker)
    {
        projectile_allangle.time--;
    }
    if (projectile_allangle.time < 0)
    {
        projectile_allangle.checker = true;
        projectile_allangle.time = DELAYALLANGLETIME;
    }
}

void Player::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    shader->Draw(dc, model);

    //�e�ە`�揈��
    projectileManager.Render(dc, shader);
}

void Player::DrawDebugGUI()
{

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        if (ImGui::TreeNode("Transform"))
        {
            ImGui::SliderFloat3("position", &position.x, -5, 5);
            ImGui::SliderFloat3("scale", &scale.x, 0.01f, 4.0f);
            ImGui::SliderFloat3("angle", &angle.x, -3.14f, 3.14f);
            ImGui::SliderInt("health", &health, 0.0f, 10.0f);
            ImGui::SliderFloat("movespeed", &moveSpeed, 0.0f, 10.0f);

            ImGui::SliderInt("delay_auto_time", &projectile_auto.time, 0.0f, DELAYAUTOTIME);
            ImGui::SliderInt("delay_allangle_time", &projectile_allangle.time, 0.0f, DELAYALLANGLETIME);

            ImGui::TreePop();
        }
    }
    ImGui::End();

    projectileManager.DrawDebugGUI();
}

bool Player::PlayerDead()
{
    if (health <= 0)return true;
    return false;
}

void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƒ�������ŏՓ˔���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsSphere(
            position, radius,height,
            enemy->GetPosition(), enemy->GetRadius(),enemy->GetHeight(),
            outPosition))
        {
            enemy->SetPosition(outPosition);
            if (hit_delay.checker)
            {
                health--;
                hit_delay.checker = false;
                //�q�b�g�G�t�F�N�g�Đ�
                {
                    hitEffect->Play(position);
                }
            }
            if (position.y >= (enemy->GetPosition().y + enemy->GetHeight())-0.1f)
            {
                Jump(jumpSpeed);
#ifdef JUMPDAMAGE
                enemy->ApplyDamage(1, 0.5f);
#endif//JUMPDAMAGE
            }
        }
    }
}

void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̒e�ۂƂ��ׂĂ̓G������������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                if (projectile->GetProectileCategory() == enemy->GetEnemyCategory())
                {
                    //�e�۔j��
                    projectile->Destroy();
#ifdef PROJECTILEDAMAGE
                    //�_���[�W��^����
                    if (enemy->ApplyDamage(1, 0.5f))
                    {
                        //������΂�
                        {
                            DirectX::XMFLOAT3 impulse;
                            //������΂���
                            const float power = 10.0f;

                            //�G�̈ʒu
                            DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                            //�e�̈ʒu
                            DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&projectile->GetPosition());
                            //�e����G�ւ̕����x�N�g�����v�Z�i�G - �e�j
                            auto v = DirectX::XMVectorSubtract(eVec, pVec);
                            //�����x�N�g���𐳋K��
                            v = DirectX::XMVector3Normalize(v);

                            DirectX::XMFLOAT3 vec;
                            DirectX::XMStoreFloat3(&vec, v);

                            impulse.x = power * vec.x;
                            impulse.y = power * 0.5f;
                            impulse.z = power * vec.z;

                            enemy->AddImpulse(impulse);
                        }

                        //�q�b�g�G�t�F�N�g�Đ�
                        {
                            DirectX::XMFLOAT3 e = enemy->GetPosition();
                            e.y += enemy->GetHeight() * 0.5f;
                            hitEffect->Play(e);
                        }
                    }
#endif // PROJECTILEDAMAGE
                }
                else
                {
                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}

//�f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    ////�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();

}

DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    //���͏��̎擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    //�J���������ƃX�e�B�b�N�̓��͂ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XY���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /=  cameraRightLength;
        cameraRightZ /=  cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�X�e�B�b�N�̐������͒l���J�����̉E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����̑O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX*ax) + (cameraFrontX*ay);
    vec.z = (cameraRightZ*ax) + (cameraFrontZ*ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

void Player::InputJump()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        if (jumpCount == jumpLimit)
        {
            return;
        }
        jumpCount++;
        Jump(jumpSpeed);
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //���i�e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X&&projectile_allangle.checker)
    {
        for (int index = 0; index < 10; index++)
        {
            switch (index)
            {
            case 0:
                ProjectileStraightWay(BLUE, 0.0f);
                break;
            case 1:
                ProjectileStraightWay(BLUE, 3.14f);
                break;
                //case 2:
                //    ProjectileStraightWay(BLUE, 12.0f);
                //    break;
                    //case 3:
                    //    ProjectileStraightWay(BLUE, 0.6f);
                    //    break;
                    //case 4:
                    //    ProjectileStraightWay(BLUE, 0.8f);
                    //    break;
                    //case 5:
                    //    ProjectileStraightWay(BLUE, 10.0f);
                    //    break;
                    //case 6:
                    //    ProjectileStraightWay(BLUE, -0.8f);
                    //    break;
                    //case 7:
                    //    ProjectileStraightWay(BLUE, -0.6f);
                    //    break;
                    //case 8:
                    //    ProjectileStraightWay(BLUE, -0.4f);
                    //    break;
                    //case 9:
                    //    ProjectileStraightWay(BLUE, -0.2f);
                    //    break;
            default:
                break;
            }
        }
        projectile_allangle.checker = false;
    }
    
    //�ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = transform._31 * 100.0f;
        dir.y = 0.0f;
        dir.z = transform._33 * 100.0f;
        //���ˈʒu�i�v���C���[�̍�������j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O��
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; i++)
        {
            //�G�Ƃ̋�������
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }

        //����
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager, GREEN);
        projectile->Launch(dir, pos, target);
    }
    if (projectile_auto.checker)
    {
        for (int index = 0; index < 3; index++)
        {
            switch (index)
            {
            case 0:
                ProjectileStraightWay(RED,0.0f);
                break;
            case 1:
                ProjectileStraightWay(RED,0.3f);
                break;
            case 2:
                ProjectileStraightWay(RED,-0.3f);
                break;
            default:
                break;
            }
        }
        projectile_auto.checker = false;
    }
}

void Player::ProjectileStraightWay(int category,float angle)//category:�e�̃^�C�v�Aangle:�e�̊p�x
{
    //����
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    dir.x = transform._31 * 100.0f;
    dir.y = 0.0f;
    dir.z = transform._33 * 100.0f;
    DirectX::XMFLOAT3 right;
    right.x = transform._11 * 100.0f;
    right.y = 0.0f;
    right.z = transform._13 * 100.0f;
    //���ˈʒu�i�v���C���[�̍�������j
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    DirectX::XMVECTOR Right = DirectX::XMLoadFloat3(&right);
    Right = DirectX::XMVectorScale(Right, angle);
    DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
    DirectX::XMVECTOR Ev = DirectX::XMVectorAdd(Dir, Right);
    DirectX::XMVECTOR Ep = DirectX::XMVectorAdd(Pos, Ev);
    Ep = DirectX::XMVectorSubtract(Ep, Pos);
    DirectX::XMFLOAT3 ep;
    DirectX::XMStoreFloat3(&ep, Ep);
    dir.x = ep.x;
    dir.y = 0.0f;
    dir.z = ep.z;
    projectile = new ProjectileStraight(&projectileManager, category);
    projectile->Launch(dir, pos);
}