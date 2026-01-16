#include "Library/Area/SwitchOnAreaGroup.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace al {
SwitchOnAreaGroup::SwitchOnAreaGroup(AreaObjGroup* areaObjGroup) : mAreaObjGroup(areaObjGroup) {}

void SwitchOnAreaGroup::update(const sead::Vector3f* positions, s32 posCount) {
    s32 size = mAreaObjGroup->getSize();
    for (s32 i = 0; i < size; i++) {
        AreaObj* areaObj = mAreaObjGroup->getAreaObj(i);
        if (isOnStageSwitch(areaObj, "SwitchAreaOn"))
            continue;

        for (s32 j = 0; j < posCount; j++) {
            if (areaObj->isInVolume(positions[j]) && isExternalCondition()) {
                onStageSwitch(areaObj, "SwitchAreaOn");

                break;
            }
        }
    }
}

void SwitchOnAreaGroup::update(const sead::Vector3f& position) {
    sead::Vector3f positions[1] = {position};
    update(positions, 1);
}

bool SwitchOnAreaGroup::isExternalCondition() const {
    return true;
}
}  // namespace al
