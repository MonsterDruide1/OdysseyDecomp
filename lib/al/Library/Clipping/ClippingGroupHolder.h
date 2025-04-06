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

private:
    void* filler[0x2];
};

}  // namespace al
