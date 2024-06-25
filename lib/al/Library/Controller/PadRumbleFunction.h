#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class LayoutActor;
class PadRumbleDirector;

struct PadRumbleParam {
    PadRumbleParam(f32 _0, f32 _4, f32 _8, f32 _c, f32 _10, f32 _14, s32 _18, bool _1C, bool _1D) {
        field_0 = _0;
        field_4 = _4;
        field_8 = _8;
        field_c = _c;
        field_10 = _10;
        field_14 = _14;
        field_18 = _18;
        field_1C = _1C;
        field_1D = _1D;
    }
    void setVolumeByBalance(f32);

    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_c;
    f32 field_10;
    f32 field_14;
    s32 field_18;
    bool field_1C;
    bool field_1D;
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
void checkIsAlivePadRumbleLoop(al::PadRumbleDirector*, const char*, const sead::Vector3f*, s32);
void checkIsAlivePadRumbleLoop(const al::LiveActor*, const char*, const sead::Vector3f*, s32);
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
