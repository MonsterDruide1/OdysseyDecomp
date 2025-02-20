#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class ActorInitInfo;

struct AnimScaleParam {
    AnimScaleParam();

    AnimScaleParam(f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, s32 h, f32 i, f32 j, f32 k,
                   f32 l);

    f32 _0 = 0.2f;
    f32 _4 = 0.91f;
    f32 _8 = 0.2f;
    f32 _c = 1.8f;
    f32 _10 = 0.06f;
    f32 _14 = 0.12f;
    f32 _18 = 0.91f;
    s32 _1c = 20;
    f32 _20 = 0.25f;
    f32 _24 = 0.9f;
    f32 _28 = 5.2f;
    f32 _2c = 0.05f;
};

static_assert(sizeof(AnimScaleParam) == 0x30);

class AnimScaleController : public NerveExecutor {
public:
    AnimScaleController(const AnimScaleParam*);
    void setAnimScaleParam(const AnimScaleParam*);
    void startAnim();
    void startVibration();
    void startHitReaction();
    void startAndSetScaleY(f32);
    void startAndSetScaleAnsVelocityY(f32, f32);
    void startAndSetScaleVelocityY(f32);
    void startAndAddScaleVelocityY(f32);
    void startCrush();
    void stopAnim();
    void stopAndReset();
    void resetScale();
    void stopAndSetScale(const sead::Vector3f&);
    void setScaleVelocityY(f32);
    void addScaleVelocityY(f32);
    void exeStop();
    void exeAnim();
    void updateScale(f32, f32);
    bool tryStop();
    void exeVibration();
    void exeHitReaction();
    void exeCrush();
    bool isHitReaction(s32) const;
    void setOriginalScale(const sead::Vector3f&);
    void update();

    const sead::Vector3f& getScale() { return mScale; }

private:
    AnimScaleParam* mParam;
    sead::Vector3f _18;
    sead::Vector3f mOriginalScale;
    sead::Vector3f mScale;
    f32 mScaleVelocityY;
};

static_assert(sizeof(AnimScaleController) == 0x40);
}  // namespace al
