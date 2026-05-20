#pragma once

namespace al {
class HitSensor;
}

class EquipmentInfo;
class IPlayerModelChanger;
class PlayerInput;
class PlayerModelHolder;
class PlayerStateRolling;

class PlayerEquipmentUser {
public:
    PlayerEquipmentUser(const PlayerInput* input, const PlayerModelHolder* model_holder,
                        const IPlayerModelChanger* model_changer);

    void startEquip(al::HitSensor* equipment_sensor, al::HitSensor* source_sensor,
                    const EquipmentInfo* info);
    void cancelEquip();
    void endEquip();
    bool noticeDamage();

    bool isEquip() const { return mEquipmentSensor != nullptr; }

    al::HitSensor* getEquipmentSensor() const { return mEquipmentSensor; }

    const EquipmentInfo* getEquipmentInfo() const { return mEquipmentInfo; }

    const PlayerInput* getPlayerInput() const { return mPlayerInput; }

    const IPlayerModelChanger* getModelChanger() const { return mPlayerModelChanger; }

    PlayerStateRolling* getPlayerStateRolling() const { return mPlayerStateRolling; }

private:
    al::HitSensor* mSourceSensor = nullptr;
    al::HitSensor* mEquipmentSensor = nullptr;
    const EquipmentInfo* mEquipmentInfo = nullptr;
    const PlayerInput* mPlayerInput;
    const PlayerModelHolder* mPlayerModelHolder;
    const IPlayerModelChanger* mPlayerModelChanger;
    PlayerStateRolling* mPlayerStateRolling = nullptr;
};

static_assert(sizeof(PlayerEquipmentUser) == 0x38);
