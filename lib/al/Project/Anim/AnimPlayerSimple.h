#pragma once

#include <basis/seadTypes.h>

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
    f32 getAnimFrame() const;
    void setAnimFrame(f32);
    f32 getAnimFrameMax() const;
    f32 getAnimFrameMax(const char*) const;
    f32 getAnimFrameRate() const;
    void setAnimFrameRate(f32);
    bool isAnimExist(const char*) const;
    bool isAnimEnd() const;
    bool isAnimOneTime() const;
    bool isAnimOneTime(const char*) const;
    bool isAnimPlaying() const;
    const char* getPlayingAnimName() const;

private:
    void* _18;
    const char* mPlayingAnimName;
};

static_assert(sizeof(AnimPlayerSimple) == 0x28);

}  // namespace al
