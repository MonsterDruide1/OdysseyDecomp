#pragma once

#include <nn/ui2d/Layout.h>

namespace eui {
class Animator;
}

namespace al {

class LayoutPaneGroup {
public:
    LayoutPaneGroup(const char* groupName);
    void startAnim(const char* animName);
    eui::Animator* getAnimator(const char* animName) const;
    void setAnimFrame(f32 frame);
    void setAnimFrameRate(f32 frameRate);
    f32 getAnimFrame() const;
    f32 getAnimFrameMax() const;
    f32 getAnimFrameMax(const char* animName) const;
    f32 getAnimFrameRate() const;
    bool isAnimExist(const char* animName) const;
    eui::Animator* tryGetAnimator(const char* animName) const;
    bool isAnimEnd() const;
    bool isAnimOneTime() const;
    bool isAnimOneTime(const char* animName) const;
    bool isAnimPlaying() const;
    const char* getPlayingAnimName() const;
    void pushAnimName(const char* animName);
    void createAnimator(nn::ui2d::Layout* layout);
    void animate(bool);

private:
    void* filler[5];
};

}  // namespace al
