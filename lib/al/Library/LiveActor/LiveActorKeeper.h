#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
struct ActorInitInfo;
struct SubActorInfo;

class SubActorKeeper {
public:
    SubActorKeeper(LiveActor*);
    void registerSubActor(LiveActor*, u32);
    void init(const ActorInitInfo&, const char*, s32);

    static SubActorKeeper* create(LiveActor*);
    static SubActorKeeper* tryCreate(LiveActor*, const char*, s32);

    s32 getCurActorCount() const { return mCurActorCount; }

    SubActorInfo* getActorInfo(s32 index) const { return mBuffer[index]; }

protected:
    friend class alSubActorFunction;

private:
    LiveActor* mRootActor;
    s32 mMaxActorCount = 0;
    s32 mCurActorCount = 0;
    SubActorInfo** mBuffer = nullptr;
};

}  // namespace al
