#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}

class TalkNpcCap : public al::LiveActor {
public:
    static TalkNpcCap* tryCreate(const al::LiveActor*, const al::ActorInitInfo&);
    static TalkNpcCap* createForAchievementNpc(const al::LiveActor*, const al::ActorInitInfo&);
    static TalkNpcCap* createForHintNpc(const al::LiveActor*, const al::ActorInitInfo&);
    static TalkNpcCap* createForShibaken(const al::LiveActor*, const al::ActorInitInfo&);
    static TalkNpcCap* createForShoppingNpc(const al::LiveActor*, const al::ActorInitInfo&);
    static TalkNpcCap* createForShoppingNpcChromakey(const al::LiveActor*,
                                                     const al::ActorInitInfo&);
    static TalkNpcCap* createForVolleyballNpc(const al::LiveActor*, const al::ActorInitInfo&);

    void initAttach(const al::LiveActor*);
    void makeActorAlive() override;
    void control() override;
    void init(const al::ActorInitInfo& info) override;

private:
    const char* _108;
    sead::Matrix34f* _110;
    sead::Vector3f _118;
    sead::Vector3f _124;
    f32 _130;
    bool _134;
};

static_assert(sizeof(TalkNpcCap) == 0x138);
