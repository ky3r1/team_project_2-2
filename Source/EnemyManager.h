#pragma once
#include<vector>
#include"Enemy.h"
#include<set>

//エネミーマネージャー
class EnemyManager {
private:
    EnemyManager(){}
    ~EnemyManager(){}
public:
    //唯一のインスタンス取得
    static EnemyManager& Instance() {
        static EnemyManager instance;
        return instance;
    }

    //デバッグ用GUI
    void DrawDebugGUI();
    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    //エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
    //エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    //エネミー登録
    void Register(Enemy* enemy);
    //エネミー全削除
    void Clear();
    //エネミー削除
    void Remove(Enemy* enemy);
private:
    //エネミー同士の衝突処理   
    void CollisionEnemyVsEnemies();
private:
    std::vector<Enemy*>     enemies;
    std::set<Enemy*>        removes;
};