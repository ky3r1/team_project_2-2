#pragma once

//�C���N���[�h
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Graphics.h"
#include <vector>
#include <set>
#include <imgui.h>

///////////////////�f�o�b�O�pdefine////////////////////////

//ImGui
#define DEBUGIMGUI

///////////////////Stage////////////////////////

//�S�X�e�[�W
#define ALLSTAGE
//������
//#define STAGEMOVE
//��
//#define STAGEWALL

///////////////////Character////////////////////////

//�v���C���[
#define ALLPLAYER
//�Ǔ����蔻��
//#define WallCollision
//Jump
#define JUMPFRAG
//Player->Enemy �e�̃_���[�W
#define PROJECTILEDAMAGE
//Player->Enemy ���݂��_���[�W
#define JUMPDAMAGE

//Enemy->Player �ڐG�_���[�W
#define ENEMYHITTINGDAMAGE

//�G�l�~�[
#define ALLENEMY
//�X���C��
#define ENEMYSLIME

///////////////////UI////////////////////////

//HP�Q�[�W
//#define HPGAUGE
//EnemyAdd
//#define ENEMYADD

///////////////////CATEGORY////////////////////////
#define RED 0       //enemy01
#define GREEN 1     //enemy02
#define BLUE 2      //enemy03
#define YELLOW 3    //enemy04
#define PURPLE 4    //enemy05
#define WHITE 5    //enemy06