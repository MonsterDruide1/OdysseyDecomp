#pragma once

#include <basis/seadTypes.h>

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

class EquipmentInfo;
class PlayerEquipmentUser;

namespace PlayerEquipmentFunction {
bool isEquipmentNoCapThrow(const PlayerEquipmentUser* user);
EquipmentInfo* createEquipmentInfoRocketFlower(al::LiveActor*, s32, f32);
EquipmentInfo* createEquipmentInfoKoopaCap(al::LiveActor*);
PlayerEquipmentUser* startEquip(al::HitSensor* equipmentSensor, al::HitSensor* sourceSensor,
                                const EquipmentInfo* info);
void endEquip(PlayerEquipmentUser** user);
void syncEquipVisibility(al::LiveActor* actor, const PlayerEquipmentUser* user);
bool tryNoticeEquipPlayerDamage(PlayerEquipmentUser* user);
bool isTriggerCapAction(const PlayerEquipmentUser* user);
bool isTriggerSwingLeft(const PlayerEquipmentUser* user);
bool isTriggerSwingRight(const PlayerEquipmentUser* user);
bool isPlayerRolling(const PlayerEquipmentUser* user);
bool isEquipmentForceDash(const PlayerEquipmentUser* user);
bool tryGetEquipmentForceDashInfo(s32*, f32*, const PlayerEquipmentUser* user);
}  // namespace PlayerEquipmentFunction
