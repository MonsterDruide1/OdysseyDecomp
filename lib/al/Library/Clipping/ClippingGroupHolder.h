#pragma once

namespace al {
struct ClippingActorInfo;
class ClippingInfoGroup;
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

private:
    void* filler[0x2];
};

}  // namespace al
