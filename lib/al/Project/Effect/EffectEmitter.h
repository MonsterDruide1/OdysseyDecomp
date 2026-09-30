#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Model/JointMtxPtr.h"

namespace nn::vfx {
struct EmitterHandleData {
    u8 _0[0x2c];
    s32 createId;
};

struct EmitterSet {
    u8 _0[0x210];
    f32 directionalVel;
};

class Handle {
public:
    EmitterSet* GetEmitterSet();

    bool isValid() const { return mHandle != nullptr && mCreateId == mHandle->createId; }

private:
    EmitterHandleData* mHandle = nullptr;
    s32 mCreateId = -1;
    u32 _c = 0;
};

static_assert(sizeof(Handle) == 0x10);
}  // namespace nn::vfx

namespace sead::ptcl {
using Handle = nn::vfx::Handle;
}  // namespace sead::ptcl

namespace al {
class EffectResourceInfo;
class EffectSystemInfo;

class EffectEmitter {
public:
    EffectEmitter(const EffectSystemInfo*, EffectResourceInfo*, s32);

    void initMtxPtr(JointMtxPtr);
    void updateMtxPtr(JointMtxPtr);
    nn::vfx::EmitterSet* createEmitter(const JointMtxPtr*, const sead::Vector3f*, s32, s32, u64);
    void tryDeleteEmitter(bool);
    void tryDeleteHandle(sead::ptcl::Handle*, bool);
    void setStopCalcAndDraw(bool);
    bool isActive() const;
    bool isEnableEmit() const;
    bool isFirstFrame() const;
    void resetFirstFrame();

    nn::vfx::Handle* getHandle() const { return mHandle; }

private:
    const EffectSystemInfo* mEffectSystemInfo = nullptr;
    sead::ptcl::Handle* mHandle = nullptr;
    sead::ptcl::Handle** mHandles = nullptr;
    s32 mHandleCount = 0;
    EffectResourceInfo* mResourceInfo = nullptr;
    bool mIsFirstFrame = false;
    u8 _29[3] = {};
    s32 _2c = -1;
    u8 _30[4] = {};
    JointMtxPtr mJointMtxPtr;
};

static_assert(sizeof(EffectEmitter) == 0x48);
}  // namespace al
