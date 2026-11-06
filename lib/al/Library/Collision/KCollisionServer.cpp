#include "Library/Collision/KCollisionServer.h"

#include <math/seadVector.h>
#include <prim/seadPtrUtil.h>

#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

static bool checkHitFace(const f32* pDist, u8* pLocation, f32 maxDepth) {
    if (*pDist > maxDepth) {
        *pLocation = KCLocation_Face;
        return true;
    }

    return false;
}

static void calcCornerVec(sead::Vector3f* outVec, f32 distA, f32 distB, const KCFxyz* normalA,
                          const KCFxyz* normalB, f32 cos) {
    f32 rateA = (distB * cos - distA) / (cos * cos + -1.0f);
    f32 rateB = distB - cos * rateA;

    outVec->x = normalA->x * rateA + normalB->x * rateB;
    outVec->y = rateA * normalA->y + normalB->y * rateB;
    outVec->z = rateA * normalA->z + rateB * normalB->z;
}

static void setUsingCast(sead::Vector3u* out, const sead::Vector3f pos) {
    out->x = static_cast<s32>(pos.x);
    out->y = static_cast<s32>(pos.y);
    out->z = static_cast<s32>(pos.z);
}

KCollisionServer::KCollisionServer() = default;

void KCollisionServer::initKCollisionServer(void* data, const void* attributeData) {
    setData(data);

    if (attributeData)
        mAttributeIter = new ByamlIter(static_cast<const u8*>(attributeData));
}

void KCollisionServer::setData(void* data) {
    mData = static_cast<KCFileHeader*>(data);

    mModelOffsets =
        static_cast<const u32*>(sead::PtrUtil::addOffset(data, mData->modelOffsetsOffset));
    mOctreeData = static_cast<const u8*>(sead::PtrUtil::addOffset(data, mData->octreeOffset));

    mAreaWidthShift.x = mData->areaWidthShift.x - 1;
    mAreaWidthShift.y = mData->areaWidthShift.y - 1;
    mAreaWidthShift.z = mData->areaWidthShift.z - 1;
    mAreaWidthMask.x = -1 << mData->areaWidthShift.x;
    mAreaWidthMask.y = -1 << mData->areaWidthShift.y;
    mAreaWidthMask.z = -1 << mData->areaWidthShift.z;

    mModelHeaders.allocBuffer(mData->modelNum, nullptr);

    for (s32 idx = 0; idx < mData->modelNum; idx++)
        mModelHeaders.pushBack(const_cast<KCPrismHeader*>(getInnerKcl(idx)));
}

const KCPrismHeader* KCollisionServer::getInnerKcl(s32 index) const {
    return reinterpret_cast<const KCPrismHeader*>(reinterpret_cast<const u8*>(mData) +
                                                  mModelOffsets[index]);
}

s32 KCollisionServer::getNumInnerKcl() const {
    return mData->modelNum;
}

const KCPrismHeader* KCollisionServer::getV1Header(s32 index) const {
    return mModelHeaders.at(index);
}

bool KCollisionServer::calcFarthestVertexDistance() {
    bool isEmpty = true;
    f32 maxDistSq = 0.0f;

    for (s32 modelIdx = 0; modelIdx < getNumInnerKcl(); modelIdx++) {
        auto* header = getV1Header(modelIdx);

        u32 triangleNum = getTriangleNum(header);
        for (u32 triIdx = 0; triIdx < triangleNum; triIdx++) {
            isEmpty = false;
            auto* data = const_cast<KCPrismData*>(getPrismData(triIdx, header));

            if (isNearParallelNormal(data, header)) {
                data->length = -sead::Mathf::abs(data->length);
                continue;
            }

            if (isNanPrism(data, header)) {
                f32 length = -sead::Mathf::abs(data->length);
                data->length = sead::Mathf::isNan(data->length) ? 0.0f : length;
                continue;
            }

            for (s32 posIdx = 0; posIdx < 3; posIdx++) {
                sead::Vector3f pos;
                calcPosLocal(&pos, data, posIdx, header);
                f32 distSq = pos.squaredLength();

                if (maxDistSq < distSq)
                    maxDistSq = distSq;
            }
        }
    }

    mFarthestVertexDistance = sead::Mathf::sqrt(maxDistSq);
    return isEmpty;
}

u32 KCollisionServer::getTriangleNum(const KCPrismHeader* header) const {
    return (static_cast<u64>(header->octreeOffset) - header->prismsOffset) / sizeof(KCPrismData);
}

const KCPrismData* KCollisionServer::getPrismData(u32 index, const KCPrismHeader* header) const {
    return &header->getPrisms()[(s32)index];
}

bool KCollisionServer::isNearParallelNormal(const KCPrismData* data,
                                            const KCPrismHeader* header) const {
    sead::Vector3f edge1 = *getEdgeNormal1(data, header);
    sead::Vector3f edge2 = *getEdgeNormal2(data, header);
    sead::Vector3f edge3 = *getEdgeNormal3(data, header);

    return isParallelDirection(edge1, edge2) || isParallelDirection(edge1, edge3) ||
           isParallelDirection(edge2, edge3);
}

// NON_MATCHING: Non-matching calcPosLocal is inlined with potentially other issues too
bool KCollisionServer::isNanPrism(const KCPrismData* data, const KCPrismHeader* header) const {
    if (sead::Mathf::isNan(data->length))
        return true;

    sead::Vector3f v0, v1, v2;
    calcPosLocal(&v0, data, 0, header);
    calcPosLocal(&v1, data, 1, header);
    calcPosLocal(&v2, data, 2, header);

    return v0.isNan() || v1.isNan() || getFaceNormal(data, header)->isNan() ||
           getEdgeNormal1(data, header)->isNan() || getEdgeNormal2(data, header)->isNan() ||
           getEdgeNormal3(data, header)->isNan() || v2.isNan();
}

inline void calcPos(sead::Vector3f* out, const KCPrismData* data, const sead::Vector3f& position,
                    const sead::Vector3f& a, const sead::Vector3f& b, const sead::Vector3f& c) {
    sead::Vector3f v21 = a.cross(b);
    f32 dot = v21.dot(c);

    f32 factor = data->length / sead::Mathf::max(dot, sead::Mathf::epsilon());
    out->setScaleAdd(factor, v21, position);
}

// NON_MATCHING: Instruction ordering (https://decomp.me/scratch/Bv8fA)
void KCollisionServer::calcPosLocal(sead::Vector3f* outPos, const KCPrismData* data,
                                    s32 vertexIndex, const KCPrismHeader* header) const {
    switch (vertexIndex) {
    case 0: {
        sead::Vector3f position = header->getVertices()[data->vertexIndex];
        *outPos = position;
        return;
    }
    case 1: {
        calcPos(outPos, data, header->getVertices()[data->vertexIndex],
                header->getNormals()[data->faceNormalIndex],
                header->getNormals()[data->edgeNormalIndex[1]],
                header->getNormals()[data->edgeNormalIndex[2]]);
        return;
    }
    case 2: {
        calcPos(outPos, data, header->getVertices()[data->vertexIndex],
                header->getNormals()[data->edgeNormalIndex[0]],
                header->getNormals()[data->faceNormalIndex],
                header->getNormals()[data->edgeNormalIndex[2]]);
        return;
    }
    default:
        *outPos = sead::Vector3f(0.0f, 0.0f, 0.0f);
        return;
    }
}

void KCollisionServer::getMinMax(sead::Vector3f* outMin, sead::Vector3f* outMax) const {
    *outMin = sead::Vector3f(mData->minCoordinate);
    *outMax = sead::Vector3f(mData->maxCoordinate);
}

void KCollisionServer::getAreaSpaceSize(sead::Vector3f* outSize,
                                        const KCPrismHeader* header) const {
    s32 sizeX, sizeY, sizeZ;
    getAreaSpaceSize(&sizeX, &sizeY, &sizeZ, header);

    outSize->set(sizeX, sizeY, sizeZ);
}

