#pragma once

#include <basis/seadTypes.h>

#include "Project/Anim/AnimPlayerBase.h"

namespace al {
class AnimPlayerSimple : public AnimPlayerBase {
public:
    AnimPlayerSimple();

    virtual bool calcNeedUpdateAnimNext();
    virtual void applyTo();

    void startAnim(const char*);
    void update();
    void clearAnim();

    const char* getPlayingAnimName();
    f32 getAnimFrame();
    f32 getAnimFrameMax();
    f32 getAnimFrameMax(const char*);
    f32 getAnimFrameRate();

    void setAnimFrame(f32);
    void setAnimFrameMax(const char*);
    void setAnimFrameRate(float);

    bool isAnimExist(const char*);
    bool isAnimEnd();
    bool isAnimOneTime();
    bool isAnimOneTime(const char*);
    bool isAnimPlaying();
};
}  // namespace al
