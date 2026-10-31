#include "Library/Collision/HitInfo.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathUtil.h"

namespace al {

Triangle::Triangle() : mCollisionParts(nullptr), mData(nullptr), mHeader(nullptr) {
    // nullptrs above done to avoid initializing them in the other ctor

    mNormals[0] = sead::Vector3f::zero;
    mNormals[1] = sead::Vector3f::zero;
    mNormals[2] = sead::Vector3f::zero;
    mNormals[3] = sead::Vector3f::zero;

    mPositions[0] = sead::Vector3f::zero;
    mPositions[1] = sead::Vector3f::zero;
    mPositions[2] = sead::Vector3f::zero;
}

Triangle::Triangle(const CollisionParts& parts, const KCPrismData* data,
                   const KCPrismHeader* header) {
    fillData(parts, data, header);
}

void Triangle::fillData(const CollisionParts& parts, const KCPrismData* data,
                        const KCPrismHeader* header) {
    mCollisionParts = &parts;
    mData = data;
    mHeader = header;

    auto* server = parts.getServer();
    const auto& baseMtx = parts.getBaseMtx();

    mNormals[0].setRotated(baseMtx, server->getFaceNormal(data, mHeader));
    mNormals[1].setRotated(baseMtx, server->getEdgeNormal1(data, mHeader));
    mNormals[2].setRotated(baseMtx, server->getEdgeNormal2(data, mHeader));
    mNormals[3].setRotated(baseMtx, server->getEdgeNormal3(data, mHeader));

    normalize(&mNormals[0]);
    normalize(&mNormals[1]);
    normalize(&mNormals[2]);
    normalize(&mNormals[3]);

    server->calcPosLocal(&mPositions[0], data, 0, mHeader);
    server->calcPosLocal(&mPositions[1], data, 1, mHeader);
    server->calcPosLocal(&mPositions[2], data, 2, mHeader);

    mPositions[0].mul(baseMtx);
    mPositions[1].mul(baseMtx);
    mPositions[2].mul(baseMtx);
}

void Triangle::fill(const sead::Vector3f& pos1, const sead::Vector3f& pos2,
                    const sead::Vector3f& pos3) {
    mPositions[0] = pos1;
    mPositions[1] = pos2;
    mPositions[2] = pos3;

    sead::Vector3f vert1To2 = pos2 - pos1;
    sead::Vector3f vert1To3 = pos3 - pos1;
    sead::Vector3f vert2To3 = pos3 - pos2;

    mNormals[0].setCross(vert1To2, vert1To3);
    tryNormalizeOrZero(&mNormals[0]);

    mNormals[1].setCross(vert1To2, mNormals[0]);
    tryNormalizeOrZero(&mNormals[1]);
    mNormals[2].setCross(mNormals[0], vert1To3);
    tryNormalizeOrZero(&mNormals[2]);
    mNormals[3].setCross(vert2To3, mNormals[0]);
    tryNormalizeOrZero(&mNormals[3]);

    mData = nullptr;
}

const LiveActor* Triangle::getHostActor() const {
    if (!mCollisionParts)
        return nullptr;
    return mCollisionParts->getConnectedHost();
}

bool Triangle::isHostMoved() const {
    return mCollisionParts->isMoving();
}

bool Triangle::isValid() const {
    return mData != nullptr;
}

const sead::Vector3f& Triangle::getNormal(s32 index) const {
    return mNormals[index];
}

const sead::Vector3f& Triangle::getFaceNormal() const {
    return getNormal(0);
}

const sead::Vector3f& Triangle::getEdgeNormal(s32 index) const {
    return getNormal(index + 1);
}

const sead::Vector3f& Triangle::getPos(s32 index) const {
    return mPositions[index];
}

void Triangle::calcCenterPos(sead::Vector3f* center) const {
    *center = (getPos(0) + getPos(1) + getPos(2)) * (1 / 3.0f);
}

const sead::Vector3f& Triangle::calcAndGetNormal(s32 index) {
    auto* parts = mCollisionParts;
    auto* server = parts->getServer();

    switch (index) {
    case 0:
        return calcAndGetFaceNormal();
    case 1:
        mNormals[1] = server->getEdgeNormal1(mData, mHeader);
        mNormals[1].setRotated(parts->getBaseMtx(), mNormals[1]);
        normalize(&mNormals[1]);
        return getNormal(index);
    case 2:
        mNormals[2] = server->getEdgeNormal2(mData, mHeader);
        mNormals[2].setRotated(parts->getBaseMtx(), mNormals[2]);
        normalize(&mNormals[2]);
        return getNormal(index);
    case 3:
        mNormals[3] = server->getEdgeNormal3(mData, mHeader);
        mNormals[3].setRotated(parts->getBaseMtx(), mNormals[3]);
        normalize(&mNormals[3]);
        return getNormal(index);
    default:
        return getNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetFaceNormal() {
    auto* parts = mCollisionParts;

    mNormals[0] = parts->getServer()->getFaceNormal(mData, mHeader);
    mNormals[0].setRotated(parts->getBaseMtx(), mNormals[0]);
    normalize(&mNormals[0]);
    return mNormals[0];
}

const sead::Vector3f& Triangle::calcAndGetEdgeNormal(s32 index) {
    auto* parts = mCollisionParts;
    auto* server = parts->getServer();

    switch (index) {
    case 0:
        mNormals[1] = server->getEdgeNormal1(mData, mHeader);
        mNormals[1].rotate(parts->getBaseMtx());
        normalize(&mNormals[1]);
        return getEdgeNormal(index);
    case 1:
        mNormals[2] = server->getEdgeNormal2(mData, mHeader);
        mNormals[2].rotate(parts->getBaseMtx());
        normalize(&mNormals[2]);
        return getEdgeNormal(index);
    case 2:
        mNormals[3] = server->getEdgeNormal3(mData, mHeader);
        mNormals[3].rotate(parts->getBaseMtx());
        normalize(&mNormals[3]);
        return getEdgeNormal(index);
    default:
        return getEdgeNormal(index);
    }
}

const sead::Vector3f& Triangle::calcAndGetPos(s32 index) {
    mCollisionParts->getServer()->calcPosLocal(&mPositions[index], mData, index, mHeader);
    mPositions[index].mul(getBaseMtx());
    return getPos(index);
}

void Triangle::getLocalPos(sead::Vector3f* pos, s32 index) const {
    mCollisionParts->getServer()->calcPosLocal(pos, mData, index, mHeader);
}

void Triangle::calcForceMovePower(sead::Vector3f* power, const sead::Vector3f& pos) const {
    mCollisionParts->calcForceMovePower(power, pos);
}

void Triangle::calcForceRotatePower(sead::Quatf* power) const {
    mCollisionParts->calcForceRotatePower(power);
}

bool Triangle::getAttributes(ByamlIter* iter) const {
    if (!isValid())
        return false;
    return mCollisionParts->getServer()->getAttributes(iter, mData);
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

bool HitInfo::isCollisionAtFace() const {
    return location == KCollisionLocation_Face;
}

bool HitInfo::isCollisionAtEdge() const {
    return location == KCollisionLocation_Edge1 || location == KCollisionLocation_Edge2 ||
           location == KCollisionLocation_Edge3;
}

bool HitInfo::isCollisionAtCorner() const {
    return location == KCollisionLocation_Corner1 || location == KCollisionLocation_Corner2 ||
           location == KCollisionLocation_Corner3;
}

const sead::Vector3f& HitInfo::tryGetHitEdgeNormal() const {
    if (location == KCollisionLocation_Edge1)
        return triangle.getEdgeNormal(0);
    if (location == KCollisionLocation_Edge2)
        return triangle.getEdgeNormal(1);
    if (location == KCollisionLocation_Edge3)
        return triangle.getEdgeNormal(2);

    return sead::Vector3f::zero;
}

void SphereHitInfo::calcFixVector(sead::Vector3f* outFix, sead::Vector3f* outFixNormal) const {
    if (isCollisionAtFace()) {
        calcFixVectorNormal(outFix, outFixNormal);
        return;
    }

    sead::Vector3f hitNormal;
    hitNormal.setSub(checkPos, hitPos);
    tryNormalizeOrZero(&hitNormal);

    sead::Vector3f fix;
    sead::Vector3f fixNormal;
    f32 fixDist = hitNormal.dot(triangle.getFaceNormal() * distance);
    f32 fixNormalDist = hitNormal.dot(triangle.getFaceNormal());
    sead::Vector3CalcCommon<f32>::multScalar(fix, hitNormal, fixDist);
    sead::Vector3CalcCommon<f32>::multScalar(fixNormal, hitNormal, fixNormalDist);
    *outFix = fix;
    *outFixNormal = fixNormal;
}

void SphereHitInfo::calcFixVectorNormal(sead::Vector3f* outFix,
                                        sead::Vector3f* outFixNormal) const {
    outFix->setScale(triangle.getFaceNormal(), distance);

    if (outFixNormal)
        outFixNormal->set(triangle.getFaceNormal());
}

void DiskHitInfo::calcFixVector(sead::Vector3f* outFix, sead::Vector3f* outFixNormal) const {
    if (isCollisionAtFace()) {
        calcFixVectorNormal(outFix, outFixNormal);
        return;
    }

    sead::Vector3f hitNormal;
    hitNormal.setSub(checkPos, hitPos);
    tryNormalizeOrZero(&hitNormal);

    sead::Vector3f fix;
    sead::Vector3f fixNormal;
    f32 fixDist = hitNormal.dot(triangle.getFaceNormal() * distance);
    f32 fixNormalDist = hitNormal.dot(triangle.getFaceNormal());
    sead::Vector3CalcCommon<f32>::multScalar(fix, hitNormal, fixDist);
    sead::Vector3CalcCommon<f32>::multScalar(fixNormal, hitNormal, fixNormalDist);
    *outFix = fix;
    *outFixNormal = fixNormal;
}

void DiskHitInfo::calcFixVectorNormal(sead::Vector3f* outFix, sead::Vector3f* outFixNormal) const {
    outFix->setScale(triangle.getFaceNormal(), distance);

    if (outFixNormal)
        outFixNormal->set(triangle.getFaceNormal());
}

}  // namespace al

bool operator==(const al::Triangle& lhs, const al::Triangle& rhs) {
    return lhs.mCollisionParts == rhs.mCollisionParts && lhs.mData == rhs.mData;
}

bool operator!=(const al::Triangle& lhs, const al::Triangle& rhs) {
    return !(lhs == rhs);
}