void KCollisionServer::getAreaSpaceSize(s32* outSizeX, s32* outSizeY, s32* outSizeZ,
                                        const KCPrismHeader* header) const {
    *outSizeX = ~header->widthMask.x;
    *outSizeY = ~header->widthMask.y;
    *outSizeZ = ~header->widthMask.z;
}

void KCollisionServer::getAreaSpaceSize(sead::Vector3u* outSize,
                                        const KCPrismHeader* header) const {
    outSize->x = ~header->widthMask.x;
    outSize->y = ~header->widthMask.y;
    outSize->z = ~header->widthMask.z;
}

const KCPrismData* KCollisionServer::checkPoint(KCFxyz* position, f32 thicknessScale,
                                                f32* outDistance) {
    for (s32 idx = 0; idx < getNumInnerKcl(); idx++) {
        f32 thickness;
        s32 shift;
        f32 dist;
        sead::Vector3u areaOffset;
        sead::Vector3f checkOffset;
        const KCPrismData* data;
        const KCPrismHeader* header;
        const KCFxyz* vertex;
        const KCFxyz* faceNormal;
        const u16* prismIndices;

        header = getV1Header(idx);

        // Using the helper functions is less ugly, but doesn't match sadly..
        // objectSpaceToAreaOffsetSpace(&areaOffset, *position, header);
        // if (!isInsideMinMaxInAreaOffsetSpace(areaOffset, header))
        //     continue;

        areaOffset.x = static_cast<s32>(position->x - header->octreeOrigin.x);
        if ((areaOffset.x & header->widthMask.x) != 0)
            continue;
        areaOffset.y = static_cast<s32>(position->y - header->octreeOrigin.y);
        if ((areaOffset.y & header->widthMask.y) != 0)
            continue;
        areaOffset.z = static_cast<s32>(position->z - header->octreeOrigin.z);
        if ((areaOffset.z & header->widthMask.z) != 0)
            continue;

        thickness = header->thickness * thicknessScale;
        prismIndices = searchBlock(&shift, areaOffset, header);

        while (*(++prismIndices) != 0xffff) {
            data = getPrismData(*prismIndices, header);
            if (data->length <= 0.0f)
                continue;

            vertex = getVertexData(data->vertexIndex, header);
            checkOffset = *position - *vertex;

            if (checkOffset.dot(*getEdgeNormal1(data, header)) > 0.0f)
                continue;

            if (checkOffset.dot(*getEdgeNormal2(data, header)) > 0.0f)
                continue;

            if (checkOffset.dot(*getEdgeNormal3(data, header)) > data->length)
                continue;

            faceNormal = getFaceNormal(data, header);
            dist = -checkOffset.x * faceNormal->x - checkOffset.y * faceNormal->y -
                   checkOffset.z * faceNormal->z;

            if (!(dist < 0.0f) && !(thickness < dist)) {
                *outDistance = dist;
                return data;
            }
        }
    }

    return nullptr;
}

const u16* KCollisionServer::searchBlock(s32* outShift, const sead::Vector3u& block,
                                         const KCPrismHeader* header) const {
    u32 offset = calcAreaBlockOffset(block, header);
    auto* octree = header->getOctree();

    *outShift = header->blockWidthShift;

    if (header->areaXYWidthShift == -1 && header->areaXWidthShift == -1)
        offset = 0;

    u32 data = getBlockData(octree, offset);

    while (!(data & KCOctree_FlagLeaf)) {
        octree = static_cast<const u32*>(sead::PtrUtil::addOffset(octree, data));
        (*outShift)--;

        offset = calcChildBlockOffset(block, *outShift);
        data = getBlockData(octree, offset);
    }

    return static_cast<const u16*>(sead::PtrUtil::addOffset(octree, data & KCOctree_MaskOffset));
}

// NON_MATCHING: Stack layout/Register allocation + slight instruction ordering
// (https://decomp.me/scratch/VthBV)
u32 KCollisionServer::checkSphere(const KCFxyz* position, f32 radius, f32 thicknessScale,
                                  u32 maxHitNum, HitInfoBuffer* outHitInfos) {
    KCHitInfo hitInfo;
    hitInfo.header = nullptr;
    hitInfo.data = nullptr;
    hitInfo.distance = 0.0f;
    hitInfo.location = KCLocation_None;

    sead::Vector3f posMax;
    posMax.x = position->x + radius;
    posMax.y = position->y + radius;
    posMax.z = position->z + radius;

    sead::Vector3f posMin;
    posMin.x = position->x - radius;
    posMin.y = position->y - radius;
    posMin.z = position->z - radius;

    const u16* prismList;
    const u16* nextSkipList = nullptr;
    const u16* skipList = nullptr;

    u32 hitNum = 0;
    for (s32 i = 0; i < getNumInnerKcl(); i++) {
        auto* header = getV1Header(i);

        sead::Vector3u blockMin, blockMax;
        if (!outCheckAndCalcArea(&blockMin, &blockMax, posMin, posMax, header))
            continue;

        u32 z = blockMin.z;
        do {
            s32 stepZ = 1000000;

            u32 y = blockMin.y;
            do {
                s32 maxRemY = 0;
                s32 stepY = 1000000;

                u32 x = blockMin.x;
                do {
                    s32 shift;
                    sead::Vector3u block(x, y, z);
                    prismList = searchBlock(&shift, block, header);

                    s32 blockSize = 1 << shift;
                    s32 blockMask = blockSize - 1;

                    s32 remX = blockSize - (blockMask & x);
                    s32 remY = blockSize - (blockMask & y);
                    s32 remZ = blockSize - (blockMask & z);

                    if (remZ < stepZ)
                        stepZ = remZ;
                    if (remY < stepY)
                        stepY = remY;

                    if (remY > maxRemY && prismList[1] != 0) {
                        maxRemY = remY;
                        nextSkipList = prismList;
                    }

                    if (!skipList || prismList != skipList) {
                        while (*(++prismList) != 0xffff) {
                            auto* data = getPrismData(*prismList, header);
                            hitInfo.data = data;

                            if (data->length <= 0.0f)
                                continue;

                            bool alreadyChecked = false;
                            for (auto it = outHitInfos->begin(); it != outHitInfos->end(); ++it) {
                                if (it->data == data) {
                                    alreadyChecked = true;
                                    break;
                                }
                            }
                            if (alreadyChecked)
                                continue;

                            bool isHit = KCHitSphere(data, header, position, radius, thicknessScale,
                                                     &hitInfo.distance, &hitInfo.location);

                            if (isHit && hitNum < maxHitNum) {
                                hitInfo.header = header;
                                outHitInfos->pushBack(hitInfo);
                                hitNum++;
                            }
                        }
                    }

                    x += remX;
                } while (x <= blockMax.x);

                skipList = nextSkipList;
                y += stepY;
            } while (y <= blockMax.y);

            z += stepZ;
        } while (z <= blockMax.z);
    }

    return hitNum;
}

bool KCollisionServer::outCheckAndCalcArea(sead::Vector3u* outBlockMin, sead::Vector3u* outBlockMax,
                                           const sead::Vector3f& positionMin,
                                           const sead::Vector3f& positionMax,
                                           const KCPrismHeader* header) const {
    objectSpaceToAreaOffsetSpace(outBlockMin, positionMin, header);
    objectSpaceToAreaOffsetSpace(outBlockMax, positionMax, header);

    if (static_cast<s32>(outBlockMin->x) < 0)
        outBlockMin->x = 0;

    if (static_cast<s32>(outBlockMin->y) < 0)
        outBlockMin->y = 0;

    if (static_cast<s32>(outBlockMin->z) < 0)
        outBlockMin->z = 0;

    if (static_cast<s32>(~header->widthMask.x) < static_cast<s32>(outBlockMax->x))
        outBlockMax->x = ~header->widthMask.x;

    if (static_cast<s32>(~header->widthMask.y) < static_cast<s32>(outBlockMax->y))
        outBlockMax->y = ~header->widthMask.y;

    if (static_cast<s32>(~header->widthMask.z) < static_cast<s32>(outBlockMax->z))
        outBlockMax->z = ~header->widthMask.z;

    for (s32 i = 0; i < 3; i++)
        if (static_cast<s32>(outBlockMax->e[i]) < static_cast<s32>(outBlockMin->e[i]))
            return false;

    return true;
}

