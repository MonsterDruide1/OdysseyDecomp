#include "Library/Controller/PadRumbleFunction.h"

#include "Library/Controller/PadRumbleDirector.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutSceneInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace al {
void PadRumbleParam::setVolumeByBalance(f32 balance) {
    f32 angle = sead::Mathf::deg2rad(balance * 90.0f);
    volumeLeft = sead::Mathf::cos(angle);
    volumeRight = sead::Mathf::sin(angle);
}
}  // namespace al

namespace alPadRumbleFunction {
al::PadRumbleDirector* getPadRumbleDirector(const al::LiveActor* actor) {
    return actor->getSceneInfo()->padRumbleDirector;
}

al::PadRumbleDirector* getPadRumbleDirector(const al::LayoutActor* layoutActor) {
    return layoutActor->getLayoutSceneInfo()->getPadRumbleDirector();
}

void startPadRumble(al::PadRumbleDirector* director, const sead::Vector3f& position,
                    const char* name, f32 near, f32 far, s32 port) {
    al::PadRumbleParam rumbleParam(near, far);
    director->startRumble(name, position, rumbleParam, port);
}

void startPadRumbleWithParam(al::PadRumbleDirector* director, const sead::Vector3f& position,
                             const char* name, const al::PadRumbleParam& rumbleParam, s32 port) {
    director->startRumble(name, position, rumbleParam, port);
}

void startPadRumble(const al::LiveActor* actor, const char* name, f32 near, f32 far, s32 port) {
    startPadRumblePos(actor, al::getTrans(actor), name, near, far, port);
}

void startPadRumblePos(const al::LiveActor* actor, const sead::Vector3f& position, const char* name,
                       f32 near, f32 far, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        startPadRumble(director, position, name, near, far, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        al::PadRumbleParam rumbleParam(near, far);
        startPadRumbleWithParam(director, position, name, rumbleParam, al::getPlayerPort(actor, i));
    }
}

void startPadRumbleWithParam(const al::LiveActor* actor, const sead::Vector3f& position,
                             const char* name, const al::PadRumbleParam& rumbleParam, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    startPadRumbleWithParam(director, position, name, rumbleParam, port);
}

void startPadRumbleNo3D(al::PadRumbleDirector* director, const char* name, s32 port) {
    director->startRumbleNo3D(name, {}, port);
}

void startPadRumbleNo3DWithParam(al::PadRumbleDirector* director, const char* name,
                                 const al::PadRumbleParam& rumbleParam, s32 port) {
    director->startRumbleNo3D(name, rumbleParam, port);
}

void startPadRumbleNo3DWithParam(al::PadRumbleDirector* director, const char* name, f32 volumeLeft,
                                 f32 volumeRight, f32 pitchLeft, f32 pitchRight, s32 port) {
    al::PadRumbleParam rumbleParam(0.0f, 0.0f, volumeLeft, volumeRight, pitchLeft, pitchRight);
    director->startRumbleNo3D(name, rumbleParam, port);
}

void startPadRumbleNo3D(const al::LiveActor* actor, const char* name, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        startPadRumbleNo3D(director, name, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        al::PadRumbleParam rumbleParam;
        startPadRumbleNo3DWithParam(director, name, rumbleParam, al::getPlayerPort(actor, i));
    }
}

void startPadRumbleNo3DWithParam(const al::LiveActor* actor, const char* name,
                                 const al::PadRumbleParam& rumbleParam, s32 port) {
    getPadRumbleDirector(actor)->startRumbleNo3D(name, rumbleParam, port);
}

void startPadRumbleNo3DWithParam(const al::LiveActor* actor, const char* name, f32 volumeLeft,
                                 f32 volumeRight, f32 pitchLeft, f32 pitchRight, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    startPadRumbleNo3DWithParam(director, name, volumeLeft, volumeRight, pitchLeft, pitchRight,
                                port);
}

void stopPadRumbleOneTime(al::PadRumbleDirector* director, const char* name, s32 port) {
    director->stopPadRumbleOneTime(name, port);
}

void stopPadRumbleOneTime(const al::LiveActor* actor, const char* name, s32 port) {
    getPadRumbleDirector(actor)->stopPadRumbleOneTime(name, port);
}

void startPadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                        const sead::Vector3f* position, f32 near, f32 far, s32 port) {
    // BUG: ignores near and far parameters
    director->startRumbleLoop(name, position, {}, port);
}

