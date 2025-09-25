#include "Library/Area/AreaObjMtxConnecter.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/ValidatorBase.h"
#include "Library/Collision/PartsMtxConnector.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
AreaObjMtxConnecter::AreaObjMtxConnecter(AreaObj* areaObj, const PlacementInfo& placementInfo)
    : mAreaObj(areaObj), mPlacementInfo(placementInfo) {
    mMatrix = mAreaObj->getAreaMtx();
}

bool AreaObjMtxConnecter::trySetParentMtx(const sead::Matrix34f* parentMtx,
                                          const PlacementInfo& placementInfo,
                                          const ValidatorBase* validatorBase) {
    if (!isEqualPlacementId(mPlacementInfo, placementInfo))
        return false;

    mMtxConnector = new MtxConnector();
    mMtxConnector->init(parentMtx);

    if (validatorBase)
        mValidatorBase = validatorBase;

    return true;
}

bool AreaObjMtxConnecter::trySyncParentMtx(const sead::Matrix34f* parentMtx,
                                           const PlacementInfo& placementInfo,
                                           const ValidatorBase* validatorBase) {
    if (!isEqualPlacementId(mPlacementInfo, placementInfo))
        return false;

    mMatrix = *parentMtx;
    mMtxConnector = new MtxConnector();
    mMtxConnector->init(parentMtx);

    if (validatorBase)
        mValidatorBase = validatorBase;

    return true;
}

void AreaObjMtxConnecter::update() {
    if (mValidatorBase) {
        if (mValidatorBase->validate() && !mAreaObj->isValid())
            mAreaObj->validate();
        else if (!mValidatorBase->validate() && mAreaObj->isValid())
            mAreaObj->invalidate();
    }

    if (mMtxConnector)
        mMtxConnector->multMtx(mAreaObj->getAreaMtxPtr(), mMatrix);
}

AreaObjMtxConnecterHolder::AreaObjMtxConnecterHolder(s32 capacity) : mCapacity(capacity) {
    mMtxConnectors = new AreaObjMtxConnecter*[mCapacity];

    for (s32 i = 0; i < mCapacity; i++)
        mMtxConnectors[i] = nullptr;
}

bool AreaObjMtxConnecterHolder::tryAddArea(AreaObj* areaObj, const PlacementInfo& placementInfo) {
    if (calcLinkChildNum(placementInfo, "NoDelete_FollowMtxTarget") == 0)
        return false;

    PlacementInfo linkPlacementInfo;
    getLinksInfo(&linkPlacementInfo, placementInfo, "NoDelete_FollowMtxTarget");
    mMtxConnectors[mNumConnectors] = new AreaObjMtxConnecter(areaObj, linkPlacementInfo);
    mNumConnectors++;

    return true;
}

void AreaObjMtxConnecterHolder::registerParentMtx(const sead::Matrix34f* parentMtx,
                                                  const PlacementInfo& placementInfo,
                                                  const ValidatorBase* validatorBase) {
    for (s32 i = 0; i < mNumConnectors; i++)
        mMtxConnectors[i]->trySetParentMtx(parentMtx, placementInfo, validatorBase);
}

void AreaObjMtxConnecterHolder::registerSyncParentMtx(const sead::Matrix34f* parentMtx,
                                                      const PlacementInfo& placementInfo,
                                                      const ValidatorBase* validatorBase) {
    for (s32 i = 0; i < mNumConnectors; i++)
        mMtxConnectors[i]->trySyncParentMtx(parentMtx, placementInfo, validatorBase);
}

void AreaObjMtxConnecterHolder::update() {
    for (s32 i = 0; i < mNumConnectors; i++)
        mMtxConnectors[i]->update();
}
}  // namespace al