// NON_MATCHING: Incorrect fcmp operand order in checkHitFace (https://decomp.me/scratch/PiCt7)
bool KCollisionServer::KCHitSphere(const KCPrismData* data, const KCPrismHeader* header,
                                   const KCFxyz* position, f32 radius, f32 thicknessScale,
                                   f32* outDistance, u8* outLocation) {
    f32 thickness = header->thickness;
    *outLocation = KCLocation_None;

    auto* vertex = getVertexData(data->vertexIndex, header);
    sead::Vector3f checkOffset = *position - *vertex;

    auto* edge1 = getEdgeNormal1(data, header);

    f32 distEdge1 = checkOffset.dot(*edge1);
    if (distEdge1 >= radius)
        return false;

    auto* edge2 = getEdgeNormal2(data, header);

    f32 distEdge2 = checkOffset.dot(*edge2);
    if (distEdge2 >= radius)
        return false;

    auto* edge3 = getEdgeNormal3(data, header);

    f32 distEdge3 = checkOffset.dot(*edge3) - data->length;
    if (distEdge3 >= radius)
        return false;

    auto* face = getFaceNormal(data, header);
    f32 distFace = checkOffset.dot(*face);

    *outDistance = radius - distFace;
    if (*outDistance < 0.0f)
        return false;

    f32 radiusSq = radius * radius;
    f32 maxDepth = thickness * thicknessScale;
    f32 cos;
    f32 cornerDistSq;
    f32 cornerDist;
    sead::Vector3f cornerVec;

    if (distEdge1 > distEdge2) {
        if (distEdge1 > distEdge3) {
            if (distEdge1 <= 0.0f)
                return checkHitFace(outDistance, outLocation, maxDepth);

            if (distEdge2 > distEdge3) {
                cos = edge1->dot(*edge2);

                if (distEdge1 * cos > distEdge2)
                    goto hitEdge1;

                goto hitCorner12;
            }

            cos = edge1->dot(*edge3);

            if (distEdge1 * cos > distEdge3)
                goto hitEdge1;

            goto hitCorner31;
        }

        goto major3;
    }

    if (distEdge2 > distEdge3) {
        if (distEdge2 <= 0.0f)
            return checkHitFace(outDistance, outLocation, maxDepth);

        if (distEdge3 > distEdge1) {
            cos = edge2->dot(*edge3);

            if (distEdge2 * cos > distEdge3)
                goto hitEdge2;

            goto hitCorner23;
        }

        cos = edge2->dot(*edge1);

        if (distEdge2 * cos > distEdge1)
            goto hitEdge2;

        goto hitCorner12;
    }

major3:
    if (distEdge3 <= 0.0f)
        return checkHitFace(outDistance, outLocation, maxDepth);

    if (distEdge1 > distEdge2) {
        cos = edge3->dot(*edge1);

        if (distEdge3 * cos > distEdge1)
            goto hitEdge3;

        goto hitCorner31;
    }

    cos = edge3->dot(*edge2);

    if (distEdge3 * cos > distEdge2)
        goto hitEdge3;

    goto hitCorner23;

hitEdge1:
    if (distEdge1 > distFace)
        return false;

    *outDistance = radiusSq - distEdge1 * distEdge1;
    *outLocation = KCLocation_Edge1;
    goto calcDepth;

hitEdge2:
    if (distEdge2 > distFace)
        return false;

    *outDistance = radiusSq - distEdge2 * distEdge2;
    *outLocation = KCLocation_Edge2;
    goto calcDepth;

hitEdge3:
    if (distEdge3 > distFace)
        return false;

    *outDistance = radiusSq - distEdge3 * distEdge3;
    *outLocation = KCLocation_Edge3;
    goto calcDepth;

hitCorner12:
    calcCornerVec(&cornerVec, distEdge1, distEdge2, edge1, edge2, cos);
    *outLocation = KCLocation_Corner12;
    goto hitCorner;

hitCorner23:
    calcCornerVec(&cornerVec, distEdge2, distEdge3, edge2, edge3, cos);
    *outLocation = KCLocation_Corner23;
    goto hitCorner;

hitCorner31:
    calcCornerVec(&cornerVec, distEdge3, distEdge1, edge3, edge1, cos);
    *outLocation = KCLocation_Corner31;

hitCorner:
    cornerDistSq = cornerVec.squaredLength();
    cornerDist = sead::Mathf::sqrt(cornerDistSq);

    if (cornerDist >= radius || cornerDist * (sead::numbers::sqrt2_v<f32> / 2.0f) > distFace) {
        *outLocation = KCLocation_None;
        return false;
    }

    *outDistance = radiusSq - cornerDistSq;

calcDepth:
    *outDistance = sead::Mathf::sqrt(sead::Mathf::clampMin(*outDistance, 0.0f)) - distFace;

    if (*outDistance < 0.0f) {
        *outLocation = KCLocation_None;
        return false;
    }

    return true;
}