void startPadRumbleLoopWithParam(al::PadRumbleDirector* director, const char* name,
                                 const sead::Vector3f* position,
                                 const al::PadRumbleParam& rumbleParam, s32 port) {
    director->startRumbleLoop(name, position, rumbleParam, port);
}

void startPadRumbleLoop(const al::LiveActor* actor, const char* name,
                        const sead::Vector3f* position, f32 near, f32 far, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        al::PadRumbleParam rumbleParam(near, far);
        director->startRumbleLoop(name, position, rumbleParam, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        al::PadRumbleParam rumbleParam(near, far);
        director->startRumbleLoop(name, position, rumbleParam, al::getPlayerPort(actor, i));
    }
}

void startPadRumbleLoopWithParam(const al::LiveActor* actor, const char* name,
                                 const sead::Vector3f* position,
                                 const al::PadRumbleParam& rumbleParam, s32 port) {
    getPadRumbleDirector(actor)->startRumbleLoop(name, position, rumbleParam, port);
}

void startPadRumbleLoopNo3D(al::PadRumbleDirector* director, const char* name,
                            const sead::Vector3f* position, s32 port) {
    director->startRumbleLoopNo3D(name, position, {}, port);
}

void startPadRumbleLoopNo3DWithParam(al::PadRumbleDirector* director, const char* name,
                                     const sead::Vector3f* position,
                                     const al::PadRumbleParam& rumbleParam, s32 port) {
    director->startRumbleLoopNo3D(name, position, rumbleParam, port);
}

void startPadRumbleLoopNo3D(const al::LiveActor* actor, const char* name,
                            const sead::Vector3f* position, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->startRumbleLoopNo3D(name, position, {}, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        al::PadRumbleParam rumbleParam;
        director->startRumbleLoopNo3D(name, position, rumbleParam, al::getPlayerPort(actor, i));
    }
}

void startPadRumbleLoopNo3DWithParam(const al::LiveActor* actor, const char* name,
                                     const sead::Vector3f* position,
                                     const al::PadRumbleParam& rumbleParam, s32 port) {
    getPadRumbleDirector(actor)->startRumbleLoopNo3D(name, position, rumbleParam, port);
}

void stopPadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                       const sead::Vector3f* position, s32 port) {
    director->stopRumbleLoop(name, position, port);
}

void stopPadRumbleLoop(const al::LiveActor* actor, const char* name, const sead::Vector3f* position,
                       s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->stopRumbleLoop(name, position, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->stopRumbleLoop(name, position, al::getPlayerPort(actor, i));
    }
}

bool checkIsAlivePadRumbleLoop(al::PadRumbleDirector* director, const char* name,
                               const sead::Vector3f* position, s32 port) {
    return director->checkIsAliveRumbleLoop(name, position, port);
}

bool checkIsAlivePadRumbleLoop(const al::LiveActor* actor, const char* name,
                               const sead::Vector3f* position, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    return director->checkIsAliveRumbleLoop(name, position, port);
}

void startPadRumbleLoopControlable(const al::LiveActor* actor, const char* name,
                                   const sead::Vector3f* position, s32 port) {
    startPadRumbleLoopNo3D(actor, name, position, port);
}

