#pragma once

#include <basis/seadTypes.h>

namespace al {
class HitSensor;
}  // namespace al

// Minimal header: only the declarations needed by HackCap::movement and
// TalkNpc::attackSensor. The class itself is NotDecompiled
// (Player/PlayerEyeSensorHitHolder.o).
class PlayerEyeSensorHitHolder {
public:
    PlayerEyeSensorHitHolder(s32);

    void clear();
    // Both return whether the look-at was recorded; TalkNpc::attackSensor
    // tests isEnableRecordLookAt's result (original returns bool all the
    // way down through rs::sendMsgEventFlowScareCheck /
    // al::EventFlowExecutor::attackSensor, which the repo currently
    // declares void — see those headers).
    bool isEnableRecordLookAt(al::HitSensor* self, al::HitSensor* other) const;
    bool isEnableRecord(al::HitSensor* self, al::HitSensor* other) const;
    bool tryRecordHomingAttack(al::HitSensor* self, al::HitSensor* other);
    bool tryRecordLookAtTarget(al::HitSensor* self, al::HitSensor* other);
    void findNearestSensorDirH(sead::Vector3f* outDir, const sead::Vector3f& bodyPos,
                               const sead::Vector3f& dir, const sead::Vector3f& pos, f32 speed,
                               f32 param) const;
    al::HitSensor* findTargetMarkerSensor(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                          const sead::Vector3f& frontDir,
                                          const sead::Vector3f& upDir, f32, f32, f32) const;
    al::HitSensor* findNearestSensor(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                     const sead::Vector3f& dir, const sead::Vector3f& upDir, f32,
                                     f32, f32) const;

private:
    // Size decoded from TalkNpc::init (operator new(0x48)); the fields
    // are still unknown.
    u8 _0[0x38];

public:
    // Decoded from HackCap::attackSpin (0x3fbfcc helper gate): both are
    // pointers, tested for non-null; the exact pointee type is not yet
    // known, but the holder tracks HitSensors, so HitSensor* is used.
    al::HitSensor* _38;
    al::HitSensor* _40;
};

static_assert(sizeof(PlayerEyeSensorHitHolder) == 0x48);
