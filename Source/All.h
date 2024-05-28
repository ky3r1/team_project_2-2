#pragma once

//インクルード
#include <DirectXMath.h>
#include "Graphics/Model.h"
#include "Graphics/Graphics.h"
#include <vector>
#include <set>
#include <imgui.h>

///////////////////デバッグ用define////////////////////////

//ImGui
#define DEBUGIMGUI

///////////////////Stage////////////////////////

//全ステージ
//#define ALLSTAGE
//動く床
//#define STAGEMOVE
//壁
//#define STAGEWALL

///////////////////Character////////////////////////

//プレイヤー
#define ALLPLAYER
//壁当たり判定
//#define WallCollision
//Jump
//#define JUMPFRAG
//Player->Enemy 弾のダメージ
#define PROJECTILEDAMAGE
//Player->Enemy 踏みつけダメージ
#define JUMPDAMAGE

//Enemy->Player 接触ダメージ
//#define ENEMYHITTINGDAMAGE

//エネミー
//#define ALLENEMY
//スライム
#define ENEMYSLIME

///////////////////UI////////////////////////

//HPゲージ
#define HPGAUGE
//EnemyAdd
//#define ENEMYADD

///////////////////CATEGORY////////////////////////
#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3