#pragma once

#include <container/seadPtrArray.h>
#include <container/seadRingBuffer.h>
#include <math/seadQuat.h>
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
    sead::Vector3u mWidthMask;
    sead::Vector3u mWidthShift;
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

struct KCHitInfo {
    const KCPrismHeader* mHeader;
    const KCPrismData* mData;
    f32 _16;
    u8 _20;  // collision location, enum
};

class KCollisionServer {
public:
    KCollisionServer();

    void initKCollisionServer(void* data, const void* attributeData);
    void setData(void* data);
    const KCPrismHeader& getInnerKcl(s32 index) const;
    u32 getNumInnerKcl() const;
    const KCPrismHeader* getV1Header(s32 index) const;
    bool calcFarthestVertexDistance();
    s32 getTriangleNum(const KCPrismHeader* header) const;
    const KCPrismData& getPrismData(u32 index, const KCPrismHeader* header) const;
    bool isNearParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const;
    bool isNanPrism(const KCPrismData* data, const KCPrismHeader* header) const;
    void calcPosLocal(sead::Vector3f* pos, const KCPrismData* data, s32 location,
                      const KCPrismHeader* header) const;
    void getMinMax(sead::Vector3f* min, sead::Vector3f* max) const;
    void getAreaSpaceSize(sead::Vector3f* size, const KCPrismHeader* header) const;
    void getAreaSpaceSize(s32* sizeX, s32* sizeY, s32* sizeZ, const KCPrismHeader* header) const;
    void getAreaSpaceSize(sead::Vector3u* size, const KCPrismHeader* header) const;
    const KCPrismData* checkPoint(sead::Vector3f*, f32, f32*);  // TODO unknown parameter usage
    const u8* searchBlock(s32* widthShift, const sead::Vector3u& block,
                          const KCPrismHeader* header) const;
    s32 checkSphere(const sead::Vector3f*, f32, f32, u32,
                    sead::FixedRingBuffer<KCHitInfo, 512>* hits);  // TODO unknown parameter usage
    bool outCheckAndCalcArea(sead::Vector3u* blockMin, sead::Vector3u* blockMax,
                             const sead::Vector3f& posMin, const sead::Vector3f& posMax,
                             const KCPrismHeader* header) const;
    bool KCHitSphere(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f*,
                     f32, f32, f32*, u8*);  // TODO unknown parameter usage
    const KCPrismData* checkArrow(const sead::Vector3f&, const sead::Vector3f&,
                                  sead::FixedRingBuffer<KCHitInfo, 512>* hits, u32*,
                                  u32) const;  // TODO unknown parameter usage
    void objectSpaceToAreaOffsetSpaceV3f(sead::Vector3f* areaOff, const sead::Vector3f& objSpace,
                                         const KCPrismHeader* header) const;
    bool isInsideMinMaxInAreaOffsetSpace(const sead::Vector3u& block,
                                         const KCPrismHeader* header) const;
    bool KCHitArrow(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f&,
                    const sead::Vector3f&, f32*, u8*) const;  // TODO unknown parameter usage
    s32
    checkSphereForPlayer(const sead::Vector3f*, f32, f32, u32,
                         sead::FixedRingBuffer<KCHitInfo, 512>*);  // TODO unknown parameter usage
    bool KCHitSphereForPlayer(const KCPrismData* data, const KCPrismHeader* header,
                              const sead::Vector3f*, f32, f32, f32*,
                              u8*);  // TODO unknown parameter usage
    s32 checkDisk(const sead::Vector3f*, f32, f32, const sead::Vector3f&, f32, u32,
                  sead::FixedRingBuffer<KCHitInfo, 512>*);  // TODO unknown parameter usage
    bool KCHitDisk(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f*, f32,
                   f32, f32, const sead::Vector3f&, f32*, u8*);  // TODO unknown parameter usage
    void searchPrism(sead::Vector3f* pos, f32 searchRadius,
                     sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& callback);
    void searchPrismMinMax(const sead::Vector3f& min, const sead::Vector3f& max,
                           sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& callback);
    void searchPrismArrow(const sead::Vector3f& pos1, const sead::Vector3f& pos2,
                          sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& callback);
    void searchPrismDisk(const sead::Vector3f&, const sead::Vector3f&, f32, f32,
                         sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>&
                             callback);  // TODO unknown parameter usage
    bool isParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const;
    const sead::Vector3f& getFaceNormal(const KCPrismData* data, const KCPrismHeader* header) const;
    const sead::Vector3f& getEdgeNormal1(const KCPrismData* data,
                                         const KCPrismHeader* header) const;
    const sead::Vector3f& getEdgeNormal2(const KCPrismData* data,
                                         const KCPrismHeader* header) const;
    const sead::Vector3f& getEdgeNormal3(const KCPrismData* data,
                                         const KCPrismHeader* header) const;
    bool KCHitDisc(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f&,
                   const sead::Vector3f&, f32, f32, sead::Vector3f*,
                   f32*);  // TODO unknown parameter usage
    s32 toIndex(const KCPrismData* data, const KCPrismHeader* header) const;
    const sead::Vector3f& getNormal(u32 index, const KCPrismHeader* header) const;

