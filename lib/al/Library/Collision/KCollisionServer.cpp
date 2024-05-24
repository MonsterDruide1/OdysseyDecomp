#include "Library/Collision/KCollisionServer.h"

#include "Library/Math/MathUtil.h"

#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/SegmentUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "math/seadVectorFwd.h"
#include <math.h>
#include "Library/Math/MathUtil.h"

#include <cstdio>

namespace al {

void SphereInterpolator::startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd,
                                     f32 sizeStart, f32 sizeEnd, f32 steps) {
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;
    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}
void SphereInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SphereInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SphereInterpolator::calcInterp(sead::Vector3f* pos, f32* size,
                                    sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    calcRemainMoveVector(remainMoveVec);
}
void SphereInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
void SphereInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}
void SphereInterpolator::calcStepMoveVector(sead::Vector3f* moveVec) const {
    f32 step = mCurrentStep - mPrevStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

void SpherePoseInterpolator::startInterp(const sead::Vector3f& posStart,
                                         const sead::Vector3f& posEnd, f32 sizeStart, f32 sizeEnd,
                                         const sead::Quatf& quatStart, const sead::Quatf& quatEnd,
                                         f32 steps) {
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;

    mQuatStart.x = quatStart.x;
    mQuatStart.y = quatStart.y;
    mQuatStart.z = quatStart.z;
    mQuatStart.w = quatStart.w;

    mQuatEnd.x = quatEnd.x;
    mQuatEnd.y = quatEnd.y;
    mQuatEnd.z = quatEnd.z;
    mQuatEnd.w = quatEnd.w;

    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}

void SpherePoseInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SpherePoseInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SpherePoseInterpolator::calcInterp(sead::Vector3f* pos, f32* size, sead::Quatf* quat,
                                        sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    slerpQuat(quat, mQuatStart, mQuatEnd, mCurrentStep);
    quat->normalize();
    calcRemainMoveVector(remainMoveVec);
}
void SpherePoseInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
f32 SpherePoseInterpolator::calcRadiusBaseScale(f32 unk) const {
    return calcRate01(unk, 0.0f, mSizeEnd);
}
void SpherePoseInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

KCollisionServer::KCollisionServer() {}

void KCollisionServer::initKCollisionServer(void* data, const void* iterData) {
    setData(data);
    if(iterData != nullptr) {
        mIter = new ByamlIter((u8*)iterData);
    }
}

void KCollisionServer::setData(void* data) {
    mData = data;
    mModelListData = reinterpret_cast<void*>(uintptr_t(data) + getBlockData((u32*)data, 0x8));
    mOctreeData = reinterpret_cast<void*>(uintptr_t(data) + getBlockData((u32*)data, 0x4));

    mCoordShift.x = getBlockData((u32*)data, 0x28)-1;
    mCoordShift.y = getBlockData((u32*)data, 0x2C)-1;
    mCoordShift.z = getBlockData((u32*)data, 0x30)-1;
    unkShiftedByCoordShift.x = -1 << getBlockData((u32*)data, 0x28);
    unkShiftedByCoordShift.y = -1 << getBlockData((u32*)data, 0x2C);
    unkShiftedByCoordShift.z = -1 << getBlockData((u32*)data, 0x30);

    mModelsData.allocBuffer(getBlockData((u32*)data, 0xC), nullptr);
    for(int i=0; i<(s32)getBlockData((u32*)mData, 0xC); i++) {
        mModelsData.pushBack(reinterpret_cast<KCPrismHeader*>(reinterpret_cast<u8*>(mData) + *(reinterpret_cast<u32*>(mModelListData) + i)));
    }
}

const KCPrismHeader& KCollisionServer::getInnerKcl(s32 index) const {
    return *reinterpret_cast<const KCPrismHeader*>(reinterpret_cast<const u8*>(mData)+*(reinterpret_cast<const u32*>(mModelListData) + index));
}

u32 KCollisionServer::getNumInnerKcl() const {
    return getBlockData(reinterpret_cast<const u32*>(mData), 0xC);
}

const KCPrismHeader* KCollisionServer::getV1Header(s32 index) const {
    return mModelsData[index];
}

bool KCollisionServer::calcFarthestVertexDistance() {
    if((s32)getNumInnerKcl() < 1) {
        mFarthestVertexDistance = 0.0f;
        return true;
    }

    f32 maxSquaredLength = 0.0f;
    bool isFirstModel = true;
    for(int i=0; i<(s32)getNumInnerKcl(); i++) {
        const KCPrismHeader* header = getV1Header(i);
        s32 numTriangles = getTriangleNum(header);
        for(int j=0; j<numTriangles; j++) {
            const KCPrismData& data = getPrismData(j, header);
            if(isNearParallelNormal(&data, header)) {
                const_cast<KCPrismData&>(data).mLength = -sead::Mathf::abs(data.mLength);
                break;
            }
            if(isNanPrism(&data, header)) {
                f32 v16 = sead::Mathf::abs(data.mLength);
                if(!isnan(data.mLength)) {
                    const_cast<KCPrismData&>(data).mLength = 0.0f;
                } else {
                    const_cast<KCPrismData&>(data).mLength = -v16;
                }
                break;
            }
            
            sead::Vector3f pos;
            calcPosLocal(&pos, &data, 0, header);
            if(pos.squaredLength() > maxSquaredLength)
                maxSquaredLength = pos.squaredLength();
            calcPosLocal(&pos, &data, 1, header);
            if(pos.squaredLength() > maxSquaredLength)
                maxSquaredLength = pos.squaredLength();
            calcPosLocal(&pos, &data, 2, header);
            if(pos.squaredLength() > maxSquaredLength)
                maxSquaredLength = pos.squaredLength();
        }
        isFirstModel = false;
    }
    mFarthestVertexDistance = sead::Mathf::sqrt(maxSquaredLength);
    return isFirstModel;
}

s32 KCollisionServer::getTriangleNum(const KCPrismHeader* header) const {
    return (header->mOctreeOffset - (u64)header->mTrianglesOffset) / sizeof(KCPrismData);
}

const KCPrismData& KCollisionServer::getPrismData(u32 index, const KCPrismHeader* header) const {
    return reinterpret_cast<const KCPrismData*>(uintptr_t(header) + header->mTrianglesOffset)[(int)index];
}

// NON_MATCHING: Major mismatch due to order of loads / first comparisons
bool KCollisionServer::isNearParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const {
    const sead::Vector3f& edge1 = getEdgeNormal1(data, header);
    const sead::Vector3f& edge2 = getEdgeNormal2(data, header);
    const sead::Vector3f& edge3 = getEdgeNormal3(data, header);
    return isParallelDirection(edge1, edge2, 0.01f)
        || isParallelDirection(edge1, edge3, 0.01f)
        || isParallelDirection(edge2, edge3, 0.01f);
}

// NON_MATCHING: Major differences, especially using ccmp vs. b.hi/and/orr/cmp
bool KCollisionServer::isNanPrism(const KCPrismData* data, const KCPrismHeader* header) const {
    if(isnan(data->mLength)) return true;

  sead::Vector3f normalFace = getFaceNormal(data, header);
  sead::Vector3f normalA = getEdgeNormal1(data, header);
  sead::Vector3f normalB = getEdgeNormal2(data, header);
  sead::Vector3f normalC = getEdgeNormal3(data, header);
  sead::Vector3f pos = getVertexData(data->mPosIndex, header);

  sead::Vector3f firstResult;
  calcPosLocal(&firstResult, data, 1, header);

  bool anyNan = isnan(firstResult.x) || isnan(firstResult.y) || isnan(firstResult.z)
        || isnan(normalFace.x) || isnan(normalFace.y) || isnan(normalFace.z)
        || isnan(normalA.x) || isnan(normalA.y) || isnan(normalA.z)
        || isnan(normalB.x) || isnan(normalB.y) || isnan(normalB.z)
        || isnan(normalC.x) || isnan(normalC.y) || isnan(normalC.z)
        || isnan(pos.x) || isnan(pos.y) || isnan(pos.z);
  if (anyNan)
    return true;

  sead::Vector3f secondResult;
  calcPosLocal(&secondResult, data, 2, header);
  return isnan(secondResult.x) || isnan(secondResult.y) || isnan(secondResult.z);
}

// NON_MATCHING: various, major stuff
void KCollisionServer::calcPosLocal(sead::Vector3f* pos, const KCPrismData* data, s32 vIndex, const KCPrismHeader* header) const {
    if(vIndex == 0) {
        sead::Vector3f vertex = getVertexData(data->mPosIndex, header);
        *pos = vertex;
        return;
    } else if(vIndex == 1) {
      /*/
        sead::Vector3f normalB = getEdgeNormal2(data, header);
        sead::Vector3f normalFace = getFaceNormal(data, header);
        const sead::Vector3f& normalC = getEdgeNormal3(data, header);
      /*/
        const sead::Vector3f* normals = reinterpret_cast<const sead::Vector3f*>(uintptr_t(header) + header->mNormalsOffset);
        sead::Vector3f normalB = normals[data->mEdgeNormalIndex[1]];
        sead::Vector3f normalFace = normals[data->mFaceNormalIndex];
        const sead::Vector3f& normalC = normals[data->mEdgeNormalIndex[2]];
      /**/
        const sead::Vector3f& vertex = getVertexData(data->mPosIndex, header);
        sead::Vector3f cross;
        calXvec(&normalB, &normalFace, &cross);
        f32 factor = data->mLength / cross.dot(normalC);//fmaxf(cross.dot(normalC), 0.00000011921f);

        pos->x = vertex.x + cross.x * factor;
        pos->y = vertex.y + cross.y * factor;
        pos->z = vertex.z + cross.z * factor;
    } else if(vIndex == 2) {
      /*/
        sead::Vector3f normalA = getEdgeNormal1(data, header);
        sead::Vector3f normalFace = getFaceNormal(data, header);
        const sead::Vector3f& normalC = getEdgeNormal3(data, header);
      /*/
        const sead::Vector3f* normals = reinterpret_cast<const sead::Vector3f*>(uintptr_t(header) + header->mNormalsOffset);
        sead::Vector3f normalA = normals[data->mEdgeNormalIndex[0]];
        sead::Vector3f normalFace = normals[data->mFaceNormalIndex];
        const sead::Vector3f& normalC = normals[data->mEdgeNormalIndex[2]];
      /**/

        const sead::Vector3f& vertex = getVertexData(data->mPosIndex, header);
        sead::Vector3f cross;
        calXvec(&normalA, &normalFace, &cross);
        f32 factor = data->mLength / cross.dot(normalC);//fmaxf(cross.dot(normalC), 0.00000011921f);
        
        pos->x = vertex.x + cross.x * factor;
        pos->y = vertex.y + cross.y * factor;
        pos->z = vertex.z + cross.z * factor;
    } else {
        *pos = {0.0f, 0.0f, 0.0f};
    }
}

// NON_MATCHING: Using 64-bit registers instead of two 32-bit registers
void KCollisionServer::getMinMax(sead::Vector3f* min, sead::Vector3f* max) const {
    sead::Vector3f minVec = *reinterpret_cast<sead::Vector3f*>((char*)mData + 0x10);
    *min = minVec;
    sead::Vector3f maxVec = *reinterpret_cast<sead::Vector3f*>((char*)mData + 0x1C);
    *max = maxVec;
}

void KCollisionServer::getAreaSpaceSize(sead::Vector3f* vec, const KCPrismHeader* header) const {
    float x = (s32) ~(header->mMask.x);
    u32 headerMaskZ = header->mMask.z;
    float y = (s32) ~(header->mMask.y);
    float z = (s32) ~(headerMaskZ);
    *vec = {x, y, z};
}

void KCollisionServer::getAreaSpaceSize(s32* x, s32* y, s32* z, const KCPrismHeader* header) const {
    *x = ~(header->mMask.x);
    *y = ~(header->mMask.y);
    *z = ~(header->mMask.z);
}

void KCollisionServer::getAreaSpaceSize(sead::Vector3u* vec, const KCPrismHeader* header) const {
    vec->x = ~(header->mMask.x);
    vec->y = ~(header->mMask.y);
    vec->z = ~(header->mMask.z);
}

// NON_MATCHING: various, major stuff
const KCPrismData* KCollisionServer::checkPoint(sead::Vector3f* point, f32 a3, f32* a4) {
    int v41 = 2;
    const KCPrismData* v4;
    for(int i=0; i<(s32)getNumInnerKcl(); i++) {
        const KCPrismHeader* header = getV1Header(i);
        sead::Vector3u areaOffsetSpace;
        objectSpaceToAreaOffsetSpace(&areaOffsetSpace, *point, header);
        if(!isInsideMinMaxInAreaOffsetSpace(areaOffsetSpace, header)) {
            v41 = 4;
        } else {
            s32 v6;
            const u8* data = searchBlock(&v6, areaOffsetSpace, header);
            u16* data3 = (u16*)(data+2);
            u16 data2;
            while((data2 = *data3++) != 0xFFFF) {
                if(getPrismData(data2, header).mLength <= 0.0) continue;

                const KCPrismData& prismData = getPrismData(data2, header);
                const sead::Vector3f& prismPos = getVertexData(prismData.mPosIndex, header);
                sead::Vector3f something = *point - prismPos;
                if(
                    something.dot(getEdgeNormal1(&prismData, header)) <= 0.0f &&
                    something.dot(getEdgeNormal2(&prismData, header)) <= 0.0f &&
                    something.dot(getEdgeNormal3(&prismData, header)) <= prismData.mLength
                ) {
                    f32 dp = -something.dot(getFaceNormal(&prismData, header));
                    if(dp >= 0.0 && header->mThickness*a3 >= dp) {
                        v41 = 1;
                        v4 = &prismData;
                        *a4 = dp;
                        return v4;
                    }
                }
            }
            v41 = 0;
        }
    }
    if(v41 == 2) return nullptr;
    return v4;
}

// NON_MATCHING: See calcAreaBlockOffset
const u8* KCollisionServer::searchBlock(s32* coordShift, const sead::Vector3u& pos, const KCPrismHeader* header) const {
    s32 v5 = calcAreaBlockOffset(pos, header);
    const u8* octreeData = reinterpret_cast<const u8*>(header) + header->mOctreeOffset;
    *coordShift = header->mCoordShift.x;
    if(header->mCoordShift.z == -1 && header->mCoordShift.y == -1)
        v5 = 0;

    u32 v8 = getBlockData((u32*) octreeData, v5);
    while((v8 & 0x80000000) == 0) {
        (*coordShift)--;
        octreeData += v8;
        u32 v10 = calcChildBlockOffset(pos, *coordShift);
        v8 = getBlockData((u32*)octreeData, v10);
    }

    return &octreeData[v8 & 0x7FFFFFFF];
}

s32 KCollisionServer::checkSphere(const sead::Vector3f*, f32, f32, u32, sead::FixedRingBuffer<KCHitInfo, 512>*) {}

// NON_MATCHING: different comparison for posOut1->x and posOut2->x
bool KCollisionServer::outCheckAndCalcArea(sead::Vector3u* posOut1, sead::Vector3u* posOut2, const sead::Vector3f& pos1,
                            const sead::Vector3f& pos2, const KCPrismHeader* header) const {
    objectSpaceToAreaOffsetSpace(posOut1, pos1, header);
    objectSpaceToAreaOffsetSpace(posOut2, pos2, header);

    if(posOut1->x & 0x80000000)
        posOut1->x = 0;
    if(posOut1->y & 0x80000000)
        posOut1->y = 0;
    if(posOut1->z & 0x80000000)
        posOut1->z = 0;

    s32 maskX = ~(header->mMask.x);
    if((s32)posOut2->x > maskX)
        posOut2->x = maskX;
    s32 maskY = ~(header->mMask.y);
    if((s32)posOut2->y > maskY)
        posOut2->y = maskY;
    s32 maskZ = ~(header->mMask.z);
    if((s32)posOut2->z > maskZ)
        posOut2->z = maskZ;

    return  (s32)posOut2->x >= (s32)posOut1->x && (s32)posOut2->y >= (s32)posOut1->y && (s32)posOut2->z >= (s32)posOut1->z;
}

bool KCollisionServer::KCHitSphere(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f*, f32, f32,
                    f32*, u8*) {}
const KCPrismData* KCollisionServer::checkArrow(const sead::Vector3f& unk1, const sead::Vector3f& unk2,
                sead::FixedRingBuffer<KCHitInfo, 512>* buffer, u32* unk3,u32 bufferCapacity) const {
  bool v10; // x0
  int *mData; // x11
  u64 v12; // x19
  al::KCPrismHeader *v13; // x28
  float v14; // s13
  u32 mXMask; // w12
  u32 mYMask; // w13
  float v17; // s12
  float v18; // s14
  unsigned int v19; // w8
  unsigned int v20; // w9
  unsigned int v21; // w10
  u32 *p_mZMask; // x25
  u32 mZMask; // w14
  float x; // s0
  float v25; // s1
  float v26; // s15
  float v28; // s3
  float y; // s1
  float v30; // s4
  float z; // s2
  float v32; // s5
  float v33; // s2
  float v35; // s3
  int *v37; // x27
  int v38; // w12
  int *v39; // x22
  int v40; // w12
  int *v41; // x24
  int v42; // w11
  unsigned int i; // w13
  int v49; // w12
  int v50; // w8
  int v51; // w9
  int v52; // w10
  int v53; // w8
  const u8 *v54; // x21
  float v55; // s10
  u64 v56; // x8
  unsigned int v57; // t1
  const al::KCPrismData *v58; // x1
  int mSize; // w9
  u64 v60; // x8
  int v61; // w11
  int v62; // w9
  int v63; // w8
  al::KCHitInfo *mBuffer; // x9
  al::KCHitInfo *v65; // x8
  const al::KCPrismData* v66; // x9
  int v67; // w8
  float v68; // s10
  const al::KCPrismData *v69; // x1
  u64 v70; // x8
  unsigned int v71; // t1
  float v72; // s10
  float v73; // s11
  float v74; // s0
  float v75; // s1
  const al::KCPrismData* v77; // [xsp+8h] [xbp-138h]
  const al::KCollisionServer *v79; // [xsp+18h] [xbp-128h]
  u32 v81; // [xsp+28h] [xbp-118h]
  int v82; // [xsp+2Ch] [xbp-114h]
  int v83; // [xsp+30h] [xbp-110h]
  int v84; // [xsp+34h] [xbp-10Ch]
  const sead::Vector3f *v85; // [xsp+48h] [xbp-F8h]
  const al::KCPrismData* v86; // [xsp+50h] [xbp-F0h]
  int v89; // [xsp+84h] [xbp-BCh] BYREF
  int v90; // [xsp+88h] [xbp-B8h] BYREF
  int v91; // [xsp+8Ch] [xbp-B4h] BYREF
  int v92; // [xsp+90h] [xbp-B0h] BYREF
  int v93; // [xsp+94h] [xbp-ACh] BYREF
  int v94; // [xsp+98h] [xbp-A8h] BYREF
  float a1; // [xsp+9Ch] [xbp-A4h] BYREF
  sead::Vector3f a2; // [xsp+A0h] [xbp-A0h] BYREF

  if ( unk2.x == 0.0 && unk2.y == 0.0 && unk2.z == 0.0 )
    return 0LL;

  a2 = unk2;
  al::separateScalarAndDirection(&a1, &a2, a2);
  v10 = al::isNearZero(a2, 0.001);
  if ( v10 )
    return 0LL;

  mData = (int *)this->mData;
  if ( mData[3] >= 1 )
  {
    v81 = 0;
    v85 = &unk2;
    v86 = 0LL;
    v12 = 0LL;
    v77 = nullptr; //1000000000.0f;   FIXME access to DWORDs, using this floating point?
    v79 = this;
    while ( 1 )
    {
      v13 = this->mModelsData[v12];
      mXMask = v13->mMask.x;
      mYMask = v13->mMask.y;
      v17 = unk1.x - v13->mOctreeOrigin.x;
      v14 = unk1.y - v13->mOctreeOrigin.y;
      v18 = unk1.z - v13->mOctreeOrigin.z;
      v19 = (int)v17;
      v20 = (int)v14;
      if ( !(mXMask & (int)v17 | mYMask & (int)v14) )
      {
        v21 = (int)v18;
        p_mZMask = &v13->mMask.z;
        if ( (v13->mMask.z & (int)v18) == 0 )
          break;
      }

      p_mZMask = &v13->mMask.z;
      mZMask = v13->mMask.z;
      x = a2.x;
      if ( a2.x != 0.0 )
      {
        v25 = (float)~mXMask;
        if ( a2.x > 0.0 )
          v25 = 0.0;

        v26 = (float)(v25 - v17) / a2.x;
        if ( v26 >= 0.0 && v26 <= a1 )
        {
          v28 = v17 + (float)(a2.x * v26);
          y = a2.y;
          v30 = v14 + (float)(v26 * a2.y);
          z = a2.z;
          v19 = (int)v28;
          v20 = (int)v30;
          v32 = v18 + (float)(v26 * a2.z);
          v21 = (int)v32;
          if ( !((int)v30 & mYMask | (int)v28 & mXMask | (int)v32 & mZMask) )
            goto LABEL_39;
        }
      }

      y = a2.y;
      if ( a2.y != 0.0 )
      {
        v33 = (float)~mYMask;
        if ( a2.y > 0.0 )
          v33 = 0.0;

        v26 = (float)(v33 - v14) / a2.y;
        if ( v26 >= 0.0 && v26 <= a1 )
        {
          v28 = v17 + (float)(v26 * a2.x);
          z = a2.z;
          v30 = v14 + (float)(a2.y * v26);
          v19 = (int)v28;
          v32 = v18 + (float)(v26 * a2.z);
          v20 = (int)v30;
          v21 = (int)v32;
          if ( !((int)v28 & mXMask | mYMask & (int)v30 | mZMask & (int)v32) )
          {
LABEL_39:
            v17 = v28;
            v14 = v30;
            v18 = v32;
            goto LABEL_41;
          }
        }
      }

      z = a2.z;
      if ( a2.z != 0.0 )
      {
        v35 = (float)~mZMask;
        if ( a2.z > 0.0 )
          v35 = 0.0;

        v26 = (float)(v35 - v18) / a2.z;
        if ( v26 >= 0.0 && v26 <= a1 )
        {
          v17 = v17 + (float)(v26 * a2.x);
          v14 = v14 + (float)(v26 * a2.y);
          v18 = v18 + (float)(a2.z * v26);
          v19 = (int)v17;
          v20 = (int)v14;
          v21 = (int)v18;
          if ( !((int)v17 & mXMask | mYMask & (int)v14 | mZMask & (int)v18) )
            goto LABEL_41;
        }
      }

LABEL_121:
      if ( ++v12 >= mData[3] )
        goto LABEL_124;
    }

    x = a2.x;
    y = a2.y;
    z = a2.z;
    v26 = 0.0;

LABEL_41:
    if ( x >= 0.0 )
      v37 = &v94;
    else
      v37 = &v91;

    if ( x < 0.0 )
      v38 = -1;
    else
      v38 = 1;

    v84 = v38;
    if ( y >= 0.0 )
      v39 = &v93;
    else
      v39 = &v90;

    if ( y < 0.0 )
      v40 = -1;
    else
      v40 = 1;

    v83 = v40;
    if ( z >= 0.0 )
      v41 = &v92;
    else
      v41 = &v89;

    if ( z < 0.0 )
      v42 = -1;
    else
      v42 = 1;

    v82 = v42;
    do
    {
      int mCoordShift;
      v54 = searchBlock(&mCoordShift, {v19, v20, v21}, v13);

      v49 = 1 << mCoordShift;
      v50 = (v49 - 1) & v19;
      v51 = (v49 - 1) & v20;
      v52 = (v49 - 1) & v21;
      v91 = -v50;
      v94 = v49 - v50;
      v90 = -v51;
      v89 = -v52;
      v53 = *v37;
      v92 = v49 - v52;
      v93 = v49 - v51;
      if ( !v53 )
        *v37 = v84;

      if ( !*v39 )
        *v39 = v83;


      
      if ( !*v41 )
        *v41 = v82;

      //*(_QWORD *)((char *)&v88 + 5) = 0LL;
      KCHitInfo hitInfo = {};
      if ( buffer )
      {
        v55 = 1.0;
        while ( 1 )
        {
          v57 = *((u16 *)v54 + 1);
          v54 += 2;
          v56 = v57;
          if ( v57 == 0xFFFFLL )
            break;

          v58 = (const al::KCPrismData *)((char *)v13 + 20 * v56 + v13->mTrianglesOffset);
          hitInfo.mData = v58;
          if ( v58->mLength > 0.0 )
          {
            v10 = KCHitArrow(v58,
                    v13,
                    unk1,
                    *v85,
                    &hitInfo.something,
                    &hitInfo.mCollisionLocation);
            if ( v10 )
            {
              buffer->pushBack(hitInfo);

              v66 = v86;
              if ( hitInfo.something < v55 )
                v66 = hitInfo.mData;

              if ( hitInfo.something < v55 )
                v55 = hitInfo.something;

              v86 = v66;
              if ( v81 + 1 == bufferCapacity )
              {
                if ( unk3 )
                  *unk3 = bufferCapacity;

                v67 = 1;
                v81 = bufferCapacity;
                v77 = v66;
              }
              else
              {
                ++v81;
                v67 = 0;
              }
            }
            else
            {
              v67 = 8;
            }

            if ( (v67 & 7 | 8) != 8 )
            {
              if ( !v67 )
                break;

              return v77;
            }
          }
        }
      }
      else
      {
        v68 = 1.0;
        while ( 1 )
        {
          v71 = *((u16 *)v54 + 1);
          v54 += 2;
          v70 = v71;
          if ( v71 == 0xFFFFLL )
            break;

          v69 = (al::KCPrismData *)((char *)v13 + 20 * v70 + v13->mTrianglesOffset);
          hitInfo.mData = v69;
          if ( v69->mLength > 0.0 )
          {
            v10 = al::KCollisionServer::KCHitArrow(
                    v69,
                    v13,
                    unk1,
                    *v85,
                    &hitInfo.something,
                    &hitInfo.mCollisionLocation);
            if ( v10 && hitInfo.something < v68 )
            {
              v68 = hitInfo.something;
              v86 = hitInfo.mData;
            }
          }
        }
      }

      if ( !buffer && v86 )
        break;

      v72 = 1000000000.0;
      if ( !al::isNearZero(a2, 0.001) )
        v72 = (float)*v37 / a2.x;

      v73 = 1000000000.0;
      if ( !al::isNearZero(a2, 0.001) )
        v73 = (float)*v39 / a2.y;

      v10 = al::isNearZero(a2, 0.001);
      v74 = 1000000000.0;
      if ( !v10 )
        v74 = (float)*v41 / a2.z;

      if ( v73 >= v72 )
        v75 = v72;
      else
        v75 = v73;

      if ( v74 < v75 )
        v75 = v74;

      if ( (float)(a1 - v26) <= v75 )
        break;

      v17 = v17 + (float)(v75 * a2.x);
      v19 = (int)v17;
      if ( (v13->mMask.x & (int)v17) != 0 )
        break;

      v14 = v14 + (float)(v75 * a2.y);
      v20 = (int)v14;
      if ( (v13->mMask.y & (int)v14) != 0 )
        break;

      v18 = v18 + (float)(v75 * a2.z);
      v21 = (int)v18;
      v26 = v26 + v75;
    }
    while ( (v13->mMask.z & (int)v18) == 0 && v26 < a1 );

    mData = (int *)v79->mData;
    goto LABEL_121;
  }

  v86 = 0LL;
  v81 = 0;

LABEL_124:
  if ( unk3 )
    *unk3 = v81;

  return v86;
}

void KCollisionServer::objectSpaceToAreaOffsetSpaceV3f(sead::Vector3f* areaOffSpace, const sead::Vector3f& objSpace,
                                        const KCPrismHeader* header) const {
                                            *areaOffSpace = objSpace - header->mOctreeOrigin;
                                        }

bool KCollisionServer::isInsideMinMaxInAreaOffsetSpace(const sead::Vector3u& pos, const KCPrismHeader* header) const {
    return !(pos.x & header->mMask.x) && !(pos.y & header->mMask.y) && !(pos.z & header->mMask.z);
}

bool KCollisionServer::KCHitArrow(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f& start,
                const sead::Vector3f& end, f32* val, u8* type) const {
  printf("KCHitArrow(%p, %p, (%.02f, %.02f, %.02f), (%.02f, %.02f, %.02f), %p, %p)\n", data, header, start.x, start.y, start.z, end.x, end.y, end.z, val, type);
  float x; // s1
  float v10; // s2
  float z; // s3
  const sead::Vector3f *v12; // x9
  float v13; // s1
  float y; // s7
  float v15; // s4
  float v16; // s16
  float v17; // s0
  float v18; // s5
  float v19; // s6
  float v20; // s7
  float v21; // s0
  const sead::Vector3f *v22; // x10
  float v23; // s1
  float v24; // s3
  float v25; // s2
  float v26; // s5
  const sead::Vector3f *v27; // x11
  bool v28; // w9
  bool v29; // w10
  float v30; // s5
  const sead::Vector3f *v31; // x12
  float v32; // s1
  bool result; // x0
  u8 v34; // w8

  const sead::Vector3f* position = &getVertexData(data->mPosIndex, header);
  x = position->x;
  v10 = start.y - position->y;
  z = position->z;
  v12 = &getFaceNormal(data, header);
  v13 = start.x - x;
  y = v12->y;
  v15 = start.z - z;
  v16 = v12->z;
  v17 = (float)((float)(v13 * v12->x) + (float)(v10 * y)) + (float)(v15 * v16);
  if ( v17 <= 0.0 )
    goto LABEL_6;

  v18 = end.y;
  v19 = end.z;
  v20 = (float)((float)(v12->x * end.x) + (float)(y * v18)) + (float)(v16 * v19);
  if ( (float)(v17 + v20) > 0.0 )
    goto LABEL_6;

  v21 = v17 / (float)-v20;
  v22 = &getEdgeNormal1(data, header);
  v23 = v13 + (float)(end.x * v21);
  v24 = v10 + (float)(v18 * v21);
  v25 = v15 + (float)(v19 * v21);
  v26 = (float)((float)(v23 * v22->x) + (float)(v24 * v22->y)) + (float)(v25 * v22->z);
  if ( v26 > 0.01
    || (v27 = &getEdgeNormal2(data, header),
        v28 = v26 >= 0.0,
        v29 = v26 <= 0.01,
        v30 = (float)((float)(v23 * v27->x) + (float)(v24 * v27->y)) + (float)(v25 * v27->z),
        v30 > 0.01)
    || (v31 = &getEdgeNormal3(data, header),
        v32 = (float)((float)(v23 * v31->x) + (float)(v24 * v31->y)) + (float)(v25 * v31->z),
        (float)(data->mLength + 0.01) < v32) )
  {
LABEL_6:
    result = 0LL;
    *type = 0;
    return result;
  }

  *val = v21;
  if ( !v28 || !v29 )
  {
    if ( v30 >= 0.0 && v30 <= 0.01 )
    {
      if ( v32 >= 0.0 && v32 <= 0.01 )
        v34 = 6;
      else
        v34 = 3;

      goto LABEL_21;
    }

    if ( v32 >= 0.0 && v32 <= 0.01 )
    {
      v34 = 4;
      goto LABEL_21;
    }

LABEL_18:
    result = 1LL;
    *type = 1;
    return result;
  }

  if ( v30 < 0.0 || v30 > 0.01 )
  {
    if ( v32 >= 0.0 && v32 <= 0.01 )
      v34 = 7;
    else
      v34 = 2;

    goto LABEL_21;
  }

  if ( v32 >= 0.0 && v32 <= 0.01 )
    goto LABEL_18;

  v34 = 5;

LABEL_21:
  result = 1LL;
  *type = v34;
  return result;
}

s32 KCollisionServer::checkSphereForPlayer(const sead::Vector3f*, f32, f32, u32,
                            sead::FixedRingBuffer<KCHitInfo, 512>*) {}
bool KCollisionServer::KCHitSphereForPlayer(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f* position, f32 a5,
                            f32 a6, f32* a7, u8* a8) {
  printf("KCHitSphereForPlayer(%p, %p, (%.02f, %.02f, %0.2f), %f, %f)\n", data, header, position->x, position->y, position->z, a5, a6);
  float mThickness; // s4
  float fxe1; // s0
  float fxe2; // s2
  float fxe3; // s3
  float fxf; // s10
  float v30; // s11
  float v31; // s1
  float e2xe1; // s16
  float e2xe3; // s16
  float e1xe3; // s16
  float v51; // s2
  float v52; // s3
  float v62; // s0
  sead::Vector3f v53v;

  mThickness = header->mThickness;
  *a8 = 0;
  const sead::Vector3f &pos = getVertexData(data->mPosIndex, header);
  sead::Vector3f finalPos = *position - pos;
  
  const sead::Vector3f& edge1 = getEdgeNormal1(data, header);
  fxe1 = finalPos.dot(edge1);
  if ( fxe1 >= a5 )
    return false;

  const sead::Vector3f& edge2 = getEdgeNormal2(data, header);
  fxe2 = finalPos.dot(edge2);
  if ( fxe2 >= a5 )
    return false;

  const sead::Vector3f& edge3 = getEdgeNormal3(data, header);
  fxe3 = finalPos.dot(edge3) - data->mLength;
  if ( fxe3 >= a5 )
    return false;

  const sead::Vector3f& face = getFaceNormal(data, header);
  fxf = finalPos.dot(face);
  *a7 = a5 - fxf;
  if ( (a5 - fxf) < 0.0f )
    return false;

  v30 = a5 * a5;
  v31 = mThickness * a6;
  if ( fxe1 <= fxe2 && fxe2 > fxe3 ) {
    if ( fxe2 <= 0.0 ) {
      if ( v31 >= (a5 - fxf) ) {
        *a8 = 1;
        return true;
      }
      return false;
    }
    
    if ( fxe3 <= fxe1 ) {
      e2xe1 = edge2.dot(edge1);
      if ( (fxe2 * e2xe1) <= fxe1 ) {
LABEL_34:
        f32 v59 = ((fxe2 * e2xe1) - fxe1) / ((e2xe1 * e2xe1) - 1.0f);
        f32 v60 = fxe2 - (e2xe1 * v59);
        v53v = (edge1 * v59) + (edge2 * v60);
        *a8 = 5;
        goto LABEL_35;
      }
    }
    else {
      e2xe3 = edge2.dot(edge3);
      if ( (fxe2 * e2xe3) <= fxe3 ) {
        goto LABEL_25;
      }
    }

    if ( fxe2 > fxf ) {
      return false;
    }

    *a7 = v30 - (fxe2 * fxe2);
    *a8 = 3;
  }
  else if ( fxe1 > fxe3 ) {
    if ( fxe1 <= 0.0 ) {
      if ( v31 >= (a5 - fxf) ) {
        *a8 = 1;
        return true;
      }
      return false;
    }
    
    if ( fxe2 > fxe3 ) {
      e2xe1 = edge1.dot(edge2);
      if ( (fxe1 * e2xe1) <= fxe2 )
        goto LABEL_34;
    }
    else {
      e1xe3 = edge1.dot(edge3);
      if ( (fxe1 * e1xe3) <= fxe3 )
        goto LABEL_29;
    }

    if ( fxe1 > fxf ) {
      return false;
    }

    *a7 = v30 - (fxe1 * fxe1);
    *a8 = 2;
  }
  else {
    if ( fxe3 <= 0.0 ) {
      if ( v31 >= (a5 - fxf) ) {
        *a8 = 1;
        return true;
      }
      return false;
    }

    if ( fxe1 <= fxe2 ) {
      e2xe3 = edge3.dot(edge2);
      if ( (fxe3 * e2xe3) <= fxe2 ) {
LABEL_25:
        v51 = ((fxe3 * e2xe3) - fxe2) / ((e2xe3 * e2xe3) - 1.0f);
        v52 = fxe3 - (e2xe3 * v51);
        v53v = (edge2 * v51) + (edge3 * v52);
        *a8 = 6;

LABEL_35:
        v62 = v53v.length();
        if ( v62 >= a5 || (v62 * 0.70711f) > fxf ) {
          *a8 = 0;
          return false;
        }

        *a7 = v30 - v53v.squaredLength();
      }
    }
    else {
      e1xe3 = edge3.dot(edge1);
      if ( (fxe3 * e1xe3) <= fxe1 ) {
LABEL_29:
        f32 v57 = ((fxe1 * e1xe3) - fxe3) / ((e1xe3 * e1xe3) - 1.0f);
        f32 v58 = fxe1 - (e1xe3 * v57);
        v53v = (edge3 * v57) + (edge1 * v58);
        *a8 = 7;
        goto LABEL_35;
      }

      if ( fxe3 > fxf )
        return false;

      *a7 = v30 - (fxe3 * fxe3);
      *a8 = 4;
    }
  }

  *a7 = sqrtf(fmaxf(*a7, 0.0f)) - fxf;
  if ( *a7 < 0.0 ) {
    *a8 = 0;
    return false;
  }

  return true;
}

s32 KCollisionServer::checkDisk(const sead::Vector3f*, f32, f32, const sead::Vector3f&, f32, u32,
                sead::FixedRingBuffer<KCHitInfo, 512>*) {}
bool KCollisionServer::KCHitDisk(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f*, f32, f32, f32,
                const sead::Vector3f&, f32*, u8*) {}

// NON_MATCHING: will match after `searchPrismMinMax` is completed
void KCollisionServer::searchPrism(sead::Vector3f* pos, f32 radius,
                    sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& delegate) {
    sead::Vector3f min = {pos->x - radius, pos->y - radius, pos->z - radius};
    sead::Vector3f max = {pos->x + radius, pos->y + radius, pos->z + radius};
    searchPrismMinMax(min, max, delegate);
}

// NON_MATCHING: surprisingly little mismatches, but still some major ones
void KCollisionServer::searchPrismMinMax(const sead::Vector3f& a2, const sead::Vector3f& a3,
                        sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& a4) {
    printf("KCollisionServer::searchPrismMinMax((%.02f, %.02f, %.02f), (%.02f, %.02f, %.02f), ...)\n", a2.x, a2.y, a2.z, a3.x, a3.y, a3.z);
    u64 v6;

    const u8 *v7 = 0LL;
    const u8 *v73 = 0LL;
    for(v6=0; (long)v6<(int)getNumInnerKcl(); v6++)
    {
      al::KCPrismHeader* header = mModelsData[v6];

      sead::Vector3u pos1, pos2;
      if ( outCheckAndCalcArea(&pos1, &pos2, a2, a3, header) )
      {
        sead::Vector3u copyPos1=pos1;
        sead::Vector3u copyPos2=pos2;
        do
        {
          int stepSizeZ = 1000000;
          do
          {
            int stepSizeY = 1000000;
            int v23 = 0;
            if ( v7 )
            {
              do
              {
                int mCoordShift;
                const u8* v32 = searchBlock(&mCoordShift, pos1, header);
                
                int v33 = 1 << mCoordShift;
                int stepSizeX = v33 - ((v33 - 1) & pos1.x);
                int v35 = v33 - ((v33 - 1) & pos1.y);
                int v36 = v33 - ((v33 - 1) & pos1.z);

                if ( v36 < stepSizeZ )
                  stepSizeZ = v36;
                if ( v35 < stepSizeY )
                  stepSizeY = v35;

                if ( v35 > v23 &&  *((short *)v32 + 1) ) {
                  v23 = v35;
                  v73 = v32;
                }

                u16* innerData = (u16*)(v32 + 2);
                if ( v32 == v7 ) {pos1.x += stepSizeX; continue;}
                
                int v39 = *innerData++;
                while ( v39 != 0xFFFF )
                {
                  const KCPrismData& data = getPrismData(v39, header);
                  if ( data.mLength > 0.0f )
                    a4.invoke(&data, header);

                  v39 = *innerData++;
                }

                pos1.x += stepSizeX;
              }
              while ( pos1.x <= copyPos2.x );
            }
            else
            {
              do
              {
                int mCoordShift;
                const u8* v32 = searchBlock(&mCoordShift, pos1, header);

                int v33 = 1 << mCoordShift;

                int stepSizeX = v33 - ((v33 - 1) & pos1.x);
                int v35 = v33 - ((v33 - 1) & pos1.y);
                int v36 = v33 - ((v33 - 1) & pos1.z);

                if ( v36 < stepSizeZ )
                  stepSizeZ = v36;
                if ( v35 < stepSizeY )
                  stepSizeY = v35;

                if ( v35 > v23 && *((short *)v32 + 1) ) {
                  v23 = v35;
                  v73 = v32;
                }

                u16* innerData = (u16*)(v32 + 2);
                int v39 = *innerData++;
                while ( v39 != 0xFFFF )
                {
                  const KCPrismData& data = getPrismData(v39, header);
                  if ( !(0.0f >= data.mLength) )
                    a4.invoke(&data, header);

                  v39 = *innerData++;
                }

                pos1.x += stepSizeX;
              }
              while ( pos1.x <= copyPos2.x );
            }

            pos1.x = copyPos1.x;
            v7 = v73;
            pos1.y += stepSizeY;
          }
          while ( pos1.y <= pos2.y );

          pos1.y = copyPos1.y;
          v7 = v73;
          pos1.z += stepSizeZ;
        }
        while ( pos1.z <= copyPos2.z );

        v7 = v73;
      }
    }
}

// FIXME inspect/correct after searchPrismMinMax is done
void KCollisionServer::searchPrismArrow(const sead::Vector3f& a2, const sead::Vector3f& a3,
                        sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& delegate) {
    float x; // s0
  float y; // s1
  float z; // s4
  float v7; // s3
  float v8; // s2
  float v9; // s5
  float v10; // s6
  float v11; // s2
  float v12; // s0
  float v13; // s0
  sead::Vector3f v15; // [xsp+0h] [xbp-20h] BYREF
  sead::Vector3f v16; // [xsp+10h] [xbp-10h] BYREF

  x = a2.x;
  y = a2.y;
  z = a2.z;
  v7 = y + a3.y;
  v8 = a2.x + a3.x;
  v9 = z + a3.z;
  if ( a2.x <= v8 )
    v10 = a2.x + a3.x;
  else
    v10 = a2.x;

  if ( x >= v8 )
    x = a2.x + a3.x;

  if ( y <= v7 )
    v11 = y + a3.y;
  else
    v11 = a2.y;

  if ( y >= v7 )
    y = y + a3.y;

  v15.x = x;
  v15.y = y;
  if ( z <= v9 )
    v12 = v9;
  else
    v12 = z;

  v16.z = v12;
  if ( z >= v9 )
    v13 = v9;
  else
    v13 = z;

  v16.x = v10;
  v16.y = v11;
  v15.z = v13;
  return searchPrismMinMax(v15, v16, delegate);
                        }

void KCollisionServer::searchPrismDisk(const sead::Vector3f& pos1, const sead::Vector3f& pos2, f32 val1, f32 val2,
                        sead::IDelegate2<const KCPrismData*, const KCPrismHeader*>& delegate) {
    f32 radiusX = calcCylinderRadiusDot(sead::Vector3f::ex, pos1, val2);
    f32 radiusY = calcCylinderRadiusDot(sead::Vector3f::ey, pos1, val2);
    f32 radiusZ = calcCylinderRadiusDot(sead::Vector3f::ez, pos1, val2);

    sead::Vector3f something = {
        sead::Mathf::min(pos2.x*val1 - pos1.x, pos1.x - pos2.x*val1),
        sead::Mathf::min(pos2.y*val1 - pos1.y, pos1.y - pos2.y*val1),
        sead::Mathf::min(pos2.z*val1 - pos1.z, pos1.z - pos2.z*val1)
    };

    sead::Vector3f vec1 = {
        something.x - radiusX,
        something.y - radiusY,
        something.z - radiusZ
    };
    sead::Vector3f vec2 = {
        something.x + radiusX,
        something.y + radiusY,
        something.z + radiusZ
    };

    searchPrismMinMax(vec1, vec2, delegate);
}

// NON_MATCHING: lists a bunch of early-return-true at the end of the function. Manually doing `if(...) return true` is unchanged.
bool KCollisionServer::isParallelNormal(const KCPrismData* data, const KCPrismHeader* header) const {
    sead::Vector3f edge1 = getEdgeNormal1(data, header);
    sead::Vector3f edge2 = getEdgeNormal2(data, header);
    sead::Vector3f edge3 = getEdgeNormal3(data, header);
    return (edge1 == edge2) || (edge1 == edge3) || (edge2 == edge3) || (edge1 == -edge2) || (edge1 == -edge3) || (edge2 == -edge3);
}

const sead::Vector3f& KCollisionServer::getFaceNormal(const KCPrismData* data, const KCPrismHeader* header) const {
    return getNormal(data->mFaceNormalIndex, header);
}

const sead::Vector3f& KCollisionServer::getEdgeNormal1(const KCPrismData* data, const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[0], header);
}

