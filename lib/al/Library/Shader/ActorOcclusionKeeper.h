#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class Resource;
class GraphicsSystemInfo;
class LiveActor;
class MtxConnector;

class OccSphere {
public:
    OccSphere();

    void calcConnectPos(sead::Vector3f*) const;
    void connectToHostJoint(const LiveActor*);
    void tryCalcLightPosDir(sead::Vector3f*, const sead::Vector3f&) const;
    void setEnable(bool);
    void postUpdate();

    bool isExistLightPosPtr() const { return mLightPosPtr != nullptr; }

    void setLightPosPtr(const sead::Vector3f* lightPosPtr) { mLightPosPtr = lightPosPtr; }

    void setAddOffset(const sead::Vector3f& offset) { mAddOffset.set(offset); }

    bool isEnable() const { return mIsEnable; }

    f32 getRadius() const { return mRadius; }

    f32 getDoRange() const { return mDoRange; }

    f32 getBoundingDistanceScale() const { return mBoundingDistanceScale; }

    const sead::FixedSafeString<32>& getGroupName() const { return mGroupName; }

    sead::FixedSafeString<32>* getGroupNamePtr() { return &mGroupName; }

private:
    u8 _0[0x90];
    f32 mRadius;
    u8 _94[0x5c];
    f32 mDoRange;
    u8 _f4[0xbc];
    sead::FixedSafeString<32> mGroupName;
    u8 _1e8[0x2c];
    sead::Vector3f mAddOffset;
    const sead::Vector3f* mLightPosPtr;
    bool _228;
    bool mIsEnable;
    bool _22a;
    u8 _22b;
    f32 mBoundingDistanceScale;
    f32 _230;
    u8 _234[4];
    MtxConnector* mMtxConnector;
};

static_assert(sizeof(OccSphere) == 0x240);

class ActorOcclusionKeeper {
public:
    static bool isExistFile(const Resource*, const char*);

    ActorOcclusionKeeper(const GraphicsSystemInfo*, const Resource*, const LiveActor*, const char*);
    void appear(bool isModelHidden);
    void requestKill();
    void updateAndRequest();
    void hideModel();
    void calcDirectionalLight(sead::Vector3f*) const;
    f32 calcSystemConeDegree() const;
    OccSphere* getOccSphere(const char*);

    void setIgnoreHostHide(bool isIgnoreHostHide) { mIsIgnoreHostHide = isIgnoreHostHide; }

private:
    void* _0[0x290 / 8];
    bool mIsIgnoreHostHide;
    u8 _291[7];
};

static_assert(sizeof(ActorOcclusionKeeper) == 0x298);

}  // namespace al
