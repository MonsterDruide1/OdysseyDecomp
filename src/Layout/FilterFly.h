#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class FilterFly : public al::LayoutActor {
public:
    FilterFly(const char*, const al::LayoutInitInfo&, const char*);
    void exeWait();
    void exeWaitEnd();
    void exeMove();
    void move(const sead::Vector2f&);
    bool isWaitEnd() const;

private:
    sead::Vector2f mTargetPos = sead::Vector2f::zero;
    sead::Vector2f mVelocity = sead::Vector2f::zero;
};