const sead::Vector3f& KCollisionServer::getEdgeNormal2(const KCPrismData* data, const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[1], header);
}

const sead::Vector3f& KCollisionServer::getEdgeNormal3(const KCPrismData* data, const KCPrismHeader* header) const {
    return getNormal(data->mEdgeNormalIndex[2], header);
}

bool KCollisionServer::KCHitDisc(const KCPrismData* data, const KCPrismHeader* header, const sead::Vector3f&,
                const sead::Vector3f&, f32, f32, sead::Vector3f*, f32*) {}

// NON_MATCHING: register swap
s32 KCollisionServer::toIndex(const KCPrismData* data, const KCPrismHeader* header) const {
    //return (reinterpret_cast<const u8*>(data) - reinterpret_cast<const u8*>(header) - header->mTrianglesOffset) / sizeof(KCPrismData);
    return 0xCCCCCCCD * ((uintptr_t(data) - uintptr_t(header) - header->mTrianglesOffset) >> 2);
}

const sead::Vector3f& KCollisionServer::getNormal(u32 index, const KCPrismHeader* header) const {
    return reinterpret_cast<const sead::Vector3f*>(uintptr_t(header) + header->mNormalsOffset)[(int)index];
}

void KCollisionServer::calXvec(const sead::Vector3f* a, const sead::Vector3f* b, sead::Vector3f* out) {
    out->x = a->z * b->y - a->y * b->z;
    out->y = a->x * b->z - a->z * b->x;
    out->z = a->y * b->x - a->x * b->y;
}

