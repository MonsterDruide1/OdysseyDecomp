#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {

class PartsModel : public LiveActor {
public:
    PartsModel(const char* name);
    void endClipped() override;
    void calcAnim() override;
    void attackSensor(HitSensor* self, HitSensor* other) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void initPartsDirect(LiveActor* parent, const ActorInitInfo& initInfo, const char* arcName,
                         const sead::Matrix34f* jointMtx, const sead::Vector3f& localTrans,
                         const sead::Vector3f& localRotate, const sead::Vector3f& localScale,
                         bool useFollowMtxScale);
    void initPartsSuffix(LiveActor* parent, const ActorInitInfo& initInfo, const char* arcName,
                         const char* suffix, const sead::Matrix34f* jointMtx,
                         bool useFollowMtxScale);
    void initPartsMtx(LiveActor* parent, const ActorInitInfo& initInfo, const char* arcName,
                      const sead::Matrix34f* jointMtx, bool useFollowMtxScale);
    void initPartsFixFile(LiveActor* parent, const ActorInitInfo& initInfo, const char* arcName,
                          const char* arcSuffix, const char* suffix);
    void initPartsFixFileNoRegister(LiveActor* parent, const ActorInitInfo& initInfo,
                                    const char* arcName, const char* arcSuffix, const char* suffix);

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
    bool mIsUpdate = true;
};

}  // namespace al
