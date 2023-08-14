#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include "Library/LiveActor/LiveActor.h"

namespace al {

class PartsModel : public LiveActor {
public:
    PartsModel(const char*);
    void endClipped() override;
    void calcAnim() override;
    void attackSensor(HitSensor*, HitSensor*) override;
    bool receiveMsg(SensorMsg const*, HitSensor*, HitSensor*) override;

    void initPartsDirect(LiveActor*, const ActorInitInfo&, const char*, const sead::Matrix34f*,
                         const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, bool);
    void initPartsSuffix(LiveActor*, const ActorInitInfo&, const char*, const char*,
                         sead::Matrix34f const*, bool);
    void initPartsMtx(LiveActor*, const ActorInitInfo&, const char*, const sead::Matrix34f*, bool);
    void initPartsFixFile(LiveActor*, const ActorInitInfo&, const char*, const char*, const char*);
    void initPartsFixFileNoRegister(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                    const char*);

    void updatePose();
    void offSyncAppearAndHide();
    void onSyncAppearAndHide();

private:
    LiveActor* mParentModel = nullptr;
    const sead::Matrix34f* mJointMtx = nullptr;
    bool mIsUseLocalPos = false;
    sead::Vector3f mLocalTrans = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f mLocalRotate = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f mLocalScale = sead::Vector3f(1.0f, 1.0f, 1.0f);
    bool mIsUseFollowMtxScale = false;
    bool mIsUseLocalScale = false;
    bool mUpdate = true;
};

}  // namespace al
