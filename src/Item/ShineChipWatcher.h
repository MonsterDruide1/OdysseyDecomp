#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AreaObjGroup;
struct ActorInitInfo;
}  // namespace al

class RouteGuidePointActor;
class Shine;
class ShineChip;

class ShineChipWatcher : public al::LiveActor {
public:
    static s32 getCompleteNum();
    void init(const al::ActorInitInfo& info) override;
    void createShineChips(const al::ActorInitInfo& info, const char* shineName,
                          const char* linkName);
    void initAfterPlacement() override;
    void updateHints();
    bool isInArea() const;
    void appear() override;
    void kill() override;
    s32 getCount() const;
    bool isWatch(const al::LiveActor* actor) const;
    bool updateCount();
    bool tryStartAppearShine();
    bool isAppearedShine() const;
    void addDemoActorWithChips();
    void exeWait();
    void exeStartAppearShine();
    void exeWaitAppearShine();
    void exeComplete();
    void exeDone();

    const Shine* getShine() const { return mShine; }

private:
    u8 _108[0x10];
    sead::PtrArray<ShineChip> mShineChips;
    Shine* mShine = nullptr;
    sead::Vector3f mAreaCenter = sead::Vector3f::zero;
    f32 mAreaRadius = 0.0f;
    RouteGuidePointActor* mRouteGuidePointActor = nullptr;
    al::AreaObjGroup* mCounterAreaGroup = nullptr;
};
