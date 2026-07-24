#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LayoutActor;
}  // namespace al

class SaveObjInfo;
class Shine;
class Yoshi;
class YoshiFruitShineHolder;

class YoshiFruitWatcher : public al::LiveActor, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_YoshiFruitWatcher;

    YoshiFruitWatcher();

    const char* getSceneObjName() const override;
    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;

    void registerShineHolder(YoshiFruitShineHolder* shineHolder);
    bool registerFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo);
    void noticeCurrentHackYoshi(Yoshi* yoshi);
    void noticeGetFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo);
    void saveGetFruit();
    void control() override;

    void exeWait();
    void exeGaugeAppear();
    void exeGaugeWait();
    void exeGaugeEnd();
    void exeDemoRequest();
    void exeDemoGauge();
    void exeDemoShine();

private:
    sead::PtrArray<al::LiveActor> mFruits;
    sead::PtrArray<SaveObjInfo> mGetFruitSaveInfos;
    YoshiFruitShineHolder* mShineHolder = nullptr;
    Yoshi* mCurrentHackYoshi = nullptr;
    s32 mGetFruitNum = 0;
    Shine* mDemoShine = nullptr;
    al::LayoutActor* mGaugeLayout = nullptr;
};

static_assert(sizeof(YoshiFruitWatcher) == 0x158);

namespace rs {
bool registerFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo);
void registerFruitShineHolder(YoshiFruitShineHolder* shineHolder);
void noticeCurrentHackYoshi(Yoshi* yoshi);
void noticeGetFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo);
}  // namespace rs
