#pragma once

namespace al {
class ClippingJudge;
class PlacementId;
class ClippingActorInfo;

class ClippingInfoGroup {
public:
    ClippingInfoGroup();
    void registerInfo(ClippingActorInfo*);
    void removeInfo(ClippingActorInfo*);
    void addCount();
    void allocBuffer();
    void setGroupId(const ClippingActorInfo*);
    bool isEqualGroupId(const PlacementId*) const;
    bool judgeClippingAll(const ClippingJudge*) const;
    void startClippedAll();
    void endClippedAll();
};
} // namespace al
