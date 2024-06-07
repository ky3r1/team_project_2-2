#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "MouseManager.h"

//TODO:�e��DelayTime
#define DELAYAUTOTIME 40
int a = 0;
#define DELAYALLANGLETIME 60
#define DELAYFRONTTIME 60

//TODO:PlayerDamage�̓����蔻���DelayTime
#define DELAYPLAYERVSENEMY 60

//TODO:Enemy�̃X�R�A
#define ENEMY01_SCORE 10
#define ENEMY02_SCORE 20
#define ENEMY03_SCORE 30
#define ENEMY04_SCORE 40
#define ENEMY05_SCORE 50


extern float enemy_score[5];
extern float total_score;

Player::Player()
{
    //TODO:�v���C���[�̃X�e�[�^�X�ݒ�
    model = new Model("Data/Model/Player/Player.mdl");

    HPbar = new Sprite("Data/Sprite/UI/HPbar.png");
    HP = new Sprite("Data/Sprite/UI/HP.png");
    Arrow = new Sprite("Data/Sprite/UI/sign.png");
    Way3Arrow = new Sprite("Data/Sprite/UI/3way.png");
    AllArrow = new Sprite("Data/Sprite/UI/all_ways.png");

    scale.x = scale.y = scale.z = 0.005f;

    color = { 1,0,0,1 };

    projectile_auto.time = DELAYAUTOTIME;
    projectile_allangle.time = DELAYALLANGLETIME;
    projectile_front.time = DELAYFRONTTIME;

    hit_delay.time = DELAYPLAYERVSENEMY;
    moveSpeed = 5.0f;

    for (int i = 0; i < 5; i++)enemy_score[i] = 0;
    total_score   = 0;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/Hit.efk");
    player_category = WHITE;
    projectile_shot = 0;
}

Player::~Player()
{
    delete hitEffect;
    hitEffect = nullptr;
    delete model;
    model = nullptr;

    delete HP;
    HP = nullptr;
    delete HPbar;
    HPbar = nullptr;
    delete Arrow;
    Arrow = nullptr;
    delete Way3Arrow;
    Way3Arrow = nullptr;
    delete AllArrow;
    AllArrow = nullptr;
    
}

