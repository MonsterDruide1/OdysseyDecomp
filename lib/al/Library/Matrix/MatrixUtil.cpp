#include "Library/Matrix/MatrixUtil.h"

#include "Library/Math/MathUtil.h"

namespace al {

void makeMtxRotateTrans(sead::Matrix34f* outMtx, const sead::Vector3f& rotate,
                        const sead::Vector3f& trans) {
    sead::Vector3f rotateRad(sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                             sead::Mathf::deg2rad(rotate.z));
    outMtx->makeRT(rotateRad, trans);
}

void makeMtxFromTwoAxis(sead::Matrix34f* outMtx, const sead::Vector3f& vectorA,
                        const sead::Vector3f& vectorB, s32 axisA, s32 axisB) {
    sead::Vector3f dir[3];

    s32 axisC;
    if (axisA == 0)
        axisC = axisB == 1 ? 2 : 1;
    else if (axisA == 1)
        axisC = axisB == 0 ? 2 : 0;
    else
        axisC = axisB == 0 ? 1 : 0;

    normalize(&dir[axisA], vectorA);
    dir[axisB] = vectorB;

    dir[axisC].setCross(dir[(axisC + 1) % 3], dir[(axisC + 2) % 3]);
    normalize(&dir[axisC]);

    dir[axisB].setCross(dir[(axisB + 1) % 3], dir[(axisB + 2) % 3]);

    outMtx->setBase(0, dir[0]);  // side
    outMtx->setBase(1, dir[1]);  // up
    outMtx->setBase(2, dir[2]);  // front
}

void makeMtxFrontUp(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                    const sead::Vector3f& up) {
    sead::Vector3f frontNorm = front;
    normalize(&frontNorm);

    sead::Vector3f side;
    side.setCross(up, frontNorm);
    normalize(&side);

    sead::Vector3f upNorm;
    upNorm.setCross(frontNorm, side);

    outMtx->setBase(0, side);
    outMtx->setBase(1, upNorm);
    outMtx->setBase(2, frontNorm);
}

void makeMtxFrontSide(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                      const sead::Vector3f& side) {
    sead::Vector3f frontNorm = front;
    normalize(&frontNorm);

    sead::Vector3f up;
    up.setCross(frontNorm, side);
    normalize(&up);

    sead::Vector3f sideNorm;
    sideNorm.setCross(up, frontNorm);

    outMtx->setBase(0, sideNorm);
    outMtx->setBase(1, up);
    outMtx->setBase(2, frontNorm);
}

void makeMtxUpFront(sead::Matrix34f* outMtx, const sead::Vector3f& up,
                    const sead::Vector3f& front) {
    sead::Vector3f upNorm = up;
    normalize(&upNorm);

    sead::Vector3f side;
    side.setCross(up, front);
    normalize(&side);

    sead::Vector3f frontNorm;
    frontNorm.setCross(side, upNorm);

    outMtx->setBase(0, side);
    outMtx->setBase(1, upNorm);
    outMtx->setBase(2, frontNorm);
}

void makeMtxUpSide(sead::Matrix34f* outMtx, const sead::Vector3f& up, const sead::Vector3f& side) {
    sead::Vector3f upNorm = up;
    normalize(&upNorm);

    sead::Vector3f front;
    front.setCross(side, up);
    normalize(&front);

    sead::Vector3f sideNorm;
    sideNorm.setCross(upNorm, front);

    outMtx->setBase(0, sideNorm);
    outMtx->setBase(1, upNorm);
    outMtx->setBase(2, front);
}

void makeMtxSideUp(sead::Matrix34f* outMtx, const sead::Vector3f& side, const sead::Vector3f& up) {
    makeMtxFromTwoAxis(outMtx, side, up, 0, 1);
}

void makeMtxSideFront(sead::Matrix34f* outMtx, const sead::Vector3f& side,
                      const sead::Vector3f& front) {
    makeMtxFromTwoAxis(outMtx, side, front, 0, 2);
}

void makeMtxFrontNoSupport(sead::Matrix34f* outMtx, const sead::Vector3f& front) {
    bool isYAxis = getMaxAbsElementIndex(front) == 1;

    sead::Vector3f up;
    up.x = isYAxis ? sead::Vector3f::ez.x : sead::Vector3f::ey.x;
    up.y = isYAxis ? sead::Vector3f::ez.y : sead::Vector3f::ey.y;
    up.z = isYAxis ? sead::Vector3f::ez.z : sead::Vector3f::ey.z;

    makeMtxFrontUp(outMtx, front, up);
}

void makeMtxFrontNoSupportPos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                              const sead::Vector3f& pos) {
    makeMtxFrontNoSupport(outMtx, front);
    outMtx->setBase(3, pos);
}

void makeMtxUpNoSupport(sead::Matrix34f* outMtx, const sead::Vector3f& up) {
    bool isZAxis = getMaxAbsElementIndex(up) == 2;

    sead::Vector3f front;
    front.x = isZAxis ? sead::Vector3f::ex.x : sead::Vector3f::ez.x;
    front.y = isZAxis ? sead::Vector3f::ex.y : sead::Vector3f::ez.y;
    front.z = isZAxis ? sead::Vector3f::ex.z : sead::Vector3f::ez.z;

    makeMtxUpFront(outMtx, up, front);
}

void makeMtxUpNoSupportPos(sead::Matrix34f* outMtx, const sead::Vector3f& up,
                           const sead::Vector3f& pos) {
    makeMtxUpNoSupport(outMtx, up);
    outMtx->setBase(3, pos);
}

void makeMtxFrontUpPos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                       const sead::Vector3f& up, const sead::Vector3f& pos) {
    makeMtxFrontUp(outMtx, front, up);
    outMtx->setBase(3, pos);
}

