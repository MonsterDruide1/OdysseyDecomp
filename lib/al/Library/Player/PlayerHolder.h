#pragma once

#include <basis/seadTypes.h>

namespace al {

class PadRumbleKeeper;
class LiveActor;

class PlayerHolder {
public:
    PlayerHolder(s32 maxPlayers);
    void clear();
    void registerPlayer(LiveActor* actor, PadRumbleKeeper* rumbleKeeper);
    LiveActor* getPlayer(s32 index) const;
    LiveActor* tryGetPlayer(s32 index) const;
    s32 getPlayerNum() const;
    s32 getBufferSize() const;
    bool isFull() const;
    bool isExistPadRumbleKeeper(s32 index) const;
    PadRumbleKeeper* getPadRumbleKeeper(s32 index) const;

private:
    struct Player {
        LiveActor* mActor = nullptr;
        PadRumbleKeeper* mPadRumbleKeeper = nullptr;
    };

    Player* mPlayers = nullptr;
    s32 mBufferSize = 0;
    s32 mPlayerNum = 0;
};
}  // namespace al
