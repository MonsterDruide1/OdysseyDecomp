#pragma once

namespace al {
class PlacementId;

struct StageSwitchInfo {
    StageSwitchInfo();

    PlacementId* placementId = nullptr;
    bool isSwitchOn = false;
};
}  // namespace al
