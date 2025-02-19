#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class LayoutActor;
class PadRumbleDirector;

struct PadRumbleParam {
    PadRumbleParam(f32 rumbleNear = 0.0f, f32 rumbleFar = 3000.0f, f32 rumbleVolume = 1.0f,
                   f32 rumblePitchVol = 1.0f, f32 rumblePitchLeft = 1.0f,
                   f32 rumblePitchRight = 1.0f, s32 _18 = 0, bool isUseController = false,
                   bool _1d = false)
        : mRumbleNear(rumbleNear), mRumbleFar(rumbleFar), mRumbleVolume(rumbleVolume),
          mRumblePitchVol(rumblePitchVol), mRumblePitchLeft(rumblePitchLeft),
          mRumblePitchRight(rumblePitchRight), field_18(_18), mIsUseController(isUseController),
          field_1d(_1d) {}

    void setVolumeByBalance(f32);

    f32 mRumbleNear;
    f32 mRumbleFar;
    f32 mRumbleVolume;
    f32 mRumblePitchVol;
    f32 mRumblePitchLeft;
    f32 mRumblePitchRight;
    s32 field_18;
    bool mIsUseController;
    bool field_1d;
};

}  // namespace al

namespace alPadRumbleFunction {
al::PadRumbleDirector* getPadRumbleDirector(const al::LiveActor*);
al::PadRumbleDirector* getPadRumbleDirector(const al::LayoutActor*);
void startPadRumble(al::PadRumbleDirector*, const sead::Vector3f&, const char*, f32, f32, s32);
void startPadRumbleWithParam(al::PadRumbleDirector*, const sead::Vector3f&, const char*,
                             const al::PadRumbleParam&, s32);
void startPadRumble(const al::LiveActor*, const char*, f32, f32, s32);
void startPadRumblePos(const al::LiveActor*, const sead::Vector3f&, const char*, f32, f32, s32);
void startPadRumbleWithParam(const al::LiveActor*, const sead::Vector3f&, const char*,
                             const al::PadRumbleParam&, s32);
void startPadRumbleNo3D(al::PadRumbleDirector*, const char*, s32);
void startPadRumbleNo3DWithParam(al::PadRumbleDirector*, const char*, const al::PadRumbleParam&,
                                 s32);
void startPadRumbleNo3DWithParam(al::PadRumbleDirector*, const char*, f32, f32, f32, f32, s32);
void startPadRumbleNo3D(const al::LiveActor*, const char*, s32);
void startPadRumbleNo3DWithParam(const al::LiveActor*, const char*, const al::PadRumbleParam&, s32);
void startPadRumbleNo3DWithParam(const al::LiveActor*, const char*, f32, f32, f32, f32, s32);
void stopPadRumbleOneTime(al::PadRumbleDirector*, const char*, s32);
void stopPadRumbleOneTime(const al::LiveActor*, const char*, s32);
void startPadRumbleLoop(al::PadRumbleDirector*, const char*, const sead::Vector3f*, f32, f32, s32);
void startPadRumbleLoopWithParam(al::PadRumbleDirector*, const char*, const sead::Vector3f*,
                                 const al::PadRumbleParam&, s32);
void startPadRumbleLoop(const al::LiveActor*, const char*, const sead::Vector3f*, f32, f32, s32);
void startPadRumbleLoopWithParam(const al::LiveActor*, const char*, const sead::Vector3f*,
                                 const al::PadRumbleParam&, s32);
void startPadRumbleLoopNo3D(al::PadRumbleDirector*, const char*, const sead::Vector3f*, s32);
void startPadRumbleLoopNo3DWithParam(al::PadRumbleDirector*, const char*, const sead::Vector3f*,
                                     const al::PadRumbleParam&, s32);
void startPadRumbleLoopNo3D(const al::LiveActor*, const char*, const sead::Vector3f*, s32);
void startPadRumbleLoopNo3DWithParam(const al::LiveActor*, const char*, const sead::Vector3f*,
                                     const al::PadRumbleParam&, s32);
void stopPadRumbleLoop(al::PadRumbleDirector*, const char*, const sead::Vector3f*, s32);
void stopPadRumbleLoop(const al::LiveActor*, const char*, const sead::Vector3f*, s32);
bool checkIsAlivePadRumbleLoop(al::PadRumbleDirector*, const char*, const sead::Vector3f*, s32);
bool checkIsAlivePadRumbleLoop(const al::LiveActor*, const char*, const sead::Vector3f*, s32);
void startPadRumbleLoopControlable(const al::LiveActor*, const char*, const sead::Vector3f*, s32);
void changePadRumbleLoopVolmue(const al::LiveActor*, const char*, const sead::Vector3f*, f32, f32,
                               s32);
void changePadRumbleLoopVolmueEaseInRange(const al::LiveActor*, const char*, const sead::Vector3f*,
                                          f32, f32, f32, f32, f32, s32);
void changePadRumbleLoopPitch(const al::LiveActor*, const char*, const sead::Vector3f*, f32, f32,
                              s32);
void startPadRumbleDirectValue(const al::LiveActor*, f32, f32, f32, f32, f32, f32, s32);
void stopPadRumbleDirectValue(const al::LiveActor*, s32);
void startPadRumbleWithVolume(const al::LiveActor*, const char*, f32, f32, s32);
void startPadRumbleWithVolume(al::PadRumbleDirector*, const char*, f32, f32, s32);
void makePadRumbleParamNearFarVolume(al::PadRumbleParam*, f32, f32, f32);
void makePadRumbleParamNearFarVolumeLR(al::PadRumbleParam*, f32, f32, f32, f32);
void makePadRumbleParamNearFarVolumePitch(al::PadRumbleParam*, f32, f32, f32, f32);
void makePadRumbleParamNearFarVolumePitchLR(al::PadRumbleParam*, f32, f32, f32, f32, f32, f32);
}  // namespace alPadRumbleFunction
