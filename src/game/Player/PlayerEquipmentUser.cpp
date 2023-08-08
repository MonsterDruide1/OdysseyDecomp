#include "game/player/PlayerEquipmentUser.h"

#include "al/sensor/SensorMsg.h"
#include "rs/Sensor.h"

PlayerEquipmentUser::PlayerEquipmentUser(const PlayerInput* input,
                                         const PlayerModelHolder* model_holder,
                                         const IPlayerModelChanger* model_changer)
    : mPlayerInput(input), mPlayerModelHolder(model_holder), mPlayerModelChanger(model_changer) {}

void PlayerEquipmentUser::startEquip(al::HitSensor* equipment_sensor, al::HitSensor* source_sensor,
                                     const EquipmentInfo* info) {
    mSourceSensor = source_sensor;
    mEquipmentSensor = equipment_sensor;
    mEquipmentInfo = info;
}

void PlayerEquipmentUser::cancelEquip() {
    al::sendMsgPlayerReleaseEquipment(mEquipmentSensor, mSourceSensor);
}

void PlayerEquipmentUser::endEquip() {
    mEquipmentSensor = nullptr;
    mEquipmentInfo = nullptr;
    mSourceSensor = nullptr;
}

void PlayerEquipmentUser::noticeDamage() {
    rs::sendMsgNoticePlayerDamage(mEquipmentSensor, mSourceSensor);
}
