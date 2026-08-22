#include "Player/EquipmentInfo.h"

EquipmentInfo::EquipmentInfo(const al::LiveActor* actor) {
    mActor = actor;
    _8 = 0;
    _c = 0;
    _10 = 0.0f;
}

void EquipmentInfo::onNoCapThrow() {
    _8 |= 1;
}

void EquipmentInfo::onForceDash(s32 param_1, f32 param_2) {
    _8 |= 2;
    _c = param_1;
    _10 = param_2;
}

bool EquipmentInfo::isNoCapThrow() const {
    return (_8 & 1) != 0;
}

bool EquipmentInfo::isForceDash() const {
    return (_8 & 2) != 0;
}
