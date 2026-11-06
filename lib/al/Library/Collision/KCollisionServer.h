#pragma once

#include <container/seadPtrArray.h>
#include <container/seadRingBuffer.h>
#include <math/seadBoundBox.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

namespace al {
class ByamlIter;
class CollisionParts;
class LiveActor;

using KCFxyz = sead::Vector3f;

struct KCFileHeader {
    u32 version;

    u32 octreeOffset;
    u32 modelOffsetsOffset;
    s32 modelNum;

    KCFxyz minCoordinate;
    KCFxyz maxCoordinate;
    sead::Vector3i areaWidthShift;

    u32 prismNum;
};

struct KCPrismData {
    f32 length;
    u16 vertexIndex;
    u16 faceNormalIndex;
    u16 edgeNormalIndex[3];
    u16 collisionType;
    u32 prismIndex;
};

struct KCPrismHeader {
    u32 verticesOffset;
    u32 normalsOffset;
    u32 prismsOffset;
    u32 octreeOffset;

    f32 thickness;
    sead::Vector3f octreeOrigin;
    sead::Vector3u widthMask;
    s32 blockWidthShift;
    s32 areaXWidthShift;
    s32 areaXYWidthShift;
    f32 hitboxRadiusCap;

    const KCFxyz* getVertices() const {
        return reinterpret_cast<const KCFxyz*>(uintptr_t(this) + verticesOffset);
    }

    const KCFxyz* getNormals() const {
        return reinterpret_cast<const KCFxyz*>(uintptr_t(this) + normalsOffset);
    }

    const KCPrismData* getPrisms() const {
        return reinterpret_cast<const KCPrismData*>(uintptr_t(this) + prismsOffset);
    }

    const u32* getOctree() const {
        return reinterpret_cast<const u32*>(uintptr_t(this) + octreeOffset);
    }
};

enum KCOctree : u32 {
    KCOctree_MaskOffset = (1u << 31) - 1u,
    KCOctree_FlagLeaf = 1u << 31,
};

enum KCLocation {
    KCLocation_None = 0,
    KCLocation_Face = 1,
    KCLocation_Edge1 = 2,
    KCLocation_Edge2 = 3,
    KCLocation_Edge3 = 4,
    KCLocation_Corner12 = 5,
    KCLocation_Corner23 = 6,
    KCLocation_Corner31 = 7,
};

struct KCHitInfo {
    const KCPrismHeader* header;
    const KCPrismData* data;
    f32 distance;
    u8 location;
};

class KCollisionServer {
public:
    using HitInfoBuffer = sead::FixedRingBuffer<KCHitInfo, 512>;
    using PrismCallback = sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>;

    KCollisionServer();

    void initKCollisionServer(void* data, const void* attributeData);
    void setData(void* data);

    const KCPrismHeader* getInnerKcl(s32 index) const;
    s32 getNumInnerKcl() const;

    const KCPrismHeader* getV1Header(s32 index) const;
    bool calcFarthestVertexDistance();
    u32 getTriangleNum(const KCPrismHeader* header) const;
    const KCPrismData* getPrismData(u32 index, const KCPrismHeader* header) const;

    bool isNearParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const;
    bool isNanPrism(const KCPrismData* data, const KCPrismHeader* header) const;

    void calcPosLocal(sead::Vector3f* outPos, const KCPrismData* data, s32 vertexIndex,
                      const KCPrismHeader* header) const;

    void getMinMax(sead::Vector3f* outMin, sead::Vector3f* outMax) const;
    void getAreaSpaceSize(sead::Vector3f* outSize, const KCPrismHeader* header) const;
    void getAreaSpaceSize(s32* outSizeX, s32* outSizeY, s32* outSizeZ,
                          const KCPrismHeader* header) const;
    void getAreaSpaceSize(sead::Vector3u* outSize, const KCPrismHeader* header) const;

    const KCPrismData* checkPoint(KCFxyz* position, f32 thicknessScale, f32* outDistance);
    const u16* searchBlock(s32* outShift, const sead::Vector3u& block,
                           const KCPrismHeader* header) const;

    u32 checkSphere(const KCFxyz* position, f32 radius, f32 thicknessScale, u32 maxHitNum,
                    HitInfoBuffer* outHitInfos);
    bool outCheckAndCalcArea(sead::Vector3u* outBlockMin, sead::Vector3u* outBlockMax,
                             const sead::Vector3f& positionMin, const sead::Vector3f& positionMax,
                             const KCPrismHeader* header) const;
    bool KCHitSphere(const KCPrismData* data, const KCPrismHeader* header, const KCFxyz* position,
                     f32 radius, f32 thicknessScale, f32* outDistance, u8* outLocation);

    const KCPrismData* checkArrow(const sead::Vector3f& position, const sead::Vector3f& direction,
                                  HitInfoBuffer* outHitInfos, u32* outHitNum, u32 maxHitNum) const;

