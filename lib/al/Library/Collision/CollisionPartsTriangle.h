#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

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
class KCPrismData;
class KCPrismHeader;
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
    void getLocalPos(sead::Vector3f* localPos, s32 index) const;
    void calcForceMovePower(sead::Vector3f* movePower, const sead::Vector3f& pos) const;
    void calcForceRotatePower(sead::Quatf* rotatePower) const;
    bool getAttributes(ByamlIter* iter) const;
    const HitSensor* getSensor() const;
    const sead::Matrix34f& getBaseMtx() const;
    const sead::Matrix34f& getBaseInvMtx() const;
    const sead::Matrix34f& getPrevBaseMtx() const;

    friend bool ::operator==(const Triangle & tri1, const Triangle & tri2);
    friend bool ::operator!=(const Triangle & tri1, const Triangle & tri2);

private:
    const CollisionParts* mCollisionParts;
    const KCPrismData* mKCPrismData;
    const KCPrismHeader* mKCPrismHeader;
    sead::Vector3f mNormals[4];  // FaceNormal, then three EdgeNormals
    sead::Vector3f mPositions[3];
};

}  // namespace al
