#include "Library/Area/SwitchKeepOnAreaGroup.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/SwitchAreaTargetInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace al {
SwitchKeepOnAreaGroup::SwitchKeepOnAreaGroup(AreaObjGroup* areaObjGroup)
    : mAreaObjGroup(areaObjGroup) {
    mOnAreaObjSize = areaObjGroup->getSize();
    if (mOnAreaObjSize > 0)
        mOnAreaObjs = new AreaObj*[mOnAreaObjSize];
}

inline bool isAnyPosInVolume(AreaObj* areaObj, sead::Vector3f* targetPos, s32 targetPosCount) {
    for (s32 j = 0; j < targetPosCount; j++)
        if (areaObj->isInVolume(targetPos[j]))
            return true;

    return false;
}

inline bool isAllPosInVolume(AreaObj* areaObj, sead::Vector3f* targetPos, s32 targetPosCount) {
    __asm("");
    for (s32 j = 0; j < targetPosCount; j++)
        if (!areaObj->isInVolume(targetPos[j]))
            return false;

    return true;
}

void SwitchKeepOnAreaGroup::update(const SwitchAreaTargetInfo* info) {
    mOnAreaObjCount = 0;
    s32 size = mAreaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = mAreaObjGroup->getAreaObj(i);
        bool shouldSkip = false;
        for (s32 j = 0; j < mOnAreaObjCount; j++) {
            if (isSameStageSwitch(areaObj, mOnAreaObjs[j], "SwitchAreaOn")) {
                shouldSkip = true;

                break;
            }
        }

        if (shouldSkip)
            continue;

        bool checkAllPos = false;
        sead::Vector3f* targetPos = nullptr;
        s32 targetPosCount = 0;
        selectTargetPosArray(&checkAllPos, &targetPos, &targetPosCount, areaObj, info);

        if (targetPosCount != 0) {
            bool result = checkAllPos ? isAllPosInVolume(areaObj, targetPos, targetPosCount) :
                                        isAnyPosInVolume(areaObj, targetPos, targetPosCount);

            if (result && isExternalCondition()) {
                tryOnStageSwitch(areaObj, "SwitchAreaOn");
                mOnAreaObjs[mOnAreaObjCount] = areaObj;
                mOnAreaObjCount++;

                continue;
            }
        }

        tryOffStageSwitch(areaObj, "SwitchAreaOn");
    }
}

void SwitchKeepOnAreaGroup::update(const sead::Vector3f& playerPos) {
    sead::Vector3f pos = playerPos;
    SwitchAreaTargetInfo info = {&pos, 1};

    update(&info);
}

void SwitchKeepOnAreaGroup::selectTargetPosArray(bool* outCheckAllPos,
                                                 sead::Vector3f** outTargetPos,
                                                 s32* outTargetPosCount, const AreaObj* areaObj,
                                                 const SwitchAreaTargetInfo* info) {
    s32 onCondition = 0;
    tryGetArg(&onCondition, areaObj->getPlacementInfo(), "OnCondition");

    switch (onCondition) {
    case 0:
        *outCheckAllPos = false;
        *outTargetPos = info->getPlayerTargetPositions();
        *outTargetPosCount = info->getPlayerTargetPositionCount();

        break;
    case 1:
        *outCheckAllPos = true;
        *outTargetPos = info->getPlayerTargetPositions();
        *outTargetPosCount = info->getPlayerTargetPositionCount();

        break;
    case 2:
        *outCheckAllPos = false;
        *outTargetPos = info->getCameraLookAtPositions();
        *outTargetPosCount = info->getCameraLookAtPositionCount();

        break;
    default:
        break;
    }
}
}  // namespace al
