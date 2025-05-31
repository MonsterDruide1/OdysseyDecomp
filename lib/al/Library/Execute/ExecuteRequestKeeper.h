#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

class ExecuteRequestInfo {
public:
    ExecuteRequestInfo();

private:
    u64 _0 = 0;
};

static_assert(sizeof(ExecuteRequestInfo) == 0x8);

class ExecuteRequestTable {
public:
    ExecuteRequestTable(s32 maxSize);

    void addRequest(LiveActor* actor) {
        for (s32 i = 0; i < mSize; i++)
            if (mRequests[i] == actor)
                return;

        mRequests[mSize++] = actor;
    }

    void removeRequest(LiveActor* actor) {
        for (s32 i = 0; i < mSize; i++) {
            if (mRequests[i] == actor) {
                mRequests[i] = mRequests[mSize - 1];
                mSize--;
            }
        }
    }

    s32 getSize() const { return mSize; };

    void clear() { mSize = 0; };

    LiveActor* getRequest(s32 index) const { return mRequests[index]; };

private:
    s32 mMaxSize = 0;
    s32 mSize = 0;
    LiveActor** mRequests = nullptr;
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
    void request(LiveActor* actor, s32 requestType);

private:
    ExecuteRequestTable* mRequestTables[4];
};

static_assert(sizeof(ExecuteRequestKeeper) == 0x20);

}  // namespace al
