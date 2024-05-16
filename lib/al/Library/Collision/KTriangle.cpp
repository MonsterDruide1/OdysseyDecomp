#include "Library/Collision/KTriangle.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathLengthUtil.h"
#include "math/seadMatrix.h"
#include "math/seadVectorFwd.h"

namespace al {

Triangle::Triangle() : mCollisionParts(nullptr), mKCPrismData(nullptr), mKCPrismHeader(nullptr) {
    mFaceNormal = sead::Vector3f::zero;
    mEdgeNormals[0] = sead::Vector3f::zero;
    mEdgeNormals[1] = sead::Vector3f::zero;
    mEdgeNormals[2] = sead::Vector3f::zero;
    mVerts[0] = sead::Vector3f::zero;
    mVerts[1] = sead::Vector3f::zero;
    mVerts[2] = sead::Vector3f::zero;
}

Triangle::Triangle(const CollisionParts& parts, const KCPrismData* data,
                   const KCPrismHeader* header) {
    fillData(parts, data, header);
}

void Triangle::fillData(const CollisionParts& parts, const KCPrismData* data,
                        const KCPrismHeader* header) {
    mCollisionParts = &parts;
    mKCPrismData = data;
    mKCPrismHeader = header;

    const KCollisionServer& kCollisionServer = parts.getKCollisionServer();
    const sead::Matrix34f& baseMtx = parts.getBaseMtx();

    mFaceNormal.setRotated(baseMtx, kCollisionServer.getFaceNormal(data, mKCPrismHeader));
    mEdgeNormals[0].setRotated(baseMtx, kCollisionServer.getEdgeNormal1(data, mKCPrismHeader));
    mEdgeNormals[1].setRotated(baseMtx, kCollisionServer.getEdgeNormal2(data, mKCPrismHeader));
    mEdgeNormals[2].setRotated(baseMtx, kCollisionServer.getEdgeNormal3(data, mKCPrismHeader));

    normalize(&mFaceNormal);
    normalize(&mEdgeNormals[0]);
    normalize(&mEdgeNormals[1]);
    normalize(&mEdgeNormals[2]);

    kCollisionServer.calcPosLocal(&mVerts[0], data, 0, mKCPrismHeader);
    kCollisionServer.calcPosLocal(&mVerts[1], data, 1, mKCPrismHeader);
    kCollisionServer.calcPosLocal(&mVerts[2], data, 2, mKCPrismHeader);

    mVerts[0].setMul(baseMtx, mVerts[0]);
    mVerts[1].setMul(baseMtx, mVerts[1]);
    mVerts[2].setMul(baseMtx, mVerts[2]);
}

// NON_MATCHING: reordering instructions within calculation of mEdgeNormals[1]
void Triangle::fill(const sead::Vector3f& vert1, const sead::Vector3f& vert2,
                    const sead::Vector3f& vert3) {
    mVerts[0] = vert1;
    mVerts[1] = vert2;
    mVerts[2] = vert3;

    sead::Vector3f vert1To2 = vert2 - vert1;
    sead::Vector3f vert1To3 = vert3 - vert1;
    sead::Vector3f vert2To3 = vert3 - vert2;

    mFaceNormal.setCross(vert1To2, vert1To3);
    tryNormalizeOrZero(&mFaceNormal);

    mEdgeNormals[0].setCross(vert1To2, mFaceNormal);
    tryNormalizeOrZero(&mEdgeNormals[0]);
    mEdgeNormals[1].setCross(vert1To3, mFaceNormal);
    tryNormalizeOrZero(&mEdgeNormals[1]);
    mEdgeNormals[2].setCross(vert2To3, mFaceNormal);
    tryNormalizeOrZero(&mEdgeNormals[2]);

    mKCPrismData = nullptr;
}

const LiveActor* Triangle::getHostActor() const {
    if (!mCollisionParts)
        return nullptr;
    return mCollisionParts->getConnectedHost();
}

bool Triangle::isHostMoved() const {
    return mCollisionParts->getSomeCounter() == 0 || mCollisionParts->isMoving();
}

bool Triangle::isValid() const {
    return mKCPrismData != nullptr;
}

const sead::Vector3f& Triangle::getNormal(int index) const {
    return *(&mFaceNormal + index);
}

const sead::Vector3f& Triangle::getFaceNormal() const {
    return mFaceNormal;
}

const sead::Vector3f& Triangle::getEdgeNormal(int index) const {
    return mEdgeNormals[index];
}

const sead::Vector3f& Triangle::getPos(int index) const {
    return mVerts[index];
}

void Triangle::calcCenterPos(sead::Vector3f* center) const {
    *center = (mVerts[0] + mVerts[1] + mVerts[2]) * 0.33333f;
}

const sead::Vector3f& Triangle::calcAndGetNormal(int index) {
    const CollisionParts* collisionParts = mCollisionParts;
    const KCollisionServer& kCollisionServer = collisionParts->getKCollisionServer();
    switch (index) {
    case 0:
        mFaceNormal = kCollisionServer.getFaceNormal(mKCPrismData, mKCPrismHeader);
        mFaceNormal.setRotated(collisionParts->getBaseMtx(), mFaceNormal);
        normalize(&mFaceNormal);
        return mFaceNormal;
    case 1:
        mEdgeNormals[0] = kCollisionServer.getEdgeNormal1(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[0].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[0]);
        normalize(&mEdgeNormals[0]);
        return mEdgeNormals[0];
    case 2:
        mEdgeNormals[1] = kCollisionServer.getEdgeNormal2(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[1].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[1]);
        normalize(&mEdgeNormals[1]);
        return mEdgeNormals[1];
    case 3:
        mEdgeNormals[2] = kCollisionServer.getEdgeNormal3(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[2].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[2]);
        normalize(&mEdgeNormals[2]);
        return mEdgeNormals[2];
        break;
    default:
        return getNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetFaceNormal() {
    const CollisionParts* collisionParts = mCollisionParts;
    mFaceNormal = collisionParts->getKCollisionServer().getFaceNormal(mKCPrismData, mKCPrismHeader);
    mFaceNormal.setRotated(collisionParts->getBaseMtx(), mFaceNormal);
    normalize(&mFaceNormal);
    return mFaceNormal;
}

const sead::Vector3f& Triangle::calcAndGetEdgeNormal(int index) {
    const CollisionParts* collisionParts = mCollisionParts;
    const KCollisionServer& kCollisionServer = collisionParts->getKCollisionServer();
    switch (index) {
    case 0:
        mEdgeNormals[0] = kCollisionServer.getEdgeNormal1(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[0].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[0]);
        normalize(&mEdgeNormals[0]);
        return getEdgeNormal(index);
    case 1:
        mEdgeNormals[1] = kCollisionServer.getEdgeNormal2(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[1].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[1]);
        normalize(&mEdgeNormals[1]);
        return getEdgeNormal(index);
    case 2:
        mEdgeNormals[2] = kCollisionServer.getEdgeNormal3(mKCPrismData, mKCPrismHeader);
        mEdgeNormals[2].setRotated(collisionParts->getBaseMtx(), mEdgeNormals[2]);
        normalize(&mEdgeNormals[2]);
        return getEdgeNormal(index);
        break;
    default:
        return getEdgeNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetPos(int index) {
    mCollisionParts->getKCollisionServer().calcPosLocal(&mVerts[index], mKCPrismData, index, mKCPrismHeader);
    mVerts[index].setMul(mCollisionParts->getBaseMtx(), mVerts[index]);
    return mVerts[index];
}

void Triangle::getLocalPos(sead::Vector3f* pos, int index) const {
    mCollisionParts->getKCollisionServer().calcPosLocal(pos, mKCPrismData, index, mKCPrismHeader);
}

void Triangle::calcForceMovePower(sead::Vector3f* power, const sead::Vector3f& pos) const {
    mCollisionParts->calcForceMovePower(power, pos);
}

void Triangle::calcForceRotatePower(sead::Quatf* power) const {
    mCollisionParts->calcForceRotatePower(power);
}

bool Triangle::getAttributes(ByamlIter* iter) const {
    if(!mKCPrismData)
        return false;
    return mCollisionParts->getKCollisionServer().getAttributes(iter, mKCPrismData);
}

const HitSensor* Triangle::getSensor() const {
    return mCollisionParts->getConnectedSensor();
}

const sead::Matrix34f& Triangle::getBaseMtx() const {
    return mCollisionParts->getBaseMtx();
}

const sead::Matrix34f& Triangle::getBaseInvMtx() const {
    return mCollisionParts->getBaseInvMtx();
}

const sead::Matrix34f& Triangle::getPrevBaseMtx() const {
    return mCollisionParts->getPrevBaseMtx();
}

HitInfo::HitInfo() = default;

}  // namespace al

bool operator==(const al::Triangle& lhs, const al::Triangle& rhs) {
    return lhs.mCollisionParts == rhs.mCollisionParts && lhs.mKCPrismData == rhs.mKCPrismData;
}

bool operator!=(const al::Triangle& lhs, const al::Triangle& rhs) {
    return !(lhs == rhs);
}
