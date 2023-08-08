#include "al/collision/KCollisionServer.h"
#include <math.h>

#include "al/collision/KCPrism.h"
#include "math/seadVector.h"

namespace al {

bool KCollisionServer::isNearParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const {

}

void KCollisionServer::calcPosLocal(sead::Vector3f* out, const KCPrismData* data, int vIndex,
                                    const KCPrismHeader* header) const {
    switch (vIndex) {
    case 0: {
        sead::Vector3f pos = getVertexData(data->mPosIndex, header);
        *out = pos;
        break;
    }
    case 2: {
        const sead::Vector3f& normalA = getEdgeNormal1(data, header);
        const sead::Vector3f& dirNormal = getFaceNormal(data, header);
        const sead::Vector3f& normalC = getEdgeNormal3(data, header);
        sead::Vector3f cross;
        calXvec(&normalA, &dirNormal, &cross);
        //cross.setCross(normalA, dirNormal);
        float factor = data->mLength / fmax(cross.dot(normalC), 0.00000011921f);
        const sead::Vector3f& pos = getVertexData(data->mPosIndex, header);
        *out = pos + (cross * factor);
        break;
    }
    case 1: {
        const sead::Vector3f& normalA = getFaceNormal(data, header);
        const sead::Vector3f& dirNormal = getEdgeNormal2(data, header);
        const sead::Vector3f& normalC = getEdgeNormal3(data, header);
        sead::Vector3f cross;
        calXvec(&normalA, &dirNormal, &cross);
        //cross.setCross(normalA, dirNormal);
        float factor = data->mLength / fmax(cross.dot(normalC), 0.00000011921f);
        const sead::Vector3f& pos = getVertexData(data->mPosIndex, header);
        *out = pos + (cross * factor);
        break;
    }
    default:
        *out = {0, 0, 0};
        break;
    }
}

const sead::Vector3f& KCollisionServer::getFaceNormal(const KCPrismData* data,
                                                      const KCPrismHeader* header) const {
    return getNormal(data->mFaceNormalIndex, header);
}
const sead::Vector3f& KCollisionServer::getEdgeNormal1(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[0], header);
}
const sead::Vector3f& KCollisionServer::getEdgeNormal2(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[1], header);
}
const sead::Vector3f& KCollisionServer::getEdgeNormal3(const KCPrismData* data,
                                                       const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[2], header);
}
const sead::Vector3f& KCollisionServer::getNormal(u32 index, const KCPrismHeader* header) const {
    return reinterpret_cast<const sead::Vector3f*>(reinterpret_cast<const char*>(header) +
                                                   header->mNormalsOffset)[(int)index];
}
void KCollisionServer::calXvec(const sead::Vector3f* a, const sead::Vector3f* b,
                               sead::Vector3f* out) {
    out->x = (a->z * b->y) - (a->y * b->z);
    out->y = (a->x * b->z) - (a->z * b->x);
    out->z = (a->y * b->x) - (a->x * b->y);
}
const sead::Vector3f& KCollisionServer::getVertexData(u32 index, const KCPrismHeader* header) const {
    return reinterpret_cast<const sead::Vector3f*>(reinterpret_cast<const char*>(header) + header->mPositionsOffset)[(int)index];
}
u32 KCollisionServer::getVertexNum(const KCPrismHeader* header) const {
    // somehow needs to be this weird to produce matching code
    //return ((header->mNormalsOffset - header->mPositionsOffset) >> 2) / 0x18;
    return 0xAAAAAAAB * ((header->mNormalsOffset - (u64)header->mPositionsOffset) >> 2);
}
u32 KCollisionServer::getNormalNum(const KCPrismHeader* header) const {
    return 4 * ((header->mOctreeOffset - (u64)header->mTrianglesOffset) / 0x14);
}

u32 KCollisionServer::getBlockData(const u32* data, u32 offset) {
    return *reinterpret_cast<const u32*>(reinterpret_cast<const char*>(data) + offset);
}

}  // namespace al
