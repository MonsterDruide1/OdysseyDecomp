#include "Library/Area/SwitchKeepOnAreaGroup.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/SwitchAreaTargetInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace al {
SwitchKeepOnAreaGroup::SwitchKeepOnAreaGroup(AreaObjGroup* areaObjGroup)
    : mAreaObjGroup(areaObjGroup) {
    _18 = areaObjGroup->getSize();
    if (_18 > 0)
        _10 = new AreaObj*[_18];
}

inline bool isInVolume(AreaObj* areaObj, sead::Vector3f* targetPos, s32 targetPosCount) {
    for (s32 j = 0; j < targetPosCount; j++)
        if (areaObj->isInVolume(targetPos[j]))
            return true;

    return false;
}

inline bool isNotInVolume(AreaObj* areaObj, sead::Vector3f* targetPos, s32 targetPosCount) {
    __asm("");
    for (s32 j = 0; j < targetPosCount; j++)
        if (!areaObj->isInVolume(targetPos[j]))
            return false;

    return true;
}

void SwitchKeepOnAreaGroup::update(const SwitchAreaTargetInfo* info) {
    _1c = 0;
    s32 size = mAreaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = mAreaObjGroup->getAreaObj(i);
        bool shouldSkip = false;
        for (s32 j = 0; j < _1c; j++) {
            if (isSameStageSwitch(areaObj, _10[j], "SwitchAreaOn")) {
                shouldSkip = true;

                break;
            }
        }

        if (shouldSkip)
            continue;

        bool local_54 = false;
        sead::Vector3f* targetPos = nullptr;
        s32 targetPosCount = 0;
        selectTargetPosArray(&local_54, &targetPos, &targetPosCount, areaObj, info);

        if (targetPosCount != 0) {
            bool result = local_54 ? isNotInVolume(areaObj, targetPos, targetPosCount) :
                                     isInVolume(areaObj, targetPos, targetPosCount);

            if (result && isExternalCondition()) {
                tryOnStageSwitch(areaObj, "SwitchAreaOn");
                _10[_1c] = areaObj;
                _1c++;

                continue;
            }
        }

        tryOffStageSwitch(areaObj, "SwitchAreaOn");
    }
}

void SwitchKeepOnAreaGroup::update(const sead::Vector3f& trans) {
    sead::Vector3f pos = trans;
    SwitchAreaTargetInfo info = {&pos, 1};

    update(&info);
}

void SwitchKeepOnAreaGroup::selectTargetPosArray(bool* out, sead::Vector3f** outTargetPos,
                                                 s32* outTargetPosCount, const AreaObj* areaObj,
                                                 const SwitchAreaTargetInfo* info) {
    s32 onCondition = 0;
    tryGetArg(&onCondition, areaObj->getPlacementInfo(), "OnCondition");

    switch (onCondition) {
    case 0:
        *out = false;
        *outTargetPos = info->getPlayerTargetPositions();
        *outTargetPosCount = info->getPlayerTargetPositionCount();

        break;
    case 1:
        *out = true;
        *outTargetPos = info->getPlayerTargetPositions();
        *outTargetPosCount = info->getPlayerTargetPositionCount();

        break;
    case 2:
        *out = false;
        *outTargetPos = info->getCameraLookAtPositions();
        *outTargetPosCount = info->getCameraLookAtPositionCount();

        break;
    default:
        break;
    }
}
}  // namespace al
