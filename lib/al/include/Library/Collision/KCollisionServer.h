#pragma once

#include <container/seadPtrArray.h>
#include <container/seadRingBuffer.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

namespace al {
class ByamlIter;

struct KCPrismHeader {
    u32 mPositionsOffset;
    u32 mNormalsOffset;
    u32 mTrianglesOffset;
    u32 mOctreeOffset;
    f32 mThickness;
    sead::Vector3f mOctreeOrigin;
    sead::Vector3u mMask;
    sead::Vector3u mCoordShift;
    f32 mHitboxRadiusCap;
};
struct KCPrismData {
    f32 mLength;
    u16 mPosIndex;
    u16 mFaceNormalIndex;
    u16 mEdgeNormalIndex[3];
    u16 mCollisionType;
    u32 mTriIndex;
};
struct KCHitInfo;

class KCollisionServer {
public:
    KCollisionServer();

    void initKCollisionServer(void*, const void*);
    void setData(void*);
    const KCPrismHeader& getInnerKcl(s32) const;
    u32 getNumInnerKcl() const;
    const KCPrismHeader* getV1Header(s32) const;
    bool calcFarthestVertexDistance();
    s32 getTriangleNum(const KCPrismHeader*) const;
    const KCPrismData& getPrismData(u32, const KCPrismHeader*) const;
    bool isNearParallelNormal(const KCPrismData*, const KCPrismHeader*) const;
    bool isNanPrism(const KCPrismData*, const KCPrismHeader*) const;
    void calcPosLocal(sead::Vector3f*, const KCPrismData*, s32, const KCPrismHeader*) const;
    void getMinMax(sead::Vector3f*, sead::Vector3f*) const;
    void getAreaSpaceSize(sead::Vector3f*, const KCPrismHeader*) const;
    void getAreaSpaceSize(s32*, s32*, s32*, const KCPrismHeader*) const;
    void getAreaSpaceSize(sead::Vector3u*, const KCPrismHeader*) const;
    const KCPrismData* checkPoint(sead::Vector3f*, f32, f32*);
    const u8* searchBlock(s32*, const sead::Vector3u&, const KCPrismHeader*) const;
    s32 checkSphere(const sead::Vector3f*, f32, f32, u32, sead::FixedRingBuffer<KCHitInfo, 512>*);
    bool outCheckAndCalcArea(sead::Vector3u*, sead::Vector3u*, const sead::Vector3f&,
                             const sead::Vector3f&, const KCPrismHeader*) const;
    bool KCHitSphere(const KCPrismData*, const KCPrismHeader*, const sead::Vector3f*, f32, f32,
                     f32*, u8*);
    s32 checkArrow(const sead::Vector3f&, const sead::Vector3f&,
                   sead::FixedRingBuffer<KCHitInfo, 512>*, u32*, u32) const;
    void objectSpaceToAreaOffsetSpaceV3f(sead::Vector3f*, const sead::Vector3f&,
                                         const KCPrismHeader*) const;
    bool isInsideMinMaxInAreaOffsetSpace(const sead::Vector3u&, const KCPrismHeader*) const;
    bool KCHitArrow(const KCPrismData*, const KCPrismHeader*, const sead::Vector3f&,
                    const sead::Vector3f&, f32*, u8*);
    s32 checkSphereForPlayer(const sead::Vector3f*, f32, f32, u32,
                             sead::FixedRingBuffer<KCHitInfo, 512>*);
    bool KCHitSphereForPlayer(const KCPrismData*, const KCPrismHeader*, const sead::Vector3f*, f32,
                              f32, f32*, u8*);
    s32 checkDisk(const sead::Vector3f*, f32, f32, const sead::Vector3f&, f32, u32,
                  sead::FixedRingBuffer<KCHitInfo, 512>*);
    bool KCHitDisk(const KCPrismData*, const KCPrismHeader*, const sead::Vector3f*, f32, f32, f32,
                   const sead::Vector3f&, f32*, u8*);
    void searchPrism(sead::Vector3f*, f32,
                     sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>&);
    void searchPrismMinMax(const sead::Vector3f&, const sead::Vector3f&,
                           sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>&);
    void searchPrismArrow(const sead::Vector3f&, const sead::Vector3f&,
                          sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>&);
    void searchPrismDisk(const sead::Vector3f&, const sead::Vector3f&, f32, f32,
                         sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>&);
    bool isParallelNormal(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getFaceNormal(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal1(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal2(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getEdgeNormal3(const KCPrismData*, const KCPrismHeader*) const;
    bool KCHitDisc(const KCPrismData*, const KCPrismHeader*, const sead::Vector3f&,
                   const sead::Vector3f&, f32, f32, sead::Vector3f*, f32*);
    s32 toIndex(const KCPrismData*, const KCPrismHeader*) const;
    const sead::Vector3f& getNormal(u32, const KCPrismHeader*) const;

    static void calXvec(const sead::Vector3f*, const sead::Vector3f*, sead::Vector3f*);

    const sead::Vector3f& getVertexData(u32, const KCPrismHeader*) const;
    u32 getVertexNum(const KCPrismHeader*) const;
    s32 getNormalNum(const KCPrismHeader*) const;
    s32 getAttributeElementNum() const;
    bool getAttributes(ByamlIter*, u32, const KCPrismHeader*) const;
    bool getAttributes(ByamlIter*, const KCPrismData*) const;
    void objectSpaceToAreaOffsetSpace(sead::Vector3u*, const sead::Vector3f&,
                                         const KCPrismHeader*) const;
    void areaOffsetSpaceToObjectSpace(sead::Vector3f*, const sead::Vector3u&,
                                         const KCPrismHeader*) const;
    bool doBoxCheck(const sead::Vector3f*, const sead::Vector3f*, sead::Vector3u*, sead::Vector3u*,
                    const KCPrismHeader*);
    s32 calcAreaBlockOffset(const sead::Vector3u&, const KCPrismHeader*) const;

    static s32 calcChildBlockOffset(const sead::Vector3u&, s32);
    static u32 getBlockData(const u32*, u32);

private:
    sead::PtrArray<KCPrismHeader> mModelsData;
    void* mData = nullptr;
    ByamlIter* mIter = nullptr;
    void* mModelListData = nullptr;
    void* mOctreeData = nullptr;
    sead::Vector3u mCoordShift = {0,0,0};
    sead::Vector3u unkShiftedByCoordShift = {0,0,0};
    f32 mFarthestVertexDistance = 1.0f;
};

}  // namespace al
