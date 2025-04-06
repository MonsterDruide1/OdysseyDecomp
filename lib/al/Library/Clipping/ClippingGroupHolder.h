#pragma once

namespace al {
class ClippingActorInfo;
class ClippingJudge;

class ClippingGroupHolder {
public:
    void allocBuffer();
    ClippingGroupHolder();
    void createAndCount(ClippingActorInfo*);
    void leave(ClippingActorInfo*);
    void reentry(ClippingActorInfo*);
    void registerInfo(ClippingActorInfo*);
    void tryFindGroup(const ClippingActorInfo*);
    void update(const ClippingJudge*);
};

} // namespace al