void changePadRumbleLoopVolmue(const al::LiveActor* actor, const char* name,
                               const sead::Vector3f* position, f32 volumeLeft, f32 volumeRight,
                               s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->changeRumbleLoopVolume(name, position, volumeLeft, volumeRight, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->changeRumbleLoopVolume(name, position, volumeLeft, volumeRight,
                                         al::getPlayerPort(actor, i));
    }
}

void changePadRumbleLoopVolmueEaseInRange(const al::LiveActor* actor, const char* name,
                                          const sead::Vector3f* position, f32 time, f32 startTime,
                                          f32 stopTime, f32 volumeLeft, f32 volumeRight, s32 port) {
    f32 easeIn = al::easeIn(al::calcRate01(time, startTime, stopTime));
    changePadRumbleLoopVolmue(actor, name, position, easeIn * volumeLeft, easeIn * volumeRight,
                              port);
}

void changePadRumbleLoopPitch(const al::LiveActor* actor, const char* name,
                              const sead::Vector3f* position, f32 pitchLeft, f32 pitchRight,
                              s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->changeRumbleLoopPitch(name, position, pitchLeft, pitchRight, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->changeRumbleLoopPitch(name, position, pitchLeft, pitchRight,
                                        al::getPlayerPort(actor, i));
    }
}

// TODO: add proper names to a, b, c, d, e, f and in header
void startPadRumbleDirectValue(const al::LiveActor* actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f,
                               s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->startRumbleDirectValue(a, b, c, d, e, f, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->startRumbleDirectValue(a, b, c, d, e, f, al::getPlayerPort(actor, i));
    }
}

void stopPadRumbleDirectValue(const al::LiveActor* actor, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->stopRumbleDirectValue(port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->stopRumbleDirectValue(al::getPlayerPort(actor, i));
    }
}

void startPadRumbleWithVolume(const al::LiveActor* actor, const char* name, f32 volumeLeft,
                              f32 volumeRight, s32 port) {
    al::PadRumbleDirector* director = getPadRumbleDirector(actor);
    if (port >= 0) {
        director->startRumbleWithVolume(name, volumeLeft, volumeRight, port);
        return;
    }

    s32 playerNum = al::getPlayerNumMax(actor);
    for (s32 i = 0; i < playerNum; i++) {
        if (al::isPlayerDead(actor, i))
            continue;
        director->startRumbleWithVolume(name, volumeLeft, volumeRight, al::getPlayerPort(actor, i));
    }
}

void startPadRumbleWithVolume(al::PadRumbleDirector* director, const char* name, f32 volumeLeft,
                              f32 volumeRight, s32 port) {
    director->startRumbleWithVolume(name, volumeLeft, volumeRight, port);
}

void makePadRumbleParamNearFarVolume(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                     f32 volume) {
    makePadRumbleParamNearFarVolumeLR(rumbleParam, near, far, volume, volume);
}

void makePadRumbleParamNearFarVolumeLR(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                       f32 volumeLeft, f32 volumeRight) {
    makePadRumbleParamNearFarVolumePitchLR(rumbleParam, near, far, volumeLeft, volumeRight, 1.0f,
                                           1.0f);
}

void makePadRumbleParamNearFarVolumePitch(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                          f32 volume, f32 pitch) {
    makePadRumbleParamNearFarVolumePitchLR(rumbleParam, near, far, volume, volume, pitch, pitch);
}

void makePadRumbleParamNearFarVolumePitchLR(al::PadRumbleParam* rumbleParam, f32 near, f32 far,
                                            f32 volumeLeft, f32 volumeRight, f32 pitchLeft,
                                            f32 pitchRight) {
    rumbleParam->far = far;
    rumbleParam->near = near;
    rumbleParam->volumeLeft = volumeLeft;
    rumbleParam->volumeRight = volumeRight;
    rumbleParam->pitchLeft = pitchLeft;
    rumbleParam->pitchRight = pitchRight;
    rumbleParam->field_18 = 0;
}
}  // namespace alPadRumbleFunction
