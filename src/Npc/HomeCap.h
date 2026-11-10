#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

class HomeCapMovePoint;
class HomeCapStateMoveChair;
class HomeCapStateMoveSouvenir;
class HomeChair;

namespace HomeCapFunction {
const sead::Vector3f* getAimChairOffset();
}  // namespace HomeCapFunction

class HomeCap : public al::LiveActor {
public:
    HomeCap(const char* name, HomeChair* chair, const sead::PtrArray<HomeCapMovePoint>& points);

    void init(const al::ActorInitInfo& info) override;
    void appearMoveOtherChair(al::LiveActor* actor);
    void tryKillReturnHead();

    void exeHide();
    void exeMoveOtherChair();
    void exeWaitOtherChair();
    void exeWanderSign();
    void exeMoveSouvenir();
    void exeSleepWait();
    void exeNoMoveSouvenirSleepWait();
    void exeSleepStart();
    void exeSleep();
    void exeSleepEnd();

private:
    HomeChair* mChair = nullptr;
    HomeCapStateMoveChair* mStateMoveChair = nullptr;
    HomeCapStateMoveSouvenir* mStateMoveSouvenir = nullptr;
    const sead::PtrArray<HomeCapMovePoint>& mMovePoints;
};

static_assert(sizeof(HomeCap) == 0x128);
