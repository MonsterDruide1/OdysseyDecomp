#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}  // namespace al

class ShineCounter : public al::LayoutActor {
public:
    ShineCounter(const char* name, const al::LayoutInitInfo& info);

    void kill() override;

    void tryStart();
    void tryStartWait();
    void tryEnd();
    void startCountAnim(bool isAddTenShines);
    bool isEndCountAnim() const;

    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeShineCountAppear();
    void exeShineCountWait();
    void exeShineCountAdd();

private:
    s32 mCurrentShineNum = 0;
};

static_assert(sizeof(ShineCounter) == 0x130);
