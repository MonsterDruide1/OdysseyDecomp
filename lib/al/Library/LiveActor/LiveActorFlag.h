#pragma once

namespace al {

struct LiveActorFlag {
    bool isDead = true;
    bool isClipped = false;
    bool isClippingInvalid = true;
    bool isDrawClipped = false;
    bool isDisableCalcAnim = false;
    bool isModelHidden = false;
    bool isCollideOff = true;
    bool field_7 = false;
    bool isMaterialCodeValid = false;
    bool isPuddleMaterialValid = false;
    bool isAreaTargetOn = true;
    bool isUpdateMovementEffectAudioCollisionSensor = true;

    LiveActorFlag();
};

static_assert(sizeof(LiveActorFlag) == 0xC);

}  // namespace al
