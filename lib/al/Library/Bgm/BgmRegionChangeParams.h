#pragma once

#include <basis/seadTypes.h>

namespace al {
struct BgmRegionChangeParams {
    BgmRegionChangeParams();

    BgmRegionChangeParams(const BgmRegionChangeParams& other);

    void operator=(const BgmRegionChangeParams& other);

    bool operator==(const BgmRegionChangeParams& other) const;

    s32 _0 = 0;
    s32 _4 = 0;
    s32 _8 = 0;
    bool _c = false;
    bool _d = false;
};

}  // namespace al
