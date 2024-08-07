#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

class ExecuteRequestInfo {
public:
    ExecuteRequestInfo();

private:
    u64 _0;
};

static_assert(sizeof(ExecuteRequestInfo) == 0x8);

class ExecuteRequestTable {
public:
    ExecuteRequestTable(s32 maxSize);

private:
    s32 mCount;
    s32 mMaxSize;
    LiveActor** mRequests;
};

static_assert(sizeof(ExecuteRequestTable) == 0x10);

class ExecuteRequestKeeper {
public:
    enum Request : s32 {
        Request_Movement = 0x0,
        Request_RemoveFromMovement = 0x1,
        Request_Draw = 0x2,
        Request_RemoveFromDraw = 0x3
    };

    ExecuteRequestKeeper(s32 maxSize);

    void executeRequestActorMovementAllOn();
    void executeRequestActorMovementAllOff();
    void executeRequestActorDrawAllOn();
    void executeRequestActorDrawAllOff();
    void request(LiveActor* actor, Request requestType);

private:
    ExecuteRequestTable* mMovementOn;
    ExecuteRequestTable* mMovementOff;
    ExecuteRequestTable* mDrawOn;
    ExecuteRequestTable* mDrawOff;
};

static_assert(sizeof(ExecuteRequestKeeper) == 0x20);

}  // namespace al