void Player::Update(float elapsedTime)
{
    //�F�ς�
    ChangeColor(color, player_category);

    //�ړ����͏���
    InputMove(elapsedTime);

    //�W�����v����
#ifdef JUMPFRAG
    InputJump();
#endif
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

    //�O���̒e��delay
    if (!projectile_front.checker)
    {
        projectile_front.time--;
    }
    if (projectile_front.time < 0)
    {
        projectile_front.checker = true;
        projectile_front.time = DELAYFRONTTIME;
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

    if (color_count == 0)
    {
        player_category = WHITE;
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
    shader->Draw(dc, model, color);
    Mouse& mouse = Input::Instance().GetMouse();

    //�e�ە`�揈��
    projectileManager.Render(dc, shader);

    //HP
    DirectX::XMFLOAT2 sp_pos = { 0,0 };
    DirectX::XMFLOAT2 sp_endpos = { 100,100 };
    DirectX::XMFLOAT2 sp_size = { 0,0 };
    DirectX::XMFLOAT2 sp_endsize = { 200,100 };
    DirectX::XMFLOAT4 sp_color = { 1,1,1,1 };
    HP->Render(dc, sp_pos, sp_endpos, sp_size, sp_endsize, 0, sp_color);

    //HPbar
    sp_pos = { 100,0 };
    sp_endpos = { 100,100 };
    sp_size = { 0,0 };
    sp_endsize = { 100,100 };
    if (health < 3)
    {
        sp_color = { 1,0,0,1 };
    }
    for (int i = 0; i < health; i++)
    {
        float x = sp_pos.x;
        sp_pos.x += i * 100;
        HPbar->Render(dc, sp_pos, sp_endpos, sp_size, sp_endsize, 0, sp_color);
        sp_pos.x = x;
    }

    //Arrow
    sp_pos = { 50,550 };
    sp_endpos = { 100,100 };
    sp_size = { 0,0 };
    sp_endsize = { 100,100 };
    sp_color = { 1,1,1,1 };
    if (!projectile_auto.checker)
    {
        sp_color = { 1,1,1,0.5 };
    }
    Arrow->Render(dc, sp_pos, sp_endpos, sp_size, sp_endsize, 0, sp_color);

    //Arrow
    sp_pos = { 150,550 };
    sp_endpos = { 100,100 };
    sp_size = { 0,0 };
    sp_endsize = { 100,100 };
    switch (player_category)
    {
    case RED:
        sp_color = { 1, 0, 0, 1 };
        break;
    case GREEN:
        sp_color = { 0, 1, 0, 1 };
        break;
    case BLUE:
        sp_color = { 0, 0, 1, 1 };
        break;
    case YELLOW:
        sp_color = { 1, 1, 0, 1 };
        break;
    case PURPLE:
        sp_color = { 1, 0, 1, 1 };
        break;
    case WHITE:
        sp_color = { 1, 1, 1, 1 };
        break;
    default:
        break;
    }
    if (!projectile_front.checker)
    {
        sp_color.w = 0.5;
    }
    if (projectile_shot == 0)
    {
        Way3Arrow->Render(dc, sp_pos, sp_endpos, sp_size, sp_endsize, 0, sp_color);
    }
    if (projectile_shot == 1)
    {
        AllArrow->Render(dc, sp_pos, sp_endpos, sp_size, sp_endsize, 180, sp_color);
    }
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
            ImGui::SliderInt("delay_front_time", &projectile_front.time, 0.0f, DELAYFRONTTIME);
            ImGui::SliderInt("delay_allangle_time", &projectile_allangle.time, 0.0f, DELAYALLANGLETIME);

            ImGui::SliderFloat("enemy_score_red", &enemy_score[0], 0.0f, 10.0f);
            ImGui::SliderFloat("totoal_socre", &total_score, 0.0f, 10.0f);

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
#ifdef ENEMYHITTINGDAMAGE
            if (hit_delay.checker)
            {
                //������΂�
                {
                    //������΂���
                    const float power = 10.0f;

                    //�G�̈ʒu
                    DirectX::XMVECTOR eVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
                    //�v���C���[�̈ʒu
                    DirectX::XMVECTOR pVec = DirectX::XMLoadFloat3(&position);
                    //�e����G�ւ̕����x�N�g�����v�Z�i�G - �e�j
                    auto v = DirectX::XMVectorSubtract(pVec, eVec);
                    //�����x�N�g���𐳋K��
                    v = DirectX::XMVector3Normalize(v);

                    DirectX::XMFLOAT3 vec;
                    DirectX::XMStoreFloat3(&vec, v);

                    velocity.x += power * vec.x;
                    velocity.y += power * 0.5f;
                    velocity.z += power * vec.z;
                }

                if (player_category == WHITE)
                {
                    player_category = enemy->GetEnemyCategory();
                    color_count = 5;
                }

                if (player_category == enemy->GetEnemyCategory())
                {
                }
                else
                {
                    health--;
                    hitEffect->Play(position,2.0f);
                }
                hit_delay.checker = false;
            }
#endif // ENEMYHITTINGDAMAGE
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
                    color_count--;
                    enemy_score[enemy->GetEnemyCategory()] += 1;
                    switch (enemy->GetEnemyCategory())
                    {
                    case RED:
                        total_score += ENEMY01_SCORE;
                        break;
                    case GREEN:
                        total_score += ENEMY02_SCORE;
                        break;
                    case BLUE:
                        total_score += ENEMY03_SCORE;
                        break;
                    case YELLOW:
                        total_score += ENEMY04_SCORE;
                        break;
                    case PURPLE:
                        total_score += ENEMY05_SCORE;
                        break;
                    default:
                        break;
                    }
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
                            hitEffect->Play(e,2.0f);
                        }
                    }
#endif // PROJECTILEDAMAGE
                }
                else if (projectile->GetProectileCategory() == WHITE)
                {
                    player_category = enemy->GetEnemyCategory();
                    color_count = 5;
                    projectile->Destroy();
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
    switch (player_category)
    {
    case RED:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));
        break;
    case GREEN:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 1, 0, 1));
        break;
    case BLUE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));
        break;
    case YELLOW:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 0, 1));
        break;
    case PURPLE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 0, 1, 1));
        break;
    case WHITE:
        debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 1));
        break;
    default:
        break;
    }

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
        projectile_shot++;
    }
    if (projectile_shot == 2)
    {
        projectile_shot = 0;
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

void Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //�O���e�۔���
    if (mouse.GetButton() & Mouse::BTN_RIGHT)
    {
        if (projectile_auto.checker)
        {
            ProjectileStraightFront(WHITE, 0.0f);
            projectile_auto.checker = false;
        }
    }
    if(player_category!=WHITE)
    {
        if (mouse.GetButton() & Mouse::BTN_LEFT)
        {
            if (projectile_shot == 0)
            {
                if (projectile_front.checker)
                {
                    for (int index = 0; index < 3; index++)
                    {
                        switch (index)
                        {
                        case 0:
                            ProjectileStraightFront(player_category, 0.0f);
                            break;
                        case 1:
                            ProjectileStraightFront(player_category, 0.3f);
                            break;
                        case 2:
                            ProjectileStraightFront(player_category, -0.3f);
                            break;
                        default:
                            break;
                        }
                    }
                    projectile_front.checker = false;
                }
            }
        }
            //���i�e�۔���
        if (mouse.GetButton() & Mouse::BTN_LEFT && projectile_allangle.checker)
        {
            if (projectile_shot == 1)
            {
                for (int index = 0; index < 10; index++)
                {
                    switch (index)
                    {
                    case 0:
                        ProjectileStraightFront(player_category, 0.0f);
                        break;
                    case 1:
                        ProjectileStraightFront(player_category, 0.9);
                        break;
                    case 2:
                        ProjectileStraightFront(player_category, 3.0);
                        break;
                    case 3:
                        ProjectileStraightFront(player_category, -0.9);
                        break;
                    case 4:
                        ProjectileStraightFront(player_category, -3.0);
                        break;
                    case 5:
                        ProjectileStraightBack(player_category, 0.0f);
                        break;
                    case 6:
                        ProjectileStraightBack(player_category, 0.9f);
                        break;
                    case 7:
                        ProjectileStraightBack(player_category, 3.0f);
                        break;
                    case 8:
                        ProjectileStraightBack(player_category, -0.9f);
                        break;
                    case 9:
                        ProjectileStraightBack(player_category, -3.0f);
                        break;
                    default:
                        break;
                    }
                }
                projectile_allangle.checker = false;
            }
        }
    }
}

