#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "al/LiveActor/LiveActor.h"

namespace al {

class PartsModel : public al::LiveActor {
public:
    PartsModel(const char*);
    void initPartsDirect(al::LiveActor*, const al::ActorInitInfo&, char const*,
                         const sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&,
                         const sead::Vector3f&, bool);
    void initPartsSuffix(al::LiveActor*, const al::ActorInitInfo&, char const*, char const*,
                         const sead::Matrix34f*, bool);
    void initPartsMtx(al::LiveActor*, const al::ActorInitInfo&, char const*, const sead::Matrix34f*,
                      bool);
    void initPartsFixFile(al::LiveActor*, const al::ActorInitInfo&, char const*, char const*,
                          char const*);
    void initPartsFixFileNoRegister(al::LiveActor*, const al::ActorInitInfo&, char const*,
                                    char const*, char const*);
    void endClipped() override;
    void updatePose();
    void calcAnim() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    void offSyncAppearAndHide();
    void onSyncAppearAndHide();

private:
    al::LiveActor* mLiveActor;
    sead::Matrix34f* _110;
    bool mUseLocalTransforms;
    sead::Vector3f mLocalTrans;
    sead::Vector3f mLocalRotate;
    sead::Vector3f mLocalScale;
    bool mUseFollowMtxScale;
    bool mUseLocalScale;
    bool mUpdate;
};

}  // namespace al
