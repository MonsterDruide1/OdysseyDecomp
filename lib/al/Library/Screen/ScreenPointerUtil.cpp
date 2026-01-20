#include "Library/Screen/ScreenPointerUtil.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Screen/ScreenPointDirector.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Screen/ScreenPointTarget.h"
#include "Library/Screen/ScreenPointer.h"

namespace al {

s32 compareScreenPointTarget(const ScreenPointTargetHitInfo* targetHitInfoA,
                             const ScreenPointTargetHitInfo* targetHitInfoB) {
    if (targetHitInfoA->directPointDistance - targetHitInfoB->directPointDistance < 0.0f)
        return -1;
    if (targetHitInfoA->directPointDistance - targetHitInfoB->directPointDistance > 0.0f)
        return 1;

    return targetHitInfoA->screenPointDistance < targetHitInfoB->screenPointDistance ? -1 :
           targetHitInfoA->screenPointDistance > targetHitInfoB->screenPointDistance ? 1 :
                                                                                       0;
}

// NON_MATCHING: https://decomp.me/scratch/bBeTv
s32 compareScreenPointTargetPriorDirectPoint(const ScreenPointTargetHitInfo* targetHitInfoA,
                                             const ScreenPointTargetHitInfo* targetHitInfoB) {
    f32 screenA = targetHitInfoA->screenPointDistance;
    f32 screenB = targetHitInfoB->screenPointDistance;

    if (screenB <= 0.0f && screenA > 0.0f)
        return -1;
    if (screenB > 0.0f && screenA <= 0.0f)
        return 1;

    f32 diffDirect = targetHitInfoA->directPointDistance - targetHitInfoB->directPointDistance;

    if (screenB <= 0.0f && screenA < 0.0f) {
        if (diffDirect < 0.0f)
            return -1;
        if (diffDirect > 0.0f)
            return 1;

        if (screenA < screenB)
            return -1;
        if (screenB < screenA)
            return 1;
        return 0;
    }

    if (screenA < screenB)
        return -1;
    if (screenB < screenA)
        return 1;

    if (diffDirect < 0.0f)
        return -1;
    if (diffDirect > 0.0f)
        return 1;
    return 0;
}

bool isExistScreenPointTargetKeeper(LiveActor* actor) {
    return actor->getScreenPointKeeper() != nullptr;
}

bool isScreenPointTargetArrayFull(LiveActor* actor) {
    return actor->getScreenPointKeeper()->isTargetArrayFull();
}

bool isExistScreenPointTarget(LiveActor* actor, const char* name) {
    return actor->getScreenPointKeeper()->isExistTarget(name);
}

ScreenPointTarget* addScreenPointTarget(LiveActor* actor, const ActorInitInfo& initInfo,
                                        const char* targetName, f32 radius, const char* jointName,
                                        const sead::Vector3f& vb) {
    ScreenPointTarget* target = actor->getScreenPointKeeper()->addTarget(
        actor, initInfo, targetName, radius, getTransPtr(actor), jointName, vb);

    ScreenPointDirector* director = initInfo.screenPointDirector;
    director->registerTarget(target);
    director->setCheckGroup(target);
    return target;
}

bool hitCheckSegmentScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector3f& posStart,
                                      const sead::Vector3f& posEnd) {
    return screenPointer->hitCheckSegment(posStart, posEnd);
}

bool hitCheckScreenCircleScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector2f& pos,
                                           f32 radius, f32 screenRadius) {
    return screenPointer->hitCheckScreenCircle(pos, radius, screenRadius);
}

bool hitCheckLayoutCircleScreenPointTarget(ScreenPointer* screenPointer, const sead::Vector2f& pos,
                                           f32 radius, f32 layoutRadius,
                                           s32 (*cmp)(const ScreenPointTargetHitInfo*,
                                                      const ScreenPointTargetHitInfo*)) {
    return screenPointer->hitCheckLayoutCircle(pos, radius, layoutRadius, cmp);
}

bool isHitScreenPointTarget(ScreenPointer* screenPointer, const ScreenPointTarget* target) {
    return screenPointer->isHitTarget(target);
}

bool sendMsgScreenPointTarget(const SensorMsg& message, ScreenPointer* screenPointer,
                              ScreenPointTarget* target) {
    return target->getActor()->receiveMsgScreenPoint(&message, screenPointer, target);
}

s32 getHitTargetNum(ScreenPointer* screenPointer) {
    return screenPointer->getHitTargetNum();
}

const sead::Vector3f& getHitTargetPos(ScreenPointer* screenPointer, s32 index) {
    return screenPointer->getHitTarget(index)->getTargetPos();
}

f32 getHitTargetRadius(ScreenPointer* screenPointer, s32 index) {
    return screenPointer->getHitTarget(index)->getTargetRadius();
}

}  // namespace al

namespace alScreenPointFunction {

void updateScreenPointAll(al::LiveActor* actor) {
    actor->getScreenPointKeeper()->update();
}

}  // namespace alScreenPointFunction