void Player::ProjectileStraightFront(int category,float angle)//category:�e�̃^�C�v�Aangle:�e�̊p�x
{
    //����
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    DirectX::XMFLOAT3 dis_pos;
    DirectX::XMVECTOR Dis_pos;
    DirectX::XMMATRIX Right_;
    DirectX::XMFLOAT3 r;
    DirectX::XMFLOAT3 axis = { 0,1,0 };
    DirectX::XMVECTOR Axis;

    Axis = DirectX::XMLoadFloat3(&axis);
    dis_pos.x = (MouseManager::GetInstance().GetScreenMousePos().x - screen_pos.x);
    dis_pos.y = 0;
    dis_pos.z = (MouseManager::GetInstance().GetScreenMousePos().z - screen_pos.y);
    Dis_pos = DirectX::XMLoadFloat3(&dis_pos);
    Dis_pos = DirectX::XMVector3Normalize(Dis_pos);
    DirectX::XMStoreFloat3(&dis_pos, Dis_pos);
    Right_ = DirectX::XMMatrixRotationAxis(Axis, DirectX::XMConvertToRadians(90));
    Dis_pos = DirectX::XMVector3Transform(Dis_pos, Right_);
    DirectX::XMStoreFloat3(&r, Dis_pos);

    dir.x = /*transform._31 * 100.0f;*/ -dis_pos.x;
    dir.y = 0.0f;
    dir.z = /*transform._33 * 100.0f;*/ dis_pos.z;
    DirectX::XMFLOAT3 right;
    right.x = /*transform._11 * 100.0f;*/ -r.x;
    right.y = 0.0f;
    right.z = /*transform._13 * 100.0f;*/ r.z;
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

void Player::ProjectileStraightBack(int category, float angle)
{
    ProjectileStraight* projectile{};
    //�O����
    DirectX::XMFLOAT3 dir;
    DirectX::XMFLOAT3 dis_pos;
    DirectX::XMVECTOR Dis_pos;
    DirectX::XMMATRIX Right_;
    DirectX::XMFLOAT3 r;
    DirectX::XMFLOAT3 axis = { 0,1,0 };
    DirectX::XMVECTOR Axis;

    Axis = DirectX::XMLoadFloat3(&axis);
    dis_pos.x = (MouseManager::GetInstance().GetScreenMousePos().x - screen_pos.x);
    dis_pos.y = 0;
    dis_pos.z = (MouseManager::GetInstance().GetScreenMousePos().z - screen_pos.y);
    Dis_pos = DirectX::XMLoadFloat3(&dis_pos);
    Dis_pos = DirectX::XMVector3Normalize(Dis_pos);
    DirectX::XMStoreFloat3(&dis_pos, Dis_pos);
    Right_ = DirectX::XMMatrixRotationAxis(Axis, DirectX::XMConvertToRadians(90));
    Dis_pos = DirectX::XMVector3Transform(Dis_pos, Right_);
    DirectX::XMStoreFloat3(&r, Dis_pos);

    dir.x = /*transform._31 * 100.0f;*/ dis_pos.x;
    dir.y = 0.0f;
    dir.z = /*transform._33 * 100.0f;*/ -dis_pos.z;
    DirectX::XMFLOAT3 right;
    right.x = /*transform._11 * 100.0f;*/ -r.x;
    right.y = 0.0f;
    right.z = /*transform._13 * 100.0f;*/ r.z;
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