void makeMtxFrontSidePos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                         const sead::Vector3f& side, const sead::Vector3f& pos) {
    makeMtxFrontSide(outMtx, front, side);
    outMtx->setBase(3, pos);
}

void makeMtxUpFrontPos(sead::Matrix34f* outMtx, const sead::Vector3f& up,
                       const sead::Vector3f& front, const sead::Vector3f& pos) {
    makeMtxUpFront(outMtx, up, front);
    outMtx->setBase(3, pos);
}

void makeMtxUpSidePos(sead::Matrix34f* outMtx, const sead::Vector3f& up, const sead::Vector3f& side,
                      const sead::Vector3f& pos) {
    makeMtxUpSide(outMtx, up, side);
    outMtx->setBase(3, pos);
}

void makeMtxSideUpPos(sead::Matrix34f* outMtx, const sead::Vector3f& side, const sead::Vector3f& up,
                      const sead::Vector3f& pos) {
    makeMtxSideUp(outMtx, side, up);
    outMtx->setBase(3, pos);
}

void makeMtxSideFrontPos(sead::Matrix34f* outMtx, const sead::Vector3f& side,
                         const sead::Vector3f& front, const sead::Vector3f& pos) {
    makeMtxSideFront(outMtx, side, front);
    outMtx->setBase(3, pos);
}

void makeMtxFollowTarget(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx,
                         const sead::Vector3f& trans, const sead::Vector3f& rotate) {
    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotateRad(sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                             sead::Mathf::deg2rad(rotate.z));
    rotationMatrix.makeR(rotateRad);

    sead::Matrix34f translationMatrix;
    translationMatrix.makeRT({0.0f, 0.0f, 0.0f}, trans);

    sead::Matrix34f pose = rotationMatrix * translationMatrix;
    *outMtx = baseMtx * pose;
}

void rotateMtxXDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle) {
    sead::Matrix34f rotationMatrix;
    rotationMatrix.makeR(sead::Mathf::deg2rad(angle) * sead::Vector3f::ex);

    *outMtx = baseMtx * rotationMatrix;
}

void rotateMtxYDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle) {
    sead::Matrix34f rotationMatrix;
    rotationMatrix.makeR(sead::Mathf::deg2rad(angle) * sead::Vector3f::ey);

    *outMtx = baseMtx * rotationMatrix;
}

void rotateMtxZDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle) {
    sead::Matrix34f rotationMatrix;
    rotationMatrix.makeR(sead::Mathf::deg2rad(angle) * sead::Vector3f::ez);

    *outMtx = baseMtx * rotationMatrix;
}

void calcMtxScale(sead::Vector3f* outMtx, const sead::Matrix34f& mtx) {
    outMtx->x = sead::Mathf::sqrt(mtx.m[0][0] * mtx.m[0][0] + mtx.m[1][0] * mtx.m[1][0] +
                                  mtx.m[2][0] * mtx.m[2][0]);

    outMtx->y = sead::Mathf::sqrt(mtx.m[0][1] * mtx.m[0][1] + mtx.m[1][1] * mtx.m[1][1] +
                                  mtx.m[2][1] * mtx.m[2][1]);

    outMtx->z = sead::Mathf::sqrt(mtx.m[0][2] * mtx.m[0][2] + mtx.m[1][2] * mtx.m[1][2] +
                                  mtx.m[2][2] * mtx.m[2][2]);
}

void calcMtxScale(sead::Vector3f* outMtx, const Matrix43f& mtx) {
    outMtx->x = sead::Mathf::sqrt(mtx.m[0][0] * mtx.m[0][0] + mtx.m[0][1] * mtx.m[0][1] +
                                  mtx.m[0][2] * mtx.m[0][2]);

    outMtx->y = sead::Mathf::sqrt(mtx.m[1][0] * mtx.m[1][0] + mtx.m[1][1] * mtx.m[1][1] +
                                  mtx.m[1][2] * mtx.m[1][2]);

    outMtx->z = sead::Mathf::sqrt(mtx.m[2][0] * mtx.m[2][0] + mtx.m[2][1] * mtx.m[2][1] +
                                  mtx.m[2][2] * mtx.m[2][2]);
}

void normalizeMtxScale(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx) {
    sead::Vector3f scale;
    calcMtxScale(&scale, mtx);

    if (!isNearZero(scale.x, 0.001f) && !isNearZero(scale.y, 0.001f) &&
        !isNearZero(scale.z, 0.001f)) {
        f32 xInv = 1.0f / scale.x;
        f32 yInv = 1.0f / scale.y;
        f32 zInv = 1.0f / scale.z;

        *outMtx = mtx;
        outMtx->scaleBases(xInv, yInv, zInv);
    }
}

bool tryNormalizeMtxScaleOrIdentity(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx) {
    sead::Vector3f scale;
    calcMtxScale(&scale, mtx);

    if (!isNearZero(scale.x, 0.001f) && !isNearZero(scale.y, 0.001f) &&
        !isNearZero(scale.z, 0.001f)) {
        f32 xInv = 1.0f / scale.x;
        f32 yInv = 1.0f / scale.y;
        f32 zInv = 1.0f / scale.z;

        *outMtx = mtx;
        outMtx->scaleBases(xInv, yInv, zInv);
        return true;
    }

    sead::Vector3f base;
    mtx.getBase(base, 3);
    outMtx->makeIdentity();
    outMtx->setBase(3, base);

    return false;
}

void preScaleMtx(sead::Matrix34f* outMtx, const sead::Vector3f& scale) {
    outMtx->scaleBases(scale.x, scale.y, scale.z);
}

}  // namespace al