const sead::Vector3f& KCollisionServer::getVertexData(u32 index, const KCPrismHeader* header) const {
    return reinterpret_cast<const sead::Vector3f*>(uintptr_t(header) + header->mPositionsOffset)[(int)index];
}

u32 KCollisionServer::getVertexNum(const KCPrismHeader* header) const {
    //return (header->mNormalsOffset - (u64)header->mPositionsOffset) / sizeof(sead::Vector3f);
    return 0xAAAAAAAB * ((uintptr_t(header->mNormalsOffset) - header->mPositionsOffset) >> 2);
}

s32 KCollisionServer::getNormalNum(const KCPrismHeader* header) const {
    return 4 * ((header->mOctreeOffset - (u64)header->mTrianglesOffset) / sizeof(KCPrismData));
}

s32 KCollisionServer::getAttributeElementNum() const {
    return 0;
}

bool KCollisionServer::getAttributes(ByamlIter* iter, u32 prismIndex, const KCPrismHeader* header) const {
    return mIter->tryGetIterByIndex(iter, getPrismData(prismIndex, header).mCollisionType);
}

bool KCollisionServer::getAttributes(ByamlIter* iter, const KCPrismData* data) const {
    return mIter->tryGetIterByIndex(iter, data->mCollisionType);
}

void KCollisionServer::objectSpaceToAreaOffsetSpace(sead::Vector3u* areaOffSpace, const sead::Vector3f& objSpace,
                                        const KCPrismHeader* header) const {
                                            areaOffSpace->x = (s32)(objSpace.x - header->mOctreeOrigin.x);
                                            areaOffSpace->y = (s32)(objSpace.y - header->mOctreeOrigin.y);
                                            areaOffSpace->z = (s32)(objSpace.z - header->mOctreeOrigin.z);
                                        }

