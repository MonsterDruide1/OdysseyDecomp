#pragma once

namespace al {
class LiveActor;
}

struct PlayerHackStartShaderParam {};

class PlayerHackStartShaderCtrl {
public:
    PlayerHackStartShaderCtrl(al::LiveActor*, PlayerHackStartShaderParam*);
    void setHost(al::LiveActor*);
    void start();
    void update();
    void end();

private:
    al::LiveActor* mParent;
    // TODO: fill this class out
    char unknown[0x30];
};
