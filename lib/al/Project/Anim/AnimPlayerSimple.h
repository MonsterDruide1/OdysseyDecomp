#pragma once

#include "Project/Anim/AnimPlayerBase.h"

namespace al {
struct AnimPlayerInitInfo;
struct AnimResInfo;

class AnimPlayerSimple : public AnimPlayerBase {
public:
    AnimPlayerSimple();

    virtual void init(const AnimPlayerInitInfo*) = 0;
    virtual void setAnimToModel(const AnimResInfo*) = 0;
    virtual void applyTo();

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
    void setAnimFrameRate(f32);

    bool isAnimExist(const char*);
    bool isAnimEnd();
    bool isAnimOneTime();
    bool isAnimOneTime(const char*);
    bool isAnimPlaying();

private:
};
}  // namespace al
