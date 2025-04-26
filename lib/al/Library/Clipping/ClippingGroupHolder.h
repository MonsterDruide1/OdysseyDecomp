#pragma once

namespace al {
class ClippingInfoGroup;
class ClippingActorInfo;
class ClippingJudge;

class ClippingGroupHolder {
public:
    ClippingGroupHolder();
    void update(const ClippingJudge*);
    void createAndCount(ClippingActorInfo*);
    ClippingInfoGroup* tryFindGroup(const ClippingActorInfo*);
    void allocBuffer();
    void registerInfo(ClippingActorInfo*);
    void leave(ClippingActorInfo*);
    void reentry(ClippingActorInfo*);
};

}  // namespace al