// NON_MATCHING: Stack layout/Instruction ordering and probably some compiler jank
// (https://decomp.me/scratch/EHpGC)
// IMPORTANT: Forward declarations and their order/location matter!
const KCPrismData* KCollisionServer::checkArrow(const sead::Vector3f& position,
                                                const sead::Vector3f& direction,
                                                HitInfoBuffer* outHitInfos, u32* outHitNum,
                                                u32 maxHitNum) const {
    if (direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f)
        return nullptr;

    u32 hitNum = 0;
    const KCPrismData* bestPrism = nullptr;

    sead::Vector3f dir = direction;
    f32 length;
    separateScalarAndDirection(&length, &dir, dir);

    if (isNearZero(dir))
        return nullptr;

    sead::Vector3f areaPos;
    sead::Vector3u block;
    f32 startT;
    sead::Vector3f hitPoint;
    const KCPrismHeader* header;
    sead::Vector3u areaSize;
    f32 edge;
    for (s32 modelIdx = 0; modelIdx < getNumInnerKcl(); modelIdx++) {
        header = getV1Header(modelIdx);

        objectSpaceToAreaOffsetSpaceV3f(&areaPos, position, header);
        setUsingCast(&block, areaPos);

        if (isInsideMinMaxInAreaOffsetSpace(block, header)) {
            startT = 0.0f;
            hitPoint.set(areaPos);
        } else {
            getAreaSpaceSize(&areaSize, header);

            if (dir.x != 0.0f) {
                edge = 0.0f < dir.x ? 0.0f : areaSize.x;
                startT = (edge - areaPos.x) / dir.x;

                if (startT >= 0.0f && startT <= length) {
                    hitPoint = areaPos + dir * startT;
                    setUsingCast(&block, hitPoint);

                    if (isInsideMinMaxInAreaOffsetSpace(block, header))
                        goto startSearch;
                }
            }

            if (dir.y != 0.0f) {
                edge = 0.0f < dir.y ? 0.0f : areaSize.y;
                startT = (edge - areaPos.y) / dir.y;

                if (startT >= 0.0f && startT <= length) {
                    hitPoint = areaPos + dir * startT;
                    setUsingCast(&block, hitPoint);

                    if (isInsideMinMaxInAreaOffsetSpace(block, header))
                        goto startSearch;
                }
            }

            if (dir.z != 0.0f) {
                edge = 0.0f < dir.z ? 0.0f : areaSize.z;
                startT = (edge - areaPos.z) / dir.z;

                if (startT >= 0.0f && startT <= length) {
                    hitPoint = areaPos + dir * startT;
                    setUsingCast(&block, hitPoint);

                    if (isInsideMinMaxInAreaOffsetSpace(block, header))
                        goto startSearch;
                }
            }

            continue;
        }

    startSearch:
        const KCPrismData* data;
        const u16* prismList;
        s32 shift;
        s32 deltaPosX, deltaPosY, deltaPosZ;
        s32 deltaNegX, deltaNegY, deltaNegZ;
        s32 *deltaX, *deltaY, *deltaZ;
        u32 blockSize, blockMask;
        s32 stepX, stepY, stepZ;
        f32 bestDist;
        KCHitInfo hitInfo;

        f32 rateX, rateY, rateZ, minRate;
        f32 accumT;
        accumT = startT;

        if (dir.x < 0.0f) {
            deltaX = &deltaNegX;
            stepX = -1;
        } else {
            deltaX = &deltaPosX;
            stepX = 1;
        }

        if (dir.y < 0.0f) {
            deltaY = &deltaNegY;
            stepY = -1;
        } else {
            deltaY = &deltaPosY;
            stepY = 1;
        }

        if (dir.z < 0.0f) {
            deltaZ = &deltaNegZ;
            stepZ = -1;
        } else {
            deltaZ = &deltaPosZ;
            stepZ = 1;
        }

        do {
            prismList = searchBlock(&shift, block, header);
            blockSize = 1 << shift;
            blockMask = blockSize - 1;

            deltaPosX = blockSize - (blockMask & block.x);
            deltaPosY = blockSize - (blockMask & block.y);
            deltaPosZ = blockSize - (blockMask & block.z);
            deltaNegX = -(blockMask & block.x);
            deltaNegY = -(blockMask & block.y);
            deltaNegZ = -(blockMask & block.z);

            if (*deltaX == 0)
                *deltaX = stepX;

            if (*deltaY == 0)
                *deltaY = stepY;

            if (*deltaZ == 0)
                *deltaZ = stepZ;

            hitInfo.header = nullptr;
            hitInfo.data = nullptr;
            hitInfo.distance = 0.0f;
            hitInfo.location = 0;

            bestDist = 1.0f;

            while (*(++prismList) != 0xFFFF) {
                data = getPrismData(*prismList, header);
                hitInfo.data = data;
                if (data->length <= 0.0f)
                    continue;

                f32 dist;
                if (!KCHitArrow(data, header, position, direction, &dist, &hitInfo.location))
                    continue;

                hitInfo.distance = dist;

                if (outHitInfos) {
                    hitInfo.header = header;
                    outHitInfos->pushBack(hitInfo);
                    hitNum++;

                    if (hitInfo.distance < bestDist) {
                        bestDist = hitInfo.distance;
                        bestPrism = data;
                    }

                    if (hitNum == maxHitNum) {
                        if (outHitNum)
                            *outHitNum = hitNum;

                        return bestPrism;
                    }
                } else {
                    if (!(hitInfo.distance < bestDist))
                        continue;

                    bestDist = hitInfo.distance;
                    bestPrism = data;
                }
            }

            if (outHitInfos == nullptr && bestPrism != nullptr)
                break;

            rateX = isNearZero(dir.x) ? 1.0e9f : *deltaX / dir.x;
            rateY = isNearZero(dir.y) ? 1.0e9f : *deltaY / dir.y;
            rateZ = isNearZero(dir.z) ? 1.0e9f : *deltaZ / dir.z;

            minRate = rateX;
            if (rateY < minRate)
                minRate = rateY;
            if (rateZ < minRate)
                minRate = rateZ;

            if (length - accumT <= minRate)
                break;

            hitPoint += minRate * dir;
            accumT += minRate;
            setUsingCast(&block, hitPoint);

            if (!isInsideMinMaxInAreaOffsetSpace(block, header))
                break;
        } while (accumT < length);
    }

    if (outHitNum != nullptr)
        *outHitNum = hitNum;

    return bestPrism;
}

void KCollisionServer::objectSpaceToAreaOffsetSpaceV3f(sead::Vector3f* outAreaSpace,
                                                       const sead::Vector3f& objectSpace,
                                                       const KCPrismHeader* header) const {
    *outAreaSpace = objectSpace - header->octreeOrigin;
}

bool KCollisionServer::isInsideMinMaxInAreaOffsetSpace(const sead::Vector3u& block,
                                                       const KCPrismHeader* header) const {
    if ((block.x & header->widthMask.x) != 0)
        return false;

    if ((block.y & header->widthMask.y) != 0)
        return false;

    if ((block.z & header->widthMask.z) != 0)
        return false;

    return true;
}

bool KCollisionServer::KCHitArrow(const KCPrismData* data, const KCPrismHeader* header,
                                  const sead::Vector3f& position, const sead::Vector3f& direction,
                                  f32* outDistance, u8* outLocation) const {
    auto* vertex = getVertexData(data->vertexIndex, header);
    auto* faceNormal = getFaceNormal(data, header);

    sead::Vector3f checkOffset = position - *vertex;

    f32 posDot = faceNormal->dot(checkOffset);
    if (posDot <= 0.0f) {
        // Check position is behind the prism
        *outLocation = KCLocation_None;
        return false;
    }

    f32 dirDot = faceNormal->dot(direction);
    if (posDot + dirDot > 0.0f) {
        // Check direction points away from the prism
        *outLocation = KCLocation_None;
        return false;
    }

    f32 rate = posDot / -dirDot;
    sead::Vector3f hitPos;
    hitPos.setScaleAdd(rate, direction, checkOffset);

    bool isOnEdge1 = false;
    bool isOnEdge2 = false;
    bool isOnEdge3 = false;

    f32 dist1 = hitPos.dot(*getEdgeNormal1(data, header));
    if (0.01f < dist1) {
        *outLocation = KCLocation_None;
        return false;
    }
    if (dist1 >= 0.0f && dist1 <= 0.01f)
        isOnEdge1 = true;

    f32 dist2 = hitPos.dot(*getEdgeNormal2(data, header));
    if (0.01f < dist2) {
        *outLocation = KCLocation_None;
        return false;
    }
    if (dist2 >= 0.0f && dist2 <= 0.01f)
        isOnEdge2 = true;

    f32 dist3 = hitPos.dot(*getEdgeNormal3(data, header));
    if ((data->length + 0.01f) < dist3) {
        *outLocation = KCLocation_None;
        return false;
    }
    if (dist3 >= 0.0f && dist3 <= 0.01f)
        isOnEdge3 = true;

    *outDistance = rate;

    if (isOnEdge1) {
        if (isOnEdge2)
            if (isOnEdge3)
                *outLocation = KCLocation_Face;
            else
                *outLocation = KCLocation_Corner12;
        else if (isOnEdge3)
            *outLocation = KCLocation_Corner31;
        else
            *outLocation = KCLocation_Edge1;

    } else if (isOnEdge2) {
        if (isOnEdge3)
            *outLocation = KCLocation_Corner23;
        else
            *outLocation = KCLocation_Edge2;
    } else if (isOnEdge3) {
        *outLocation = KCLocation_Edge3;
    } else {
        *outLocation = KCLocation_Face;
    }

    return true;
}

