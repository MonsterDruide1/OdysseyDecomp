#pragma once

#include <math/seadVector.h>

#include "Library/Collision/IUseCollision.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class CollisionDirector;

class CameraTargetCollideInfoHolder : public IUseCollision, public IUseHioNode {
public:
    CameraTargetCollideInfoHolder();
    void update(bool, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
    void reset();
    bool isExistUnderWall() const;
    bool tryCalcSlopDownFrontDirH(sead::Vector3f*) const;
    CollisionDirector* getCollisionDirector() const override;

    bool isExistCollisionUnderTarget() const { return mIsExistCollisionUnderTarget; }

    const sead::Vector3f& getTargetCollisionPos() const { return mTargetCollisionPos; }

    const sead::Vector3f& getTargetCollisionNormal() const { return mTargetCollisionNormal; }

    bool isExistSlopeCollisionUnderTarget() const { return mIsExistSlopeCollisionUnderTarget; }

    f32 getSlopeCollisionUpSpeed() const { return mSlopeCollisionUpSpeed; }

    f32 getSlopeCollisionDownSpeed() const { return mSlopeCollisionDownSpeed; }

private:
    void* mBuffer;
    char mBuffer2;
    bool mIsExistCollisionUnderTarget;
    void* mBuffer3;
    sead::Vector3f mTargetCollisionNormal;
    sead::Vector3f mTargetCollisionPos;
    bool mIsExistSlopeCollisionUnderTarget;
    char mBuffer4[0xc];
    f32 mSlopeCollisionDownSpeed;
    f32 mSlopeCollisionUpSpeed;
};

}  // namespace al
