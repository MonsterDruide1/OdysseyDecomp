#pragma once

namespace al {
class LiveActor;

class ExecuteRequestInfo {
public:
    ExecuteRequestInfo();

private:
    unsigned long _0;
};

class ExecuteRequestTable {
public:
    ExecuteRequestTable(s32 size);

private:
    s32 mRequestCount;
    LiveActor** mRequestArray;
};

class ExecuteRequestKeeper {
public:
    enum Request : s32 {
        Request_Movement = 0x0,
        Request_RemoveFromMovement = 0x1,
        Request_Draw = 0x2,
        Request_RemoveFromDraw = 0x3
    };

    ExecuteRequestKeeper(s32 size);

    void executeRequestActorMovementAllOn();
    void executeRequestActorMovementAllOff();
    void executeRequestActorDrawAllOn();
    void executeRequestActorDrawAllOff();
    void request(LiveActor* actor, Request request_type);

private:
    ExecuteRequestTable* mMovementOn;
    ExecuteRequestTable* mMovementOff;
    ExecuteRequestTable* mDrawOn;
    ExecuteRequestTable* mDrawOff;
};
}  // namespace al
