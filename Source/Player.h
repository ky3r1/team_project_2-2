#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Graphics/sprite.h"


class Player : public Character
{
public:
    Player();
    ~Player() override;

    static Player& Instance()
    {
        static Player instance;
        return instance;
    }

    //�X�V����
    void Update(float olapsedTime);

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O�pGUI
    void DrawDebugGUI() override;

    //���S����
    bool PlayerDead();

    // �v���C���[�̎��
    int GetPlayerCategory() { return player_category; }

private:
    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�ړ�����
    //void Move(float elapsedTime, float vx, float vz, float speed);

    //�ړ����͏���
    void InputMove(float elapsedTime);

    //���񏈗�
    //void Turn(float elapsedTime, float vx, float vz, float speed);

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    void CollisionProjectilesVsEnemies();

    //�e�ۓ��͏���
    void InputProjectile();

protected:

    void OnLanding() override;

    //�W�����v���͏���
    void InputJump();

    //�O���e����
    void ProjectileStraightFront(int category,float angle);

    //����e����
    void ProjectileStraightBack(int category, float angle);
private:
    Model* model = nullptr;
    float       moveSpeed = 5.0f;
    float       turnSpeed = DirectX::XMConvertToRadians(720);
    float       jumpSpeed = 20.0f;
    /*float       gravity = -1.0f;
    DirectX::XMFLOAT3 velocity = { 0,0,0 };*/
    int player_category;
    int color_count = 0;
    int projectile_shot;

    int         jumpCount = 0;
    int         jumpLimit = 2;
    DelayTime projectile_allangle;
    DelayTime projectile_auto;
    DelayTime projectile_front;

    ProjectileManager projectileManager;

    Effect*     hitEffect = nullptr;

    Sprite*  HPbar = nullptr;
    Sprite*  HP = nullptr;
    Sprite*  Arrow = nullptr;
    Sprite*  Way3Arrow = nullptr;

    GamePad& gamePad = Input::Instance().GetGamePad();
};