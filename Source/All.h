#pragma once

//インクルード
#include<DirectXMath.h>
#include "Graphics/Model.h"
#include <vector>
#include <set>
#include <imgui.h>

///////////////////デバッグ用define////////////////////////

//ImGui
#define DEBUGIMGUI

///////////////////Stage////////////////////////

//全ステージ
#define ALLSTAGE
//動く床
//#define STAGEMOVE
//壁
#define STAGEWALL

///////////////////Character////////////////////////

//プレイヤー
#define ALLPLAYER
//壁当たり判定
#define WallCollision

//エネミー
//#define ALLENEMY
//スライム
#define ENEMYSLIME

///////////////////UI////////////////////////

//HPゲージ
#define HPGAUGE
//EnemyAdd
//#define EnemyAdd