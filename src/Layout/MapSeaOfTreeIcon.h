#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}  // namespace al

class MapSeaOfTreeIcon : public al::LayoutActor {
public:
    enum SeaOfTreeIconType : s32 {
        Value_0 = 0,
        Value_1 = 1,
        Value_2 = 2,
    };

    MapSeaOfTreeIcon(const al::LayoutInitInfo& initInfo);

    void appearHint0Mode();
    void appearHint1Mode();
    void appearHint2Mode();
    void end();
    bool trySetIconType(SeaOfTreeIconType iconType);
    void resetIconType();
    void getHint1LayoutPos(sead::Vector3f* trans) const;
    void getHint2LayoutPos(sead::Vector3f* trans) const;
    void getPlayerLayoutPos(sead::Vector3f* trans) const;
    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeDead();

private:
    s32 mCurrentIconType = Value_2;
    s32 mNextIconType = Value_2;
};

static_assert(sizeof(MapSeaOfTreeIcon) == 0x138);
