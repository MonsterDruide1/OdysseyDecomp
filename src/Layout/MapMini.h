#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class PlayerHolder;
}  // namespace al

class MapMini : public al::LayoutActor {
public:
    MapMini(const al::LayoutInitInfo& info, const al::PlayerHolder* playerHolder);

    void appearSlideIn();
    void end();
    bool isEnd() const;

    void exeAppear();
    void exeWait();
    void calcNearHintTrans();
    void exeEnd();

private:
    const al::PlayerHolder* mPlayerHolder;
    f32 mCameraAngle = 0.0f;
    al::LayoutActor* mPlayerIconLayout = nullptr;
    sead::PtrArray<al::LayoutActor> mHintIconLayouts;
    s32 mHintIconNum = 0;
};

static_assert(sizeof(MapMini) == 0x160);
