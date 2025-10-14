#pragma once
#include "Camera.h"
#include "Player.h"
#include "Platform.h"

class RailCamera {
public:
    RailCamera();

    void Initialize();
    void Update();

    void SetPlayer(Player* player);
    void SetPlayerOffset(const Vector3& offset);
    void SetPlatform(Platform* platform);
    void SetPlatformOffset(const Vector3& offset);
    void SetSpeed(float speed);

    Camera* GetCamera();

private:
    Camera camera_;
    float speed_ = 3.0f;
    Player* player_ = nullptr;
    Vector3 playerOffset_;
    Platform* platform_ = nullptr;
    Vector3 platformOffset_ = { 0,0,0 };
};