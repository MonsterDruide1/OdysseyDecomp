#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class Triangle;
class KCollisionServer;
}

bool operator==(const al::Triangle&, const al::Triangle&);
bool operator!=(const al::Triangle&, const al::Triangle&);

namespace al {
class ByamlIter;
class CollisionParts;
class HitSensor;
class KCPrismData;
class KCPrismHeader;
class LiveActor;
class Triangle;

class Triangle {
public:
    Triangle();
    Triangle(const CollisionParts&, const KCPrismData*, const KCPrismHeader*);

    void fillData(const CollisionParts&, const KCPrismData*, const KCPrismHeader*);
    void fill(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
    const LiveActor* getHostActor() const;
    bool isHostMoved() const;
    bool isValid() const;
    const sead::Vector3f& getNormal(int) const;
    const sead::Vector3f& getFaceNormal() const;
    const sead::Vector3f& getEdgeNormal(int) const;
    const sead::Vector3f& getPos(int) const;
    void calcCenterPos(sead::Vector3f*) const;
    const sead::Vector3f& calcAndGetNormal(int);
    const sead::Vector3f& calcAndGetFaceNormal();
    const sead::Vector3f& calcAndGetEdgeNormal(int);
    const sead::Vector3f& calcAndGetPos(int);
    void getLocalPos(sead::Vector3f*, int) const;
    void calcForceMovePower(sead::Vector3f*, const sead::Vector3f&) const;
    void calcForceRotatePower(sead::Quatf*) const;
    bool getAttributes(ByamlIter*) const;
    const HitSensor* getSensor() const;
    const sead::Matrix34f& getBaseMtx() const;
    const sead::Matrix34f& getBaseInvMtx() const;
    const sead::Matrix34f& getPrevBaseMtx() const;

    friend bool ::operator==(const Triangle&, const Triangle&);
    friend bool ::operator!=(const Triangle&, const Triangle&);

public:
    const CollisionParts* mCollisionParts;
    const KCPrismData* mKCPrismData;
    const KCPrismHeader* mKCPrismHeader;
    sead::Vector3f mFaceNormal;
    sead::Vector3f mEdgeNormals[3];
    sead::Vector3f mVerts[3];
};

class HitInfo {
public:
    HitInfo();

    bool isCollisionAtFace() const;
    
public:
    al::Triangle mTriangle;
    f32 unk = 0.0f;
    sead::Vector3f mCollisionHitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f unk3 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mCollisionMovingReaction = {0.0f, 0.0f, 0.0f};
    u8 mCollisionLocation = 0;
};
static_assert(sizeof(HitInfo) == 0xA0);

class ArrowHitInfo : public HitInfo {};

class DiskHitInfo : public HitInfo {};

class SphereHitInfo : public HitInfo {
public:
    void calcFixVector(sead::Vector3f*, sead::Vector3f*) const;
    void calcFixVectorNormal(sead::Vector3f*, sead::Vector3f*) const;
};

}  // namespace al

namespace alKCollisionFunc {

void calcSphereHitPos(sead::Vector3<float> *,al::KCollisionServer const*,sead::Vector3<float> const&,al::KCPrismData const&,al::KCPrismHeader const*,u8);

}