void KCollisionServer::areaOffsetSpaceToObjectSpace(sead::Vector3f* objSpace, const sead::Vector3u& areaOffSpace,
                                        const KCPrismHeader* header) const {
                                            *objSpace = sead::Vector3f{(float)areaOffSpace.x,(float) areaOffSpace.y, (float)areaOffSpace.z} + header->mOctreeOrigin;
                                        }

bool KCollisionServer::doBoxCheck(const sead::Vector3f* pos1, const sead::Vector3f* pos2, sead::Vector3u* posOut1, sead::Vector3u* posOut2,
                const KCPrismHeader* header) {
    sead::Vector3f min, max;
    sead::Vector3f sum = *pos1 + *pos2;
    if(pos1->x >= sum.x) {
        min.x = sum.x;
        max.x = pos1->x;
    } else {
        min.x = pos1->x;
        max.x = sum.x;
    }

    if(pos1->y >= sum.y) {
        min.y = sum.y;
        max.y = pos1->y;
    } else {
        min.y = pos1->y;
        max.y = sum.y;
    }

    if(pos1->z >= sum.z) {
        min.z = sum.z;
        max.z = pos1->z;
    } else {
        min.z = pos1->z;
        max.z = sum.z;
    }

    if(min.x == max.x) {
        min.x -= 1.0f;
        max.x += 1.0f;
    }
    if(min.y == max.y) {
        min.y -= 1.0f;
        max.y += 1.0f;
    }
    if(min.z == max.z) {
        min.z -= 1.0f;
        max.z += 1.0f;
    }

    return outCheckAndCalcArea(posOut1, posOut2, min, max, header);

}