// NON_MATCHING: Same as checkSphere (https://decomp.me/scratch/zZcCu)
u32 KCollisionServer::checkSphereForPlayer(const KCFxyz* position, f32 radius, f32 thicknessScale,
                                           u32 maxHitNum, HitInfoBuffer* outHitInfos) {
    KCHitInfo hitInfo;
    hitInfo.header = nullptr;
    hitInfo.data = nullptr;
    hitInfo.distance = 0.0f;
    hitInfo.location = KCLocation_None;

    sead::Vector3f posMax;
    posMax.x = position->x + radius;
    posMax.y = position->y + radius;
    posMax.z = position->z + radius;

    sead::Vector3f posMin;
    posMin.x = position->x - radius;
    posMin.y = position->y - radius;
    posMin.z = position->z - radius;

    const u16* prismList;
    const u16* nextSkipList = nullptr;
    const u16* skipList = nullptr;

    u32 hitNum = 0;
    for (s32 i = 0; i < getNumInnerKcl(); i++) {
        auto* header = getV1Header(i);

        sead::Vector3u blockMin, blockMax;
        if (!outCheckAndCalcArea(&blockMin, &blockMax, posMin, posMax, header))
            continue;

        u32 z = blockMin.z;
        do {
            s32 stepZ = 1000000;

            u32 y = blockMin.y;
            do {
                s32 maxRemY = 0;
                s32 stepY = 1000000;

                u32 x = blockMin.x;
                do {
                    s32 shift;
                    sead::Vector3u block(x, y, z);
                    prismList = searchBlock(&shift, block, header);

                    s32 blockSize = 1 << shift;
                    s32 blockMask = blockSize - 1;

                    s32 remX = blockSize - (blockMask & x);
                    s32 remY = blockSize - (blockMask & y);
                    s32 remZ = blockSize - (blockMask & z);

                    if (remZ < stepZ)
                        stepZ = remZ;
                    if (remY < stepY)
                        stepY = remY;

                    if (remY > maxRemY && prismList[1] != 0) {
                        maxRemY = remY;
                        nextSkipList = prismList;
                    }

                    if (!skipList || prismList != skipList) {
                        while (*(++prismList) != 0xffff) {
                            auto* data = getPrismData(*prismList, header);
                            hitInfo.data = data;

                            if (data->length <= 0.0f)
                                continue;

                            bool alreadyChecked = false;
                            for (auto it = outHitInfos->begin(); it != outHitInfos->end(); ++it) {
                                if (it->data == data) {
                                    alreadyChecked = true;
                                    break;
                                }
                            }
                            if (alreadyChecked)
                                continue;

                            if (!KCHitSphereForPlayer(data, header, position, radius,
                                                      thicknessScale, &hitInfo.distance,
                                                      &hitInfo.location))
                                continue;

                            if (hitNum < maxHitNum) {
                                hitInfo.header = header;
                                outHitInfos->pushBack(hitInfo);
                                hitNum++;
                            }
                        }
                    }

                    x += remX;
                } while (x <= blockMax.x);

                skipList = nextSkipList;
                y += stepY;
            } while (y <= blockMax.y);

            z += stepZ;
        } while (z <= blockMax.z);
    }

    return hitNum;
}

// NON_MATCHING: Same as KCHitSphere (https://decomp.me/scratch/Zhovu)
bool KCollisionServer::KCHitSphereForPlayer(const KCPrismData* data, const KCPrismHeader* header,
                                            const KCFxyz* position, f32 radius, f32 thicknessScale,
                                            f32* outDistance, u8* outLocation) {
    f32 thickness = header->thickness;
    *outLocation = KCLocation_None;

    auto* vertex = getVertexData(data->vertexIndex, header);
    sead::Vector3f checkOffset = *position - *vertex;

    auto* edge1 = getEdgeNormal1(data, header);

    f32 distEdge1 = checkOffset.dot(*edge1);
    if (distEdge1 >= radius)
        return false;

    auto* edge2 = getEdgeNormal2(data, header);

    f32 distEdge2 = checkOffset.dot(*edge2);
    if (distEdge2 >= radius)
        return false;

    auto* edge3 = getEdgeNormal3(data, header);

    f32 distEdge3 = checkOffset.dot(*edge3) - data->length;
    if (distEdge3 >= radius)
        return false;

    auto* face = getFaceNormal(data, header);
    f32 distFace = checkOffset.dot(*face);

    *outDistance = radius - distFace;
    if (*outDistance < 0.0f)
        return false;

    f32 radiusSq = radius * radius;
    f32 maxDepth = thickness * thicknessScale;
    f32 cos;
    f32 cornerDistSq;
    f32 cornerDist;
    sead::Vector3f cornerVec;

    if (distEdge1 > distEdge2) {
        if (distEdge1 > distEdge3) {
            if (distEdge1 <= 0.0f)
                return checkHitFace(outDistance, outLocation, maxDepth);

            if (distEdge2 > distEdge3) {
                cos = edge1->dot(*edge2);

                if (distEdge1 * cos > distEdge2)
                    goto hitEdge1;

                goto hitCorner12;
            }

            cos = edge1->dot(*edge3);

            if (distEdge1 * cos > distEdge3)
                goto hitEdge1;

            goto hitCorner31;
        }

        goto major3;
    }

    if (distEdge2 > distEdge3) {
        if (distEdge2 <= 0.0f)
            return checkHitFace(outDistance, outLocation, maxDepth);

        if (distEdge3 > distEdge1) {
            cos = edge2->dot(*edge3);

            if (distEdge2 * cos > distEdge3)
                goto hitEdge2;

            goto hitCorner23;
        }

        cos = edge2->dot(*edge1);

        if (distEdge2 * cos > distEdge1)
            goto hitEdge2;

        goto hitCorner12;
    }

major3:
    if (distEdge3 <= 0.0f)
        return checkHitFace(outDistance, outLocation, maxDepth);

    if (distEdge1 > distEdge2) {
        cos = edge3->dot(*edge1);

        if (distEdge3 * cos > distEdge1)
            goto hitEdge3;

        goto hitCorner31;
    }

    cos = edge3->dot(*edge2);

    if (distEdge3 * cos > distEdge2)
        goto hitEdge3;

    goto hitCorner23;

hitEdge1:
    if (distEdge1 > distFace)
        return false;

    *outDistance = radiusSq - distEdge1 * distEdge1;
    *outLocation = KCLocation_Edge1;
    goto calcDepth;

hitEdge2:
    if (distEdge2 > distFace)
        return false;

    *outDistance = radiusSq - distEdge2 * distEdge2;
    *outLocation = KCLocation_Edge2;
    goto calcDepth;

hitEdge3:
    if (distEdge3 > distFace)
        return false;

    *outDistance = radiusSq - distEdge3 * distEdge3;
    *outLocation = KCLocation_Edge3;
    goto calcDepth;

hitCorner12:
    calcCornerVec(&cornerVec, distEdge1, distEdge2, edge1, edge2, cos);
    *outLocation = KCLocation_Corner12;
    goto hitCorner;

hitCorner23:
    calcCornerVec(&cornerVec, distEdge2, distEdge3, edge2, edge3, cos);
    *outLocation = KCLocation_Corner23;
    goto hitCorner;

hitCorner31:
    calcCornerVec(&cornerVec, distEdge3, distEdge1, edge3, edge1, cos);
    *outLocation = KCLocation_Corner31;

hitCorner:
    cornerDistSq = cornerVec.squaredLength();
    cornerDist = sead::Mathf::sqrt(cornerDistSq);

    if (cornerDist >= radius || cornerDist * (sead::numbers::sqrt2_v<f32> / 2.0f) > distFace) {
        *outLocation = KCLocation_None;
        return false;
    }

    *outDistance = radiusSq - cornerDistSq;

calcDepth:
    *outDistance = sead::Mathf::sqrt(sead::Mathf::clampMin(*outDistance, 0.0f)) - distFace;

    if (*outDistance < 0.0f) {
        *outLocation = KCLocation_None;
        return false;
    }

    return true;
}

