#pragma once

#include <basis/seadTypes.h>

namespace al {
class PlacementId;
class ClippingJudge;
<<<<<<< HEAD

class ClippingActorInfo;
=======
struct ClippingActorInfo;
>>>>>>> a77f7900 (Implemented: ClippingGroupHolder)

struct ClippingInfoGroup {
    ClippingInfoGroup();
    void registerInfo(ClippingActorInfo* clippingActorInfo);
    void removeInfo(ClippingActorInfo* clippingActorInfo);
    void addCount();
    void allocBuffer();
    void setGroupId(const ClippingActorInfo* clippingActorInfo);
    bool isEqualGroupId(const PlacementId* placementId) const;
    bool judgeClippingAll(const ClippingJudge* clippingJudge) const;
    void startClippedAll();
    void endClippedAll();

    s32 count = 0;
    s32 size = 0;
    ClippingActorInfo** clippingInfos = nullptr;
    PlacementId* groupId;
    bool isClipped = false;
};

class ClippingGroupHolder {
public:
    ClippingGroupHolder();
    void update(const ClippingJudge* clippingJudge);
    void createAndCount(ClippingActorInfo* clippingActorInfo);
    ClippingInfoGroup* tryFindGroup(const ClippingActorInfo* clippingActorInfo);
    void allocBuffer();
    void registerInfo(ClippingActorInfo* clippingActorInfo);
    void leave(ClippingActorInfo* clippingActorInfo);
    void reentry(ClippingActorInfo* clippingActorInfo);

private:
    s32 mGroupCount = 0;
    ClippingInfoGroup** mGroups = nullptr;
};

}  // namespace al
