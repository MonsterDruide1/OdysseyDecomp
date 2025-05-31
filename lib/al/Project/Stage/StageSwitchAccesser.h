#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseName;
class StageSwitchListener;
class StageSwitchDirector;
class PlacementId;

class StageSwitchAccesser {
public:
    StageSwitchAccesser();

    bool init(StageSwitchDirector* director, const char* linkName, const PlacementId& placementId);
    StageSwitchDirector* getStageSwitchDirector() const;
    bool isValid() const;
    void onSwitch();
    void offSwitch();
    bool isOnSwitch() const;
    bool isEnableRead() const;
    bool isEnableWrite() const;
    bool isEqualSwitch(const StageSwitchAccesser* other) const;
    void addListener(StageSwitchListener* listener);

    const char* getLinkName() const { return mLinkName; }

    void setUseName(IUseName* useName) { mUseName = useName; }

    PlacementId* getPlacementId() const { return mPlacementId; }

    s32 getInfoIndex() const { return mInfoIndex; }

private:
    enum class SwitchKind : u32 { Invalid = 0, Read = 1, Write = 2 };

    StageSwitchDirector* mStageSwitchDirector = nullptr;
    IUseName* mUseName = nullptr;
    const char* mLinkName = "";
    PlacementId* mPlacementId = nullptr;
    s32 mInfoIndex = -1;
    SwitchKind mSwitchKind = SwitchKind::Invalid;
};
}  // namespace al