// NON_MATCHING: Float min/max calculation and instruction ordering
// (https://decomp.me/scratch/RWeEi)
u32 KCollisionServer::checkDisk(const KCFxyz* position, f32 radius, f32 halfHeight,
                                const sead::Vector3f& normal, f32 thicknessScale, u32 maxHitNum,
                                HitInfoBuffer* outHitInfos) {
    KCHitInfo hitInfo;
    hitInfo.header = nullptr;
    hitInfo.data = nullptr;
    hitInfo.distance = 0.0f;
    hitInfo.location = 0;

    sead::Vector3f extent = normal * halfHeight;
    sead::Vector3f posMax = *position + extent;
    sead::Vector3f posMin = *position - extent;

    if (posMax.x < posMin.x) {
        f32 newMin = posMax.x;
        f32 newMax = posMin.x;

        posMin.x = newMin;
        posMax.x = newMax;
    }
    if (posMax.y < posMin.y) {
        f32 newMin = posMax.y;
        f32 newMax = posMin.y;

        posMin.y = newMin;
        posMax.y = newMax;
    }
    if (posMax.z < posMin.z) {
        f32 newMin = posMax.z;
        f32 newMax = posMin.z;

        posMin.z = newMin;
        posMax.z = newMax;
    }

    const u16* prismList;
    const u16* nextSkipList = nullptr;
    const u16* skipList = nullptr;
    u32 hitNum = 0;

    for (s32 i = 0; i < getNumInnerKcl(); i++) {
        const KCPrismHeader* header = getV1Header(i);

        sead::Vector3u blockMin;
        sead::Vector3u blockMax;
        if (!outCheckAndCalcArea(&blockMin, &blockMax, posMin, posMax, header))
            continue;

        u32 z = blockMin.z;
        do {
            s32 stepZ = 1000000;

            u32 y = blockMin.y;
            do {
                s32 maxRemY = 0;
                s32 stepY = 1000000;

                u32 x = blockMin.x;
                do {
                    s32 shift;
                    sead::Vector3u block(x, y, z);
                    prismList = searchBlock(&shift, block, header);

                    s32 blockSize = 1 << shift;
                    s32 blockMask = blockSize - 1;

                    s32 remX = blockSize - (blockMask & x);
                    s32 remY = blockSize - (blockMask & y);
                    s32 remZ = blockSize - (blockMask & z);

                    if (stepZ < remZ)
                        stepZ = remZ;
                    if (stepY < remZ)
                        stepY = remZ;

                    if (remY > maxRemY && prismList[1] != 0) {
                        maxRemY = remY;
                        nextSkipList = prismList;
                    }

                    if (!skipList || prismList != skipList) {
                        while (*(++prismList) != 0xffff) {
                            auto* data = getPrismData(*prismList, header);
                            hitInfo.data = data;

                            if (data->length <= 0.0f)
                                continue;

                            bool alreadyChecked = false;
                            for (auto it = outHitInfos->begin(); it != outHitInfos->end(); ++it) {
                                if (it->data == data) {
                                    alreadyChecked = true;
                                    break;
                                }
                            }
                            if (alreadyChecked)
                                continue;

                            if (!KCHitDisk(data, header, position, radius, thicknessScale,
                                           halfHeight, normal, &hitInfo.distance,
                                           &hitInfo.location))
                                continue;

                            if (hitNum < maxHitNum) {
                                hitInfo.header = header;
                                outHitInfos->pushBack(hitInfo);
                                hitNum++;
                            }
                        }
                    }

                    x += remX;
                } while (x <= blockMax.x);

                skipList = nextSkipList;
                y += stepY;
            } while (y <= blockMax.y);

            z += stepZ;
        } while (z <= blockMax.z);
    }

    return hitNum;
}

// NON_MATCHING: Non-matching calcPosLocal is inlined and other major issues
// (https://decomp.me/scratch/902GP)
bool KCollisionServer::KCHitDisk(const KCPrismData* data, const KCPrismHeader* header,
                                 const KCFxyz* position, f32 radius, f32 thicknessScale,
                                 f32 halfHeight, const sead::Vector3f& normal, f32* outDistance,
                                 u8* outLocation) {
    sead::Vector3f vertex;
    sead::Vector3f pos1;
    sead::Vector3f pos2;
    calcPosLocal(&vertex, data, 0, header);
    calcPosLocal(&pos1, data, 1, header);
    calcPosLocal(&pos2, data, 2, header);
    sead::Vector3f pos = *position;
    sead::Vector3f relPos0 = vertex - pos;
    sead::Vector3f relPos1 = pos1 - pos;
    sead::Vector3f relPos2 = pos2 - pos;
    f32 height0 = relPos0.dot(normal);
    f32 height1 = relPos1.dot(normal);
    f32 height2 = normal.dot(relPos2);
    *outLocation = KCLocation_None;
    *outDistance = 0.0f;

    if (height0 < -halfHeight && height1 < -halfHeight && height2 < -halfHeight)
        return false;

    if (height0 > halfHeight && height1 > halfHeight && height2 > halfHeight)
        return false;

    sead::Vector3f top = pos + normal * halfHeight;
    sead::Vector3f bottom = pos + normal * -halfHeight;
    sead::Vector3f relTop = top - vertex;
    sead::Vector3f relBottom = bottom - vertex;

    const sead::Vector3f* edge1 = getEdgeNormal1(data, header);
    f32 topDist1 = relTop.dot(*edge1);
    f32 bottomDist1 = relBottom.dot(*edge1);
    f32 radius1 =
        sinf(acosf(sead::Mathf::clamp(sead::Mathf::abs(normal.dot(*edge1)), -1.0f, 1.0f))) * radius;

    if (topDist1 >= radius1 && bottomDist1 >= radius1)
        return false;

    f32 minDist1 = topDist1 < bottomDist1 ? topDist1 : bottomDist1;

    const sead::Vector3f* edge2 = getEdgeNormal2(data, header);
    f32 topDist2 = relTop.dot(*edge2);
    f32 bottomDist2 = relBottom.dot(*edge2);
    f32 radius2 =
        sinf(acosf(sead::Mathf::clamp(sead::Mathf::abs(normal.dot(*edge2)), -1.0f, 1.0f))) * radius;

    if (topDist2 >= radius2 && bottomDist2 >= radius2)
        return false;

    f32 minDist2 = topDist2 < bottomDist2 ? topDist2 : bottomDist2;

    const sead::Vector3f* edge3 = getEdgeNormal3(data, header);
    f32 topDist3 = relTop.dot(*edge3) - data->length;
    f32 bottomDist3 = relBottom.dot(*edge3) - data->length;
    f32 radius3 =
        sinf(acosf(sead::Mathf::clamp(sead::Mathf::abs(normal.dot(*edge3)), -1.0f, 1.0f))) * radius;

    if (topDist3 >= radius3 && bottomDist3 >= radius3)
        return false;

    f32 minDist3 = topDist3 < bottomDist3 ? topDist3 : bottomDist3;

    const sead::Vector3f* faceNormal = getFaceNormal(data, header);
    f32 topDistFace = relTop.dot(*faceNormal);
    f32 bottomDistFace = relBottom.dot(*faceNormal);
    f32 sinFace =
        sinf(acosf(sead::Mathf::clamp(sead::Mathf::abs(normal.dot(*faceNormal)), -1.0f, 1.0f)));

    if (bottomDistFace <= 0.0f && topDistFace <= 0.0f)
        return false;

    f32 radiusFace = sinFace * radius;

    if (bottomDistFace > radiusFace && topDistFace > radiusFace)
        return false;

    f32 minDistFace = topDistFace < bottomDistFace ? topDistFace : bottomDistFace;
    *outDistance = radiusFace - minDistFace;

    s32 edgeIndex =
        minDist1 > minDist2 ? (minDist1 > minDist3 ? 0 : 2) : (minDist2 > minDist3 ? 1 : 2);

    switch (edgeIndex) {
    case 0:
        if (minDist1 > 0.0f) {
            if (minDist1 > minDistFace)
                return false;

            if (sead::Mathf::sqrt(radius1 * radius1 - minDist1 * minDist1) - minDistFace < 0.0f)
                return false;
        }

        break;
    case 1:
        if (minDist2 > 0.0f) {
            if (minDist2 > minDistFace)
                return false;

            if (sead::Mathf::sqrt(radius2 * radius2 - minDist2 * minDist2) - minDistFace < 0.0f)
                return false;
        }

        break;
    case 2:
        if (minDist3 > 0.0f) {
            if (minDist3 > minDistFace)
                return false;

            if (sead::Mathf::sqrt(radius3 * radius3 - minDist3 * minDist3) - minDistFace < 0.0f)
                return false;
        }

        break;
    }

    *outLocation = KCLocation_Face;
    return true;
}

