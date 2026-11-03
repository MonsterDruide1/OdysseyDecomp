#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadStorageFor.h>

#include "Library/Collision/KCollisionServer.h"

namespace al {
class Triangle;
class KCollisionServer;
}  // namespace al

bool operator==(const al::Triangle& tri1, const al::Triangle& tri2);
bool operator!=(const al::Triangle& tri1, const al::Triangle& tri2);

namespace al {
class ByamlIter;
class CollisionParts;
class HitSensor;
struct KCPrismData;
struct KCPrismHeader;
class LiveActor;

class Triangle {
public:
    Triangle();
    Triangle(const CollisionParts& parts, const KCPrismData* data, const KCPrismHeader* header);

    void fillData(const CollisionParts& parts, const KCPrismData* data,
                  const KCPrismHeader* header);
    void fill(const sead::Vector3f& pos1, const sead::Vector3f& pos2, const sead::Vector3f& pos3);

    const LiveActor* getHostActor() const;

    bool isHostMoved() const;
    bool isValid() const;

    const sead::Vector3f& getNormal(s32 index) const;
    const sead::Vector3f& getFaceNormal() const;
    const sead::Vector3f& getEdgeNormal(s32 index) const;
    const sead::Vector3f& getPos(s32 index) const;

    void calcCenterPos(sead::Vector3f* center) const;
    const sead::Vector3f& calcAndGetNormal(s32 index);
    const sead::Vector3f& calcAndGetFaceNormal();
    const sead::Vector3f& calcAndGetEdgeNormal(s32 index);
    const sead::Vector3f& calcAndGetPos(s32 index);

    void getLocalPos(sead::Vector3f* pos, s32 index) const;

    void calcForceMovePower(sead::Vector3f* power, const sead::Vector3f& pos) const;
    void calcForceRotatePower(sead::Quatf* power) const;

    bool getAttributes(ByamlIter* iter) const;

    const HitSensor* getSensor() const;
    const sead::Matrix34f& getBaseMtx() const;
    const sead::Matrix34f& getBaseInvMtx() const;
    const sead::Matrix34f& getPrevBaseMtx() const;

    const CollisionParts* getParts() const { return mParts; }

    const KCPrismData* getPrismData() const { return mData; }

    const KCPrismHeader* getPrismHeader() const { return mHeader; }

    // clang-format off
    friend bool ::operator==(const Triangle& lhs, const Triangle& rhs);
    friend bool ::operator!=(const Triangle& lhs, const Triangle& rhs);
    // clang-format on

private:
    const CollisionParts* mParts;
    const KCPrismData* mData;
    const KCPrismHeader* mHeader;

    sead::Vector3f mNormals[4];  // FaceNormal, then three EdgeNormals
    sead::Vector3f mPositions[3];
};

struct HitInfo {
    HitInfo();

    bool isCollisionAtFace() const;
    bool isCollisionAtEdge() const;
    bool isCollisionAtCorner() const;

    const sead::Vector3f& tryGetHitEdgeNormal() const;

    Triangle triangle;
    f32 distance = 0.0f;
    sead::Vector3f hitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f checkPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f movingReaction = {0.0f, 0.0f, 0.0f};
    u8 location = KCLocation_None;
};

struct ArrowHitInfo : public HitInfo {};

struct SphereHitInfo : public HitInfo {
    void calcFixVector(sead::Vector3f* a1, sead::Vector3f* a2) const;
    void calcFixVectorNormal(sead::Vector3f* a1, sead::Vector3f* a2) const;
};

struct DiskHitInfo : public HitInfo {
    void calcFixVector(sead::Vector3f* a1, sead::Vector3f* a2) const;
    void calcFixVectorNormal(sead::Vector3f* a1, sead::Vector3f* a2) const;
};

}  // namespace al
