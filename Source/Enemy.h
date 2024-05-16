#pragma once
#include"Graphics/Shader.h"
#include"Character.h"

//エネミー
class Enemy :public Character {
public:
    Enemy() {}
    ~Enemy() {}

    //更新処理
    virtual void Update(float elapedTime) = 0;
    //描画時間
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;
    //破棄
    void Destroy();

    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

    
private:
};