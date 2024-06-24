#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FilterFly : public al::LayoutActor {
public:
    FilterFly(const char* name, const al::LayoutInitInfo& info, const char* suffix);
    void exeWait();
    void exeWaitEnd();
    void exeMove();
    void move(const sead::Vector2f& target);
    bool isWaitEnd() const;

private:
    sead::Vector2f mTargetPos = sead::Vector2f::zero;
    sead::Vector2f mVelocity = sead::Vector2f::zero;
};
