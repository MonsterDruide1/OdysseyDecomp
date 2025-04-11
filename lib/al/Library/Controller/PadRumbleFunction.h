#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class LayoutActor;
class PadRumbleDirector;

struct PadRumbleParam {
    PadRumbleParam(f32 near = 0.0f, f32 far = 3000.0f, f32 volumeLeft = 1.0f,
                   f32 volumeRight = 1.0f, f32 pitchLeft = 1.0f, f32 pitchRight = 1.0f, s32 _18 = 0,
                   bool isUseController = false, bool _1d = false)
        : near(near), far(far), volumeLeft(volumeLeft), volumeRight(volumeRight),
          pitchLeft(pitchLeft), pitchRight(pitchRight), field_18(_18),
          isUseController(isUseController), field_1d(_1d) {}

    void setVolumeByBalance(f32 balance);

    f32 near;
    f32 far;
    f32 volumeLeft;
    f32 volumeRight;
    f32 pitchLeft;
    f32 pitchRight;
    s32 field_18;
    bool isUseController;
    bool field_1d;
};

}  // namespace al

namespace alPadRumbleFunction {
al::PadRumbleDirector* getPadRumbleDirector(const al::LiveActor* actor);
al::PadRumbleDirector* getPadRumbleDirector(const al::LayoutActor* layoutActor);
void startPadRumble(al::PadRumbleDirector* director, const sead::Vector3f& position,
                    const char* name, f32 near, f32 far, s32 port = -1);
void startPadRumbleWithParam(al::PadRumbleDirector* director, const sead::Vector3f& position,
                             const char* name, const al::PadRumbleParam& rumbleParam,
                             s32 port = -1);
void startPadRumble(const al::LiveActor* actor, const char* name, f32 near, f32 far, s32 port = -1);
void startPadRumblePos(const al::LiveActor* actor, const sead::Vector3f& position, const char* name,
                       f32 near, f32 far, s32 port = -1);
void startPadRumbleWithParam(const al::LiveActor* actor, const sead::Vector3f& position,
                             const char* name, const al::PadRumbleParam& rumbleParam,
                             s32 port = -1);
void startPadRumbleNo3D(al::PadRumbleDirector* director, const char* name, s32 port = -1);
void startPadRumbleNo3DWithParam(al::PadRumbleDirector* director, const char* name,
                                 const al::PadRumbleParam& rumbleParam, s32 port = -1);
void startPadRumbleNo3DWithParam(al::PadRumbleDirector* director, const char* name, f32 volumeLeft,
                                 f32 volumeRight, f32 pitchLeft, f32 pitchRight, s32 port = -1);
void startPadRumbleNo3D(const al::LiveActor* actor, const char* name, s32 port = -1);
void startPadRumbleNo3DWithParam(const al::LiveActor* actor, const char* name,
                                 const al::PadRumbleParam& rumbleParam, s32 port = -1);
void startPadRumbleNo3DWithParam(const al::LiveActor* actor, const char* name, f32 volumeLeft,
                                 f32 volumeRight, f32 pitchLeft, f32 pitchRight, s32 port = -1);
void stopPadRumbleOneTime(al::PadRumbleDirector* director, const char* name, s32 port = -1);
void stopPadRumbleOneTime(const al::LiveActor* actor, const char* name, s32 port = -1);
void startPadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                        const sead::Vector3f* position, f32 near, f32 far, s32 port = -1);
void startPadRumbleLoopWithParam(al::PadRumbleDirector* director, const char* name,
                                 const sead::Vector3f* position,
                                 const al::PadRumbleParam& rumbleParam, s32 port = -1);
void startPadRumbleLoop(const al::LiveActor* actor, const char* name,
                        const sead::Vector3f* position, f32 near, f32 far, s32 port = -1);
void startPadRumbleLoopWithParam(const al::LiveActor* actor, const char* name,
                                 const sead::Vector3f* position,
                                 const al::PadRumbleParam& rumbleParam, s32 port = -1);
void startPadRumbleLoopNo3D(const al::LiveActor* actor, const char* name,
                            const sead::Vector3f* position, s32 port = -1);
void startPadRumbleLoopNo3DWithParam(al::PadRumbleDirector* director, const char* name,
                                     const sead::Vector3f* position,
                                     const al::PadRumbleParam& rumbleParam, s32 port = -1);
void startPadRumbleLoopNo3DWithParam(const al::LiveActor* actor, const char* name,
                                     const sead::Vector3f* position,
                                     const al::PadRumbleParam& rumbleParam, s32 port = -1);
void stopPadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                       const sead::Vector3f* position, s32 port = -1);
void stopPadRumbleLoop(const al::LiveActor* actor, const char* name, const sead::Vector3f* position,
                       s32 port = -1);
bool checkIsAlivePadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                               const sead::Vector3f* position, s32 port = -1);
bool checkIsAlivePadRumbleLoop(const al::LiveActor* actor, const char* name,
                               const sead::Vector3f* position, s32 port = -1);
void startPadRumbleLoopControlable(const al::LiveActor* actor, const char* name,
                                   const sead::Vector3f* position, s32 port = -1);
void changePadRumbleLoopVolmue(const al::LiveActor* actor, const char* name,
                               const sead::Vector3f* position, f32 volumeLeft, f32 volumeRight,
                               s32 port = -1);
void changePadRumbleLoopVolmueEaseInRange(const al::LiveActor* actor, const char* name,
                                          const sead::Vector3f* position, f32 time, f32 startTime,
                                          f32 stopTime, f32 volumeLeft, f32 volumeRight,
                                          s32 port = -1);
void changePadRumbleLoopPitch(const al::LiveActor* actor, const char* name,
                              const sead::Vector3f* position, f32 pitchLeft, f32 pitchRight,
                              s32 port = -1);
void startPadRumbleDirectValue(const al::LiveActor* actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f,
                               s32 port = -1);
void stopPadRumbleDirectValue(const al::LiveActor* actor, s32 port = -1);
void startPadRumbleWithVolume(const al::LiveActor* actor, const char* name, f32 volumeLeft,
                              f32 volumeRight, s32 port = -1);
void startPadRumbleWithVolume(al::PadRumbleDirector* director, const char* name, f32 volumeLeft,
                              f32 volumeRight, s32 port = -1);
void makePadRumbleParamNearFarVolume(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                     f32 volume);
void makePadRumbleParamNearFarVolumeLR(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                       f32 volumeLeft, f32 volumeRight);
void makePadRumbleParamNearFarVolumePitch(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                          f32 volume, f32 pitch);
void makePadRumbleParamNearFarVolumePitchLR(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                            f32 volumeLeft, f32 volumeRight, f32 pitchLeft,
                                            f32 pitchRight);
}  // namespace alPadRumbleFunction
