#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
struct SubActorInfo;

class SubActorKeeper {
public:
    SubActorKeeper(LiveActor* rootActor);
    void registerSubActor(LiveActor* subActor, u32 syncType);
    void init(const ActorInitInfo& initInfo, const char* suffix, s32 maxSubActors);

    static SubActorKeeper* create(LiveActor* rootActor);
    static SubActorKeeper* tryCreate(LiveActor* rootActor, const char* suffix, s32 maxSubActors);

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