// NON_MATCHING: Different order of loading mCoordShift
s32 KCollisionServer::calcAreaBlockOffset(const sead::Vector3u& pos, const KCPrismHeader* header) const {
    return 4 * ((pos.y >> header->mCoordShift.x << header->mCoordShift.y) | (pos.z >> header->mCoordShift.x << header->mCoordShift.z) | (pos.x >> header->mCoordShift.x));
}

s32 KCollisionServer::calcChildBlockOffset(const sead::Vector3u& position, s32 index) {
    return 4 * ((4 * ((position.z >> index) & 1)) | (2 * ((position.y >> index) & 1)) | ((position.x >> index) & 1));
}

u32 KCollisionServer::getBlockData(const u32* data, u32 offset) {
    return *reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(data)+offset);
}

void SphereInterpolator::startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd,
                                     f32 sizeStart, f32 sizeEnd, f32 steps) {
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;
    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}
void SphereInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SphereInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SphereInterpolator::calcInterp(sead::Vector3f* pos, f32* size,
                                    sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    calcRemainMoveVector(remainMoveVec);
}
void SphereInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
void SphereInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}
void SphereInterpolator::calcStepMoveVector(sead::Vector3f* moveVec) const {
    f32 step = mCurrentStep - mPrevStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

void SpherePoseInterpolator::startInterp(const sead::Vector3f& posStart,
                                         const sead::Vector3f& posEnd, f32 sizeStart, f32 sizeEnd,
                                         const sead::Quatf& quatStart, const sead::Quatf& quatEnd,
                                         f32 steps) {
    printf("SpherePoseInterpolator::startInterp(start=(%f, %f, %f), end=(%f, %f, %f), ..., steps=%f)\n", posStart.x, posStart.y, posStart.z, posEnd.x, posEnd.y, posEnd.z, steps);
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;

    mQuatStart.x = quatStart.x;
    mQuatStart.y = quatStart.y;
    mQuatStart.z = quatStart.z;
    mQuatStart.w = quatStart.w;

    mQuatEnd.x = quatEnd.x;
    mQuatEnd.y = quatEnd.y;
    mQuatEnd.z = quatEnd.z;
    mQuatEnd.w = quatEnd.w;

    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}

void SpherePoseInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SpherePoseInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SpherePoseInterpolator::calcInterp(sead::Vector3f* pos, f32* size, sead::Quatf* quat,
                                        sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    slerpQuat(quat, mQuatStart, mQuatEnd, mCurrentStep);
    quat->normalize();
    calcRemainMoveVector(remainMoveVec);
}
void SpherePoseInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
f32 SpherePoseInterpolator::calcRadiusBaseScale(f32 unk) const {
    return calcRate01(unk, 0.0f, mSizeEnd);
}
void SpherePoseInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

}  // namespace al
