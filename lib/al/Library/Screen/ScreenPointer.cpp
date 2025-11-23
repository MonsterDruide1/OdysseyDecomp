#include "Library/Screen/ScreenPointer.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Screen/ScreenPointDirector.h"
#include "Library/Screen/ScreenPointTarget.h"
#include "Library/Screen/ScreenPointerUtil.h"

namespace al {

ScreenPointer::ScreenPointer(const ActorInitInfo& initInfo, const char* name) {
    mHitTargetArray.allocBuffer(0x400, nullptr);
    mDirector = initInfo.screenPointDirector;
}

bool ScreenPointer::hitCheckSegment(const sead::Vector3f& posStart, const sead::Vector3f& posEnd) {
    return mDirector->hitCheckSegment(this, &mHitTargetArray, 0x400, posStart, posEnd);
}

bool ScreenPointer::hitCheckScreenCircle(const sead::Vector2f& pos, f32 radius, f32 screenRadius) {
    return mDirector->hitCheckScreenCircle(this, &mHitTargetArray, 0x400, pos, radius,
                                           screenRadius);
}

bool ScreenPointer::hitCheckLayoutCircle(const sead::Vector2f& pos, f32 radius, f32 layoutRadius,
                                         s32 (*cmp)(const ScreenPointTargetHitInfo*,
                                                    const ScreenPointTargetHitInfo*)) {
    return mDirector->hitCheckLayoutCircle(this, &mHitTargetArray, 0x400, pos, radius, layoutRadius,
                                           cmp);
}

bool ScreenPointer::recheckAndSortSegment(const sead::Vector3f& posStart,
                                          const sead::Vector3f& posEnd) {
    s32 size = mHitTargetArray.size();
    for (s32 i = 0; i < size; i++) {
        ScreenPointTarget* target = getHitTarget(i);
        f32 targetRadius = target->getTargetRadius();

        if ((target->getTargetPos() - posStart).length() <= targetRadius)
            continue;

        sead::Vector3f segmentPointStart = sead::Vector3f::zero;
        sead::Vector3f segmentPointEnd = sead::Vector3f::zero;
        bool isValid =
            checkHitSegmentSphereNearDepth(target->getTargetPos(), posStart, posEnd, targetRadius,
                                           &segmentPointStart, &segmentPointEnd);

        sead::Vector3f closestSegmentPoint;
        calcClosestSegmentPoint(&closestSegmentPoint, posStart, posEnd, target->getTargetPos());

        mHitTargetArray[i]->screenPointDistance =
            (target->getTargetPos() - closestSegmentPoint).length() - targetRadius;

        if (isValid)
            mHitTargetArray[i]->directPointDistance = (segmentPointStart - posStart).length();
        else
            mHitTargetArray[i]->directPointDistance = (closestSegmentPoint - posStart).length();

        mHitTargetArray[i]->segmentPointStart.set(segmentPointStart);
        mHitTargetArray[i]->segmentPointEnd.set(segmentPointEnd);
    }

    mHitTargetArray.sort(compareScreenPointTargetPriorDirectPoint);
    return true;
}

ScreenPointTarget* ScreenPointer::getHitTarget(s32 index) const {
    return mHitTargetArray(index)->target;
}

bool ScreenPointer::isHitTarget(const ScreenPointTarget* target) const {
    for (s32 i = 0; i < mHitTargetArray.size(); i++)
        if (getHitTarget(i) == target)
            return true;

    return false;
}

}  // namespace al
