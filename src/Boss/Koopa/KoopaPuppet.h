#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
}  // namespace al

class Koopa;

class KoopaPuppet {
public:
    KoopaPuppet(Koopa* koopa);

    void start(al::HitSensor* sensor);
    void end();
    void endAndPopUp(const sead::Vector3f& velocity);
    bool isBindWait() const;
    void change2D();
    void changeHackStart(const sead::Vector3f& startTrans, const sead::Matrix34f* bindMtx);
    f32 getBindHackStartActionFrame() const;
    f32 getBindHackStartActionFrameMax() const;
    const sead::Vector3f& getTrans() const;
    void calcQuat(sead::Quatf* quat) const;
    void setTrans(const sead::Vector3f& trans);
    void resetPosition(const sead::Vector3f& pos);
    void updatePoseQuat(const sead::Quatf& quat);
    void startAction(const char* actionName);
    bool calcPuppetMoveDir(sead::Vector3f* dir, const sead::Vector3f& up) const;
    bool isTriggerSwing() const;
    void hideModel();
    void showModel();
    void hideSilhouetteModel();
    void showSilhouetteModel();
    void changeHackTutorialElectricWire();
    void closeHackTutorial();

private:
    Koopa* mKoopa;
    al::HitSensor* mHitSensor = nullptr;
    const sead::Matrix34f* mBindMtx = nullptr;
};

static_assert(sizeof(KoopaPuppet) == 0x18);

namespace rs {
KoopaPuppet* tryStartKoopaPuppet(al::HitSensor* receiver, al::HitSensor* sender);
void endKoopaPuppet(KoopaPuppet** puppet);
void endKoopaPuppetAndPopUp(KoopaPuppet** puppet, const sead::Vector3f& velocity);
}  // namespace rs
