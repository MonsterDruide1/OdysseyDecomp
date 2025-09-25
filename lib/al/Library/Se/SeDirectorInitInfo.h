#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {
class Projection;

struct SeDirectorInitInfo {
    s32 maxRequests = -1;
    s32 playerCount = -1;
    s32 listenerCount = 1;
    const char* defaultListenerName = "注視点";
    const char* defaultStageEffectName = nullptr;
    bool useMeInfo = true;
    bool useLoopSequencer = false;
    s32* field_28 = nullptr;
    s32 field_30 = -1;
    const sead::Vector3f* cameraPos = nullptr;
    const sead::Matrix34f* cameraMatrix = nullptr;
    const Projection* cameraProjection = nullptr;
    const sead::Vector3f* cameraAt = nullptr;
    void* field_58 = nullptr;
    f32 interiorSize = -1.0f;
    f32 pitchModifier = 1.0f;
};
}  // namespace al