void KCollisionServer::searchPrism(KCFxyz* position, f32 radius, PrismCallback& callback) {
    sead::Vector3f extent(radius, radius, radius);

    sead::Vector3f max;
    max.setAdd(*position, extent);
    sead::Vector3f min;
    min.setSub(*position, extent);

    searchPrismMinMax(min, max, callback);
}

// NON_MATCHING: Instruction ordering (https://decomp.me/scratch/mnoCg)
void KCollisionServer::searchPrismMinMax(const sead::Vector3f& min, const sead::Vector3f& max,
                                         PrismCallback& callback) {
    const u16* lastPrismList = nullptr;
    const u16* skipPrismList = nullptr;

    for (s32 i = 0; i < getNumInnerKcl(); i++) {
        const KCPrismHeader* header = getV1Header(i);
        sead::Vector3u blockMin;
        sead::Vector3u blockMax;

        if (!outCheckAndCalcArea(&blockMin, &blockMax, min, max, header))
            continue;

        u32 z = blockMin.z;

        do {
            s32 stepZ = 1000000;

            u32 y = blockMin.y;

            do {
                s32 maxStepY = 0;
                s32 stepY = 1000000;

                u32 x = blockMin.x;

                do {
                    sead::Vector3u block(x, y, z);
                    s32 shift;
                    const u16* prismList = searchBlock(&shift, block, header);
                    s32 blockSize = 1 << shift;
                    s32 blockMask = blockSize - 1;
                    s32 offsetX = blockMask & x;
                    s32 offsetY = blockMask & y;
                    s32 offsetZ = blockMask & z;
                    stepZ = sead::Mathi::min(blockSize - offsetZ, stepZ);
                    stepY = sead::Mathi::min(blockSize - offsetY, stepY);

                    if (blockSize - offsetY > maxStepY && prismList[1] != 0) {
                        maxStepY = blockSize - offsetY;
                        lastPrismList = prismList;
                    }

                    s32 stepX = blockSize - offsetX;

                    if (skipPrismList == nullptr || skipPrismList != prismList) {
                        while (*++prismList != 0xffff) {
                            KCPrismData* data =
                                const_cast<KCPrismData*>(getPrismData(*prismList, header));

                            if (data->length <= 0.0f)
                                continue;

                            callback.invoke(data, header);
                        }
                    }

                    x += stepX;
                } while (x <= blockMax.x);

                skipPrismList = lastPrismList;
                y += stepY;
            } while (y <= blockMax.y);

            z += stepZ;
        } while (z <= blockMax.z);
    }
}

void KCollisionServer::searchPrismArrow(const sead::Vector3f& position,
                                        const sead::Vector3f& direction, PrismCallback& callback) {
    sead::Vector3f target = position + direction;

    sead::Vector3f max(sead::Mathf::max(position.x, target.x),
                       sead::Mathf::max(position.y, target.y),
                       sead::Mathf::max(position.z, target.z));
    sead::Vector3f min(sead::Mathf::min(position.x, target.x),
                       sead::Mathf::min(position.y, target.y),
                       sead::Mathf::min(position.z, target.z));

    searchPrismMinMax(min, max, callback);
}

// NON_MATCHING: Instruction ordering (https://decomp.me/scratch/oS0wM)
void KCollisionServer::searchPrismDisk(const sead::Vector3f& position, const sead::Vector3f& normal,
                                       f32 halfHeight, f32 radius, PrismCallback& callback) {
    f32 dotX = calcCylinderRadiusDot(sead::Vector3f::ex, normal, radius);
    f32 dotY = calcCylinderRadiusDot(sead::Vector3f::ey, normal, radius);
    f32 dotZ = calcCylinderRadiusDot(sead::Vector3f::ez, normal, radius);
    sead::Vector3f dot(dotX, dotY, dotZ);

    sead::Vector3f extent = normal * radius;
    sead::Vector3f diskMax = extent + position;
    sead::Vector3f diskMin = position - extent;

    sead::Vector3f max(sead::Mathf::max(diskMax.x, diskMin.x),
                       sead::Mathf::max(diskMax.y, diskMin.y),
                       sead::Mathf::max(diskMax.z, diskMin.z));
    sead::Vector3f min(sead::Mathf::min(diskMax.x, diskMin.x),
                       sead::Mathf::min(diskMax.y, diskMin.y),
                       sead::Mathf::min(diskMax.z, diskMin.z));

    max.x += dotX;
    max.y += dotY;
    max.z += dotZ;
    min.x -= dotX;
    min.y -= dotY;
    min.z -= dotZ;

    searchPrismMinMax(min, max, callback);
}

// NON_MATCHING: Trailing 'return true's at the end (https://decomp.me/scratch/Q73SB)
bool KCollisionServer::isParallelNormal(const KCPrismData* data,
                                        const KCPrismHeader* header) const {
    sead::Vector3f edge1 = *getEdgeNormal1(data, header);
    sead::Vector3f edge2 = *getEdgeNormal2(data, header);
    sead::Vector3f edge3 = *getEdgeNormal3(data, header);

    return edge1 == edge2 || edge1 == edge3 || edge2 == edge3 || edge1 == -edge2 ||
           edge1 == -edge3 || edge2 == -edge3;
}

const sead::Vector3f* KCollisionServer::getFaceNormal(const KCPrismData* data,
                                                      const KCPrismHeader* header) const {
    return getNormal(data->faceNormalIndex, header);
}

const sead::Vector3f* KCollisionServer::getEdgeNormal1(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->edgeNormalIndex[0], header);
}

const sead::Vector3f* KCollisionServer::getEdgeNormal2(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->edgeNormalIndex[1], header);
}

const sead::Vector3f* KCollisionServer::getEdgeNormal3(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->edgeNormalIndex[2], header);
}

