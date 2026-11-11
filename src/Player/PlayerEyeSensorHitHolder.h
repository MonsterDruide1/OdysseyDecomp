#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

// Holds the HitSensors the player's eye (cap) has recorded, in three
// sead::PtrArray<al::HitSensor> buffers plus the current look-at /
// priority targets. Layout decoded from Player/PlayerEyeSensorHitHolder.o:
// +0x00 homing-attack buffer, +0x10 target-marker buffer, +0x20 eat-target
// buffer (each a PtrArray: s32 mPtrNum, s32 mPtrNumMax, HitSensor** mPtrs),
// +0x30 the current look-at target, +0x38 the current priority target,
// +0x40 the previous priority target (clear() moves _38 here).
class PlayerEyeSensorHitHolder {
public:
    PlayerEyeSensorHitHolder(s32);

    void createTargetMarkerBuffer(s32 size);
    void createEatTargetBuffer(s32 size);
    void clear();
    bool tryRecordHomingAttack(al::HitSensor* self, al::HitSensor* other);
    bool tryRecordTargetMarker(al::HitSensor* self, al::HitSensor* other);
    bool tryRecordEatTarget(al::HitSensor* self, al::HitSensor* other);
    bool tryRecordLookAtTarget(al::HitSensor* self, al::HitSensor* other);
    bool tryRecordPriorityTarget(const al::LiveActor* actor, al::HitSensor* self,
                                 al::HitSensor* other);
    // Both return whether the look-at was recorded; TalkNpc::attackSensor
    // tests isEnableRecordLookAt's result (original returns bool all the
    // way down through rs::sendMsgEventFlowScareCheck /
    // al::EventFlowExecutor::attackSensor, which the repo currently
    // declares void — see those headers).
    bool isEnableRecordLookAt(al::HitSensor* self, al::HitSensor* other) const;
    bool isEnableRecord(al::HitSensor* self, al::HitSensor* other) const;
    // All four find functions return the nearest matching sensor, or
    // nullptr when none matches; *outDir receives the normalized
    // direction to it. (findNearestSensorDirH was previously declared
    // void — the original returns the sensor in x0.)
    al::HitSensor* findNearestSensorDirH(sead::Vector3f* outDir, const sead::Vector3f& bodyPos,
                                         const sead::Vector3f& dir, const sead::Vector3f& pos,
                                         f32 angle, f32 angleOffset) const;
    al::HitSensor* findNearestSensor(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                     const sead::Vector3f& dir, const sead::Vector3f& upDir, f32,
                                     f32, f32) const;
    al::HitSensor* findNearestSensorLimit(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                          const sead::Vector3f& dir, const sead::Vector3f& upDir,
                                          f32, f32, f32, f32) const;
    al::HitSensor* findTargetMarkerSensor(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                          const sead::Vector3f& frontDir,
                                          const sead::Vector3f& upDir, f32, f32, f32) const;
    al::HitSensor* findEatTargetSensor(sead::Vector3f* out, const sead::Vector3f& bodyPos,
                                       const sead::Vector3f& frontDir, const sead::Vector3f& upDir,
                                       f32, f32, f32) const;

    sead::PtrArray<al::HitSensor> _0;
    sead::PtrArray<al::HitSensor> _10;
    sead::PtrArray<al::HitSensor> _20;
    al::HitSensor* _30 = nullptr;
    // Decoded from HackCap::attackSpin (0x3fbfcc helper gate): both are
    // pointers, tested for non-null; the holder tracks HitSensors, so
    // HitSensor* is used.
    al::HitSensor* _38 = nullptr;
    al::HitSensor* _40 = nullptr;
};

static_assert(sizeof(PlayerEyeSensorHitHolder) == 0x48);
