#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadStorageFor.h>  // IWYU pragma: export

namespace al {
class Triangle;
}  // namespace al

bool operator==(const al::Triangle& tri1, const al::Triangle& tri2);
bool operator!=(const al::Triangle& tri1, const al::Triangle& tri2);

namespace al {
class ByamlIter;
class CollisionParts;
class HitSensor;
class LiveActor;
struct KCPrismData;
struct KCPrismHeader;

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
    void getLocalPos(sead::Vector3f* localPos, s32 index) const;
    void calcForceMovePower(sead::Vector3f* movePower, const sead::Vector3f& pos) const;
    void calcForceRotatePower(sead::Quatf* rotatePower) const;
    bool getAttributes(ByamlIter* iter) const;
    const HitSensor* getSensor() const;
    const sead::Matrix34f& getBaseMtx() const;
    const sead::Matrix34f& getBaseInvMtx() const;
    const sead::Matrix34f& getPrevBaseMtx() const;

    // clang-format off
    friend bool ::operator==(const Triangle& tri1, const Triangle& tri2);
    friend bool ::operator!=(const Triangle& tri1, const Triangle& tri2);
    // clang-format on

private:
    const CollisionParts* mCollisionParts;
    const KCPrismData* mKCPrismData;
    const KCPrismHeader* mKCPrismHeader;
    sead::Vector3f mNormals[4];  // FaceNormal, then three EdgeNormals
    sead::Vector3f mPositions[3];
};

enum class CollisionLocation : u8 {
    None = 0,
    Face = 1,
    Edge1 = 2,
    Edge2 = 3,
    Edge3 = 4,
    Corner1 = 5,
    Corner2 = 6,
    Corner3 = 7,
};

struct HitInfo {
    HitInfo();

    bool isCollisionAtFace() const;
    bool isCollisionAtEdge() const;
    bool isCollisionAtCorner() const;
    const sead::Vector3f& tryGetHitEdgeNormal() const;

    Triangle triangle;
    f32 _70 = 0.0f;
    sead::Vector3f collisionHitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _80 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f collisionMovingReaction = {0.0f, 0.0f, 0.0f};
    CollisionLocation collisionLocation = CollisionLocation::None;
};

struct ArrowHitInfo {
    HitInfo* operator*() { return hitInfo.data(); }

    const HitInfo* operator*() const { return hitInfo.data(); }

    HitInfo& operator->() { return *hitInfo; }

    const HitInfo& operator->() const { return *hitInfo; }

    sead::StorageFor<HitInfo> hitInfo{sead::ZeroInitializeTag{}};
};

struct SphereHitInfo {
    void calcFixVector(sead::Vector3f* a1, sead::Vector3f* a2) const;
    void calcFixVectorNormal(sead::Vector3f* a1, sead::Vector3f* a2) const;

    HitInfo* operator*() { return hitInfo.data(); }

    const HitInfo* operator*() const { return hitInfo.data(); }

    HitInfo& operator->() { return *hitInfo; }

    const HitInfo& operator->() const { return *hitInfo; }

    sead::StorageFor<HitInfo> hitInfo{sead::ZeroInitializeTag{}};
};

struct DiskHitInfo {
    void calcFixVector(sead::Vector3f* a1, sead::Vector3f* a2) const;
    void calcFixVectorNormal(sead::Vector3f* a1, sead::Vector3f* a2) const;

    HitInfo* operator*() { return hitInfo.data(); }

    const HitInfo* operator*() const { return hitInfo.data(); }

    HitInfo& operator->() { return *hitInfo; }

    const HitInfo& operator->() const { return *hitInfo; }

    sead::StorageFor<HitInfo> hitInfo{sead::ZeroInitializeTag{}};
};

}  // namespace al
