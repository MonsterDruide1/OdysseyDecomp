#include "Library/Bgm/BgmRegionChangeParams.h"

namespace al {
BgmRegionChangeParams::BgmRegionChangeParams() {}

BgmRegionChangeParams::BgmRegionChangeParams(const BgmRegionChangeParams& other)
    : _0(other._0), _4(other._4), _8(other._8), _c(other._c), _d(other._d) {
    *this = other;
}

void BgmRegionChangeParams::operator=(const BgmRegionChangeParams& other) {
    _0 = other._0;
    _4 = other._4;
    _8 = other._8;
    _c = other._c;
    _d = other._d;
}

bool BgmRegionChangeParams::operator==(const BgmRegionChangeParams& other) const {
    if (_0 == other._0 && _4 == other._4 && _8 == other._8 && _c == other._c && _d == other._d)
        return true;
    return false;
}

}  // namespace al