// NON_MATCHING: https://decomp.me/scratch/k6cM4
bool KCollisionServer::KCHitDisc(const KCPrismData* data, const KCPrismHeader* header,
                                 const sead::Vector3f& position, const sead::Vector3f& normal,
                                 f32 radius, f32 angle, sead::Vector3f* outFixDir,
                                 f32* outDistance) {
    sead::Vector3f pos0 = *getVertexData(data->vertexIndex, header);
    KCFxyz faceNormal = *(getFaceNormal(data, header));
    KCFxyz edge2 = *(getEdgeNormal2(data, header));
    sead::Vector3f edge3 = *getEdgeNormal3(data, header);
    f32 prislength = data->length;
    KCFxyz edge1 = *(getEdgeNormal1(data, header));
    f32 cos = normal.dot(faceNormal);

    if (cos > cosf(angle))
        return false;

    if (cos < cosf(180.0f - angle))
        return false;

    if (cos == 1.0f)
        return false;

    if (cos == -1.0f)
        return false;

    KCFxyz cross1;
    calXvec(&edge2, &faceNormal, &cross1);
    sead::Vector3f pos1 =
        pos0 + cross1 * (prislength / sead::Mathf::max(cross1.dot(edge3), sead::Mathf::epsilon()));
    KCFxyz cross2;
    calXvec(&faceNormal, &edge1, &cross2);
    sead::Vector3f pos2 =
        pos0 + cross2 * (prislength / sead::Mathf::max(cross2.dot(edge3), sead::Mathf::epsilon()));

    f32 height0 = (pos0 - position).dot(normal);
    f32 height1 = (pos1 - position).dot(normal);
    f32 height2 = (pos2 - position).dot(normal);

    if (height2 <= 0.0f && height0 <= 0.0f && height1 <= 0.0f)
        return false;

    if (height2 > 0.0f && height0 > 0.0f && height1 > 0.0f)
        return false;

    sead::Vector3f crossPos[2];
    s32 crossNum = 0;

    if (height0 * height1 < 0.0f) {
        f32 rate0 = sead::Mathf::abs(height0);
        f32 rate1 = sead::Mathf::abs(height1);
        f32 invRate = 1.0f / (rate0 + rate1);
        crossPos[crossNum++] = pos0 * rate1 * invRate + pos1 * rate0 * invRate;
    }

    if (height1 * height2 < 0.0f) {
        f32 rate1 = sead::Mathf::abs(height1);
        f32 rate2 = sead::Mathf::abs(height2);
        f32 invRate = 1.0f / (rate1 + rate2);
        crossPos[crossNum++] = pos1 * rate2 * invRate + pos2 * rate1 * invRate;
    }

    if (height2 * height0 < 0.0f) {
        f32 rate2 = sead::Mathf::abs(height2);
        f32 rate0 = sead::Mathf::abs(height0);
        f32 invRate = 1.0f / (rate2 + rate0);
        crossPos[crossNum++] = pos2 * rate0 * invRate + pos0 * rate2 * invRate;
    }

    if (crossNum != 2)
        return false;

    KCFxyz edge;
    edge.set(crossPos[1] - crossPos[0]);
    KCFxyz cross;
    calXvec(static_cast<const KCFxyz*>(&normal), &edge, &cross);

    if (getFaceNormal(data, header)->dot(cross) < 0.0f) {
        cross.set(-cross);
        edge.set(-edge);
        sead::Vector3f tmp = crossPos[0];
        crossPos[0] = crossPos[1];
        crossPos[1] = tmp;
    }

    f32 length = cross.length();

    if (length == 0.0f)
        return false;

    f32 invLength = 1.0f / length;
    sead::Vector3f fixDir = cross * invLength;
    sead::Vector3f startDiff = position - crossPos[0];
    f32 height = fixDir.dot(startDiff);

    if (height < 0.0f)
        return false;

    sead::Vector3f edgeDir = edge * invLength;
    f32 edgeRate = edgeDir.dot(startDiff);

    if (edgeRate >= 0.0f && edgeRate <= length) {
        f32 dist = (crossPos[0] + edgeDir * edgeRate - position).length();

        if (dist >= radius)
            return false;

        *outFixDir = fixDir;
        *outDistance = radius - dist;
        return true;
    }

    sead::Vector3f endDiff = position - crossPos[1];
    f32 startDist = startDiff.length();
    f32 endDist = endDiff.length();

    if (startDist >= radius && endDist >= radius)
        return false;

    if (startDist > endDist) {
        sead::Vector3f dir = endDiff;
        tryNormalizeOrZero(&dir);

        if (dir.dot(fixDir) < 0.70710677f)
            return false;

        f32 endRate = edgeDir.dot(endDiff);
        f32 sq = radius * radius - endRate * endRate;
        if (sq > 0.0f)
            *outDistance = sead::Mathf::sqrt(sq) - sead::Mathf::abs(fixDir.dot(endDiff));
        else
            *outDistance = -sead::Mathf::abs(fixDir.dot(endDiff));
    } else {
        sead::Vector3f dir = startDiff;
        tryNormalizeOrZero(&dir);

        if (dir.dot(fixDir) < 0.70710677f)
            return false;

        f32 sq = radius * radius - edgeRate * edgeRate;
        if (sq > 0.0f)
            *outDistance = sead::Mathf::sqrt(sq) - sead::Mathf::abs(height);
        else
            *outDistance = -sead::Mathf::abs(height);
    }

    *outFixDir = fixDir;
    return true;
}

s32 KCollisionServer::toIndex(const KCPrismData* data, const KCPrismHeader* header) const {
    return data - getPrismData(0, header);
}

const sead::Vector3f* KCollisionServer::getNormal(u32 index, const KCPrismHeader* header) const {
    return &header->getNormals()[(s32)index];
}

void KCollisionServer::calXvec(const KCFxyz* a, const KCFxyz* b, KCFxyz* out) {
    out->x = a->z * b->y - a->y * b->z;
    out->y = a->x * b->z - a->z * b->x;
    out->z = a->y * b->x - a->x * b->y;
}

const KCFxyz* KCollisionServer::getVertexData(u32 index, const KCPrismHeader* header) const {
    return &header->getVertices()[(s32)index];
}

s32 KCollisionServer::getVertexNum(const KCPrismHeader* header) const {
    return getNormal(0, header) - getVertexData(0, header);
}

s32 KCollisionServer::getNormalNum(const KCPrismHeader* header) const {
    return getTriangleNum(header) * 4;
}

s32 KCollisionServer::getAttributeElementNum() const {
    return 0;
}

bool KCollisionServer::getAttributes(ByamlIter* destIter, u32 prismIndex,
                                     const KCPrismHeader* header) const {
    return getAttributes(destIter, getPrismData(prismIndex, header));
}

bool KCollisionServer::getAttributes(ByamlIter* destIter, const KCPrismData* data) const {
    return mAttributeIter->tryGetIterByIndex(destIter, data->collisionType);
}

void KCollisionServer::objectSpaceToAreaOffsetSpace(sead::Vector3u* outAreaSpace,
                                                    const sead::Vector3f& objectSpace,
                                                    const KCPrismHeader* header) const {
    outAreaSpace->x = static_cast<s32>(objectSpace.x - header->octreeOrigin.x);
    outAreaSpace->y = static_cast<s32>(objectSpace.y - header->octreeOrigin.y);
    outAreaSpace->z = static_cast<s32>(objectSpace.z - header->octreeOrigin.z);
}

void KCollisionServer::areaOffsetSpaceToObjectSpace(sead::Vector3f* outObjectSpace,
                                                    const sead::Vector3u& areaSpace,
                                                    const KCPrismHeader* header) const {
    sead::Vector3f areaPos(areaSpace.x, areaSpace.y, areaSpace.z);
    *outObjectSpace = header->octreeOrigin + areaPos;
}

bool KCollisionServer::doBoxCheck(const sead::Vector3f* position, const sead::Vector3f* size,
                                  sead::Vector3u* outBlockMin, sead::Vector3u* outBlockMax,
                                  const KCPrismHeader* header) {
    sead::Vector3f end = *position + *size;
    sead::Vector3f min;
    sead::Vector3f max;

    if (position->x >= end.x) {
        min.x = end.x;
        max.x = position->x;
    } else {
        max.x = end.x;
        min.x = position->x;
    }

    if (position->y >= end.y) {
        min.y = end.y;
        max.y = position->y;
    } else {
        max.y = end.y;
        min.y = position->y;
    }

    if (position->z >= end.z) {
        min.z = end.z;
        max.z = position->z;
    } else {
        max.z = end.z;
        min.z = position->z;
    }

    if (min.x == max.x) {
        min.x += -1.0f;
        max.x += 1.0f;
    }

    if (min.y == max.y) {
        min.y += -1.0f;
        max.y += 1.0f;
    }

    if (min.z == max.z) {
        min.z += -1.0f;
        max.z += 1.0f;
    }

    return outCheckAndCalcArea(outBlockMin, outBlockMax, min, max, header);
}

u32 KCollisionServer::calcAreaBlockOffset(const sead::Vector3u& block,
                                          const KCPrismHeader* header) const {
    s32 shift = header->blockWidthShift;
    return (((block.z >> shift) << header->areaXYWidthShift) |
            ((block.y >> shift) << header->areaXWidthShift) | (block.x >> shift))
           << 2;
}

u32 KCollisionServer::calcChildBlockOffset(const sead::Vector3u& block, s32 shift) {
    return ((((block.z >> shift) & 1) << 2) | (((block.y >> shift) & 1) << 1) |
            ((block.x >> shift) & 1))
           << 2;
}

u32 KCollisionServer::getBlockData(const u32* octree, u32 offset) {
    auto* data = static_cast<const u32*>(sead::PtrUtil::addOffset(octree, offset));
    return *data;
}
}  // namespace al
