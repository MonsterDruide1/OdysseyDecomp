#pragma once

#include <math/seadMatrix.h>
#include "al/include/Library/Placement/PlacementInfo.h"

namespace al {
class AreaObj;
class MtxConnector;
class PlacementInfo;
class ValidatorBase;

class AreaObjMtxConnecter {
private:
    AreaObj* mAreaObj;
    sead::Matrix34f mMatrix;
    MtxConnector* mMtxConnector = nullptr;
    PlacementInfo mPlacementInfo;
    ValidatorBase* mValidatorBase = nullptr;

public:
    AreaObjMtxConnecter(AreaObj* areaObj, const PlacementInfo& placementInfo);
    bool trySetParentMtx(const sead::Matrix34f* parentMtx, const PlacementInfo& placementInfo,
                         const ValidatorBase* validatorBase);
    bool trySyncParentMtx(const sead::Matrix34f* parentMtx, const PlacementInfo& placementInfo,
                          const ValidatorBase* validatorBase);
    void update();
};

class AreaObjMtxConnecterHolder {
private:
    MtxConnector** mMtxConnectors;
    s32 mNumConnectors = 0;
    s32 mCapacity;

public:
    AreaObjMtxConnecterHolder(s32 capacity);
    void registerParentMtx(const sead::Matrix34f* parentMtx, const PlacementInfo& placementInfo,
                           const ValidatorBase* validatorBase);
    void registerSyncParentMtx(const sead::Matrix34f* parentMtx, const PlacementInfo& placementInfo,
                               const ValidatorBase* validatorBase);
    s32 tryAddArea(AreaObj* areaObj, const PlacementInfo& placementInfo);
    void update();
};

}  // namespace al