    void objectSpaceToAreaOffsetSpaceV3f(sead::Vector3f* outAreaSpace,
                                         const sead::Vector3f& objectSpace,
                                         const KCPrismHeader* header) const;
    bool isInsideMinMaxInAreaOffsetSpace(const sead::Vector3u& block,
                                         const KCPrismHeader* header) const;

    bool KCHitArrow(const KCPrismData* data, const KCPrismHeader* header,
                    const sead::Vector3f& position, const sead::Vector3f& direction,
                    f32* outDistance, u8* outLocation) const;

    u32 checkSphereForPlayer(const KCFxyz* position, f32 radius, f32 thicknessScale, u32 maxHitNum,
                             HitInfoBuffer* outHitInfos);
    bool KCHitSphereForPlayer(const KCPrismData* data, const KCPrismHeader* header,
                              const KCFxyz* position, f32 radius, f32 thicknessScale,
                              f32* outDistance, u8* outLocation);

    u32 checkDisk(const KCFxyz* position, f32 radius, f32 halfHeight, const sead::Vector3f& normal,
                  f32 thicknessScale, u32 maxHitNum, HitInfoBuffer* outHitInfos);
    bool KCHitDisk(const KCPrismData* data, const KCPrismHeader* header, const KCFxyz* position,
                   f32 radius, f32 thicknessScale, f32 halfHeight, const sead::Vector3f& normal,
                   f32* outDistance, u8* outLocation);

    void searchPrism(KCFxyz* position, f32 radius, PrismCallback& callback);
    void searchPrismMinMax(const sead::Vector3f& min, const sead::Vector3f& max,
                           PrismCallback& callback);
    void searchPrismArrow(const sead::Vector3f& position, const sead::Vector3f& direction,
                          PrismCallback& callback);
    void searchPrismDisk(const sead::Vector3f& position, const sead::Vector3f& normal,
                         f32 halfHeight, f32 radius, PrismCallback& callback);

    bool isParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const;

    const KCFxyz* getFaceNormal(const KCPrismData* data, const KCPrismHeader* header) const;
    const KCFxyz* getEdgeNormal1(const KCPrismData* data, const KCPrismHeader* header) const;
    const KCFxyz* getEdgeNormal2(const KCPrismData* data, const KCPrismHeader* header) const;
    const KCFxyz* getEdgeNormal3(const KCPrismData* data, const KCPrismHeader* header) const;

    bool KCHitDisc(const KCPrismData* data, const KCPrismHeader* header,
                   const sead::Vector3f& position, const sead::Vector3f& normal, f32 radius,
                   f32 angle, sead::Vector3f* outFixDir, f32* outDistance);

    s32 toIndex(const KCPrismData* data, const KCPrismHeader* header) const;
    const KCFxyz* getNormal(u32 index, const KCPrismHeader* header) const;

    static void calXvec(const KCFxyz* a, const KCFxyz* b, KCFxyz* out);

    const KCFxyz* getVertexData(u32 index, const KCPrismHeader* header) const;
    s32 getVertexNum(const KCPrismHeader* header) const;
    s32 getNormalNum(const KCPrismHeader* header) const;
    s32 getAttributeElementNum() const;

    bool getAttributes(ByamlIter* destIter, u32 prismIndex, const KCPrismHeader* header) const;
    bool getAttributes(ByamlIter* destIter, const KCPrismData* data) const;

    void objectSpaceToAreaOffsetSpace(sead::Vector3u* outAreaSpace,
                                      const sead::Vector3f& objectSpace,
                                      const KCPrismHeader* header) const;
    void areaOffsetSpaceToObjectSpace(sead::Vector3f* outObjectSpace,
                                      const sead::Vector3u& areaSpace,
                                      const KCPrismHeader* header) const;

    bool doBoxCheck(const sead::Vector3f* position, const sead::Vector3f* size,
                    sead::Vector3u* outBlockMin, sead::Vector3u* outBlockMax,
                    const KCPrismHeader* header);

    u32 calcAreaBlockOffset(const sead::Vector3u& block, const KCPrismHeader* header) const;
    static u32 calcChildBlockOffset(const sead::Vector3u& block, s32 shift);
    static u32 getBlockData(const u32* data, u32 offset);

    f32 getFarthestVertexDistance() const { return mFarthestVertexDistance; }

private:
    sead::PtrArray<KCPrismHeader> mModelHeaders;

    KCFileHeader* mData = nullptr;
    ByamlIter* mAttributeIter = nullptr;
    const u32* mModelOffsets = nullptr;
    const u8* mOctreeData = nullptr;

    sead::Vector3i mAreaWidthShift = {0, 0, 0};
    sead::Vector3i mAreaWidthMask = {0, 0, 0};

    f32 mFarthestVertexDistance = 1.0f;
};

}  // namespace al
