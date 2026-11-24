#include "Enemy/GotogotonMark.h"

GotogotonMark::GotogotonMark() {}

bool GotogotonMark::isMatch(const GotogotonMark* other) const {
    if (!other)
        return false;

    if (mId == 0 || other->mId == 0 || mId != other->mId)
        return false;

    if ((mId & 0x80000000) == 0)
        return true;

    return mFrontDir.dot(other->mFrontDir) >= 0.8f;
}
