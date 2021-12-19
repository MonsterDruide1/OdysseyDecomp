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
    ExecuteRequestTable(int size);

private:
    int mRequestCount;
    LiveActor** mRequestArray;
};

class ExecuteRequestKeeper {
public:
    enum Request : int {
        Request_Movement = 0x0,
        Request_RemoveFromMovement = 0x1,
        Request_Draw = 0x2,
        Request_RemoveFromDraw = 0x3
    };
    
    ExecuteRequestKeeper(int size);

    void executeRequestActorMovementAllOn();
    void executeRequestActorMovementAllOff();
    void executeRequestActorDrawAllOn();
    void executeRequestActorDrawAllOff();
    void request(LiveActor* actor, ExecuteRequestKeeper::Request request_type);

private:
    ExecuteRequestTable* mMovement;
    ExecuteRequestTable* mRemoveFromMovement;
    ExecuteRequestTable* mDraw;
    ExecuteRequestTable* mRemoveFromDraw;
};
}  // namespace al
