#pragma once

#include <basis/seadTypes.h>

#include "Project/Anim/AnimPlayerBase.h"

namespace al {
struct AnimPlayerInitInfo;
struct AnimResInfo;

class AnimPlayerSimple : public AnimPlayerBase {
public:
    AnimPlayerSimple();

    virtual void applyTo();
    virtual void init(const AnimPlayerInitInfo*);
    virtual void setAnimToModel(const AnimResInfo*);

    bool calcNeedUpdateAnimNext() override;

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

private:
};
}  // namespace al