    static void calXvec(const sead::Vector3f* a, const sead::Vector3f* b, sead::Vector3f* result);

    const sead::Vector3f& getVertexData(u32 index, const KCPrismHeader* header) const;
    u32 getVertexNum(const KCPrismHeader* header) const;
    s32 getNormalNum(const KCPrismHeader* header) const;
    s32 getAttributeElementNum() const;
    bool getAttributes(ByamlIter* destIter, u32 triIndex, const KCPrismHeader* header) const;
    bool getAttributes(ByamlIter* destIter, const KCPrismData* data) const;
    void objectSpaceToAreaOffsetSpace(sead::Vector3u* areaOffSpace, const sead::Vector3f& objSpace,
                                      const KCPrismHeader* header) const;
    void areaOffsetSpaceToObjectSpace(sead::Vector3f* objSpace, const sead::Vector3u& areaOffSpace,
                                      const KCPrismHeader* header) const;
    bool doBoxCheck(const sead::Vector3f*, const sead::Vector3f*, sead::Vector3u*, sead::Vector3u*,
                    const KCPrismHeader* header);  // TODO unknown parameter usage
    s32 calcAreaBlockOffset(const sead::Vector3u& block, const KCPrismHeader* header) const;

    static s32 calcChildBlockOffset(const sead::Vector3u& block, s32 shift);
    static u32 getBlockData(const u32* data, u32 offset);

private:
    sead::PtrArray<KCPrismHeader> mModelsData;
    void* mData;
    ByamlIter* mAttributeIter;
    void* mModelListData;
    void* mOctreeData;
    s32 mAreaWidthShift[3];
    s32 mAreaWidthMask[3];
    f32 mFarthestVertexDistance;
};

class SphereInterpolator {
public:
    SphereInterpolator() {}

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 sizeStart,
                     f32 sizeEnd, f32 steps);
    void nextStep();
    void calcInterpPos(sead::Vector3f* pos) const;
    void calcInterp(sead::Vector3f* pos, f32* size, sead::Vector3f* remainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* remainMoveVec) const;
    void getMoveVector(sead::Vector3f* moveVec);
    void calcStepMoveVector(sead::Vector3f* moveVec) const;

private:
    sead::Vector3f mPos;
    sead::Vector3f mMove;
    f32 mSizeStart;
    f32 mSizeEnd;
    f32 mStepSize;
    f32 mCurrentStep;
    f32 mPrevStep;
};

class SpherePoseInterpolator {
public:
    SpherePoseInterpolator() {}

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 sizeStart,
                     f32 sizeEnd, const sead::Quatf& quatStart, const sead::Quatf& quatEnd,
                     f32 steps);
    void nextStep();
    void calcInterpPos(sead::Vector3f* pos) const;
    void calcInterp(sead::Vector3f* pos, f32* size, sead::Quatf* quat,
                    sead::Vector3f* remainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* remainMoveVec) const;
    f32 calcRadiusBaseScale(f32 unk) const;
    void getMoveVector(sead::Vector3f* moveVec);

private:
    sead::Vector3f mPos;
    sead::Vector3f mMove;
    f32 mSizeStart;
    f32 mSizeEnd;
    sead::Quatf mQuatStart;
    sead::Quatf mQuatEnd;
    f32 mStepSize;
    f32 mCurrentStep;
    f32 mPrevStep;
};

}  // namespace al
