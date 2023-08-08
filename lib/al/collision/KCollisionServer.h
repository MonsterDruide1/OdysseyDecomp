#pragma once

#include <math/seadVector.h>

namespace al {

struct KCPrismHeader;
struct KCPrismData;

class KCollisionServer {
public:
    bool isNearParallelNormal(const KCPrismData*, const KCPrismHeader*) const;
    void calcPosLocal(sead::Vector3f*, const KCPrismData*, int, const KCPrismHeader*) const;
    const sead::Vector3f& getFaceNormal(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal1(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal2(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal3(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getNormal(u32, const KCPrismHeader*) const;
    const sead::Vector3f& getVertexData(u32, const KCPrismHeader*) const;
    u32 getVertexNum(const KCPrismHeader*) const;
    u32 getNormalNum(const KCPrismHeader*) const;

    static void calXvec(const sead::Vector3f*, const sead::Vector3f*, sead::Vector3f*);
    static u32 getBlockData(const u32*, u32);
};

}
