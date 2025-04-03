#include "Project/Stage/StageSwitchAccesser.h"

#include "Library/Base/StringUtil.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Stage/StageSwitchDirector.h"

namespace al {

StageSwitchAccesser::StageSwitchAccesser() = default;

bool StageSwitchAccesser::init(StageSwitchDirector* director, const char* linkName,
                               const PlacementId& placementId) {
    mStageSwitchDirector = director;
    mLinkName = linkName;
    mPlacementId = new PlacementId(placementId);

    if (isMatchString(linkName, MatchStr{"*On"}) || isMatchString(linkName, MatchStr{"*Off"}))
        mSwitchKind = SwitchKind::Write;
    else
        mSwitchKind = SwitchKind::Read;
    mInfoIndex = mStageSwitchDirector->useSwitch(this);
    return isValid();
}

StageSwitchDirector* StageSwitchAccesser::getStageSwitchDirector() const {
    return mStageSwitchDirector;
}

bool StageSwitchAccesser::isValid() const {
    return mInfoIndex >= 0;
}

void StageSwitchAccesser::onSwitch() {
    if (isValid())
        mStageSwitchDirector->onSwitch(this);
}

void StageSwitchAccesser::offSwitch() {
    if (isValid())
        mStageSwitchDirector->offSwitch(this);
}

bool StageSwitchAccesser::isOnSwitch() const {
    return mStageSwitchDirector->isOnSwitch(this);
}

bool StageSwitchAccesser::isEnableRead() const {
    return mSwitchKind == SwitchKind::Read || mSwitchKind == SwitchKind::Write;
}

bool StageSwitchAccesser::isEnableWrite() const {
    return mSwitchKind == SwitchKind::Write;
}

bool StageSwitchAccesser::isEqualSwitch(const StageSwitchAccesser* other) const {
    if (!other)
        return false;

    return mInfoIndex == other->mInfoIndex;
}

void StageSwitchAccesser::addListener(StageSwitchListener* listener) {
    mStageSwitchDirector->addListener(listener, this);
}

}  // namespace al
