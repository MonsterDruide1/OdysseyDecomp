#include "Library/Clipping/ClippingActorInfo.h"

#include "Library/Clipping/ClippingJudge.h"
#include "Library/Clipping/ClippingRequestKeeper.h"
#include "Library/Clipping/ViewIdHolder.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {
enum class ClippingRequestType;

ClippingActorInfo::ClippingActorInfo(LiveActor* actor)
    : mLiveActor(actor), mPlacementId(new PlacementId()) {
    mClippingRadius = 300.0f;
    mTransPtr = getTransPtr(mLiveActor);
    _20.setUndef();
}

void ClippingActorInfo::setTypeToSphere(f32 radius, const sead::Vector3f* pos) {
    mClippingRadius = radius;
    if (!pos)
        pos = getTransPtr(mLiveActor);
    mTransPtr = pos;
    _20.setUndef();
}

void ClippingActorInfo::startClipped() {
    if (!isDead(mLiveActor) && !isClipped(mLiveActor))
        mLiveActor->startClipped();
}

void ClippingActorInfo::endClipped() {
    if (!isDead(mLiveActor) && isClipped(mLiveActor))
        mLiveActor->endClipped();
}

void ClippingActorInfo::updateClipping(const ClippingJudge* clippingJudge) {
    bool isJudged;
    if (_20.isUndef())
        isJudged = clippingJudge->isJudgedToClipFrustum(*mTransPtr, mClippingRadius, _38, _4a);
    else
        isJudged = clippingJudge->isJudgedToClipFrustumObb(_18, _20, _38, _4a);
    if (!isJudged) {
        if (!isClipped(mLiveActor))
            return;
        if (isDead(mLiveActor))
            return;
        if (!isClipped(mLiveActor))
            return;

        mLiveActor->endClipped();
    } else {
        if (isClipped(mLiveActor) || isDead(mLiveActor) || isClipped(mLiveActor))
            return;
        mLiveActor->startClipped();
    }
}

bool ClippingActorInfo::judgeClipping(const ClippingJudge* clippingJudge) const {
    if (!_20.isUndef())
        return clippingJudge->isJudgedToClipFrustumObb(_18, _20, _38, _4a);
    return clippingJudge->isJudgedToClipFrustum(*mTransPtr, mClippingRadius, _38, _4a);
}

void ClippingActorInfo::updateClipping(ClippingRequestKeeper* clippingRequestKeeper,
                                       const ClippingJudge* clippingJudge) {
    bool isClipping = judgeClipping(clippingJudge);
    bool isClip = isClipped(mLiveActor);

    if (isClipping) {
        if (isClip)
            return;

        clippingRequestKeeper->request(mLiveActor, ClippingRequestType::IsClipped);
    } else {
        if (!isClip)
            return;

        clippingRequestKeeper->request(mLiveActor, ClippingRequestType::IsNotClipped);
    }
}

bool ClippingActorInfo::isGroupClipping() const {
    if (mIsEnableGroupClipping)
        return mPlacementId->isValid();
    return false;
}

bool ClippingActorInfo::isGroupClippingInit() const {
    return mPlacementId->isValid();
}

void ClippingActorInfo::setTypeToObb(const sead::BoundBox3f& boundBox,
                                     const sead::Matrix34f* matrix) {
    if (!matrix)
        matrix = mLiveActor->getBaseMtx();
    _18 = matrix;
    _20 = boundBox;
}

void ClippingActorInfo::setGroupClippingId(const ActorInitInfo& clippingId) {
    alPlacementFunction::getClippingGroupId(mPlacementId, clippingId);
}

void ClippingActorInfo::setFarClipLevel20M() {
    _48 = 1;
    _4a = 1;
    if (checkActiveViewGroupAny())
        _4a = 0;
}

void ClippingActorInfo::updateFarClipLevel() {
    _4a = _48;
    if (_48 != 0 && checkActiveViewGroupAny())
        _4a = 0;
}

void ClippingActorInfo::setFarClipLevelMax() {
    _48 = 0;
    _4a = 0;
}

bool ClippingActorInfo::isFarClipLevelMax() const {
    return _48 == 0;
}

bool ClippingActorInfo::checkActiveViewGroupAny() const {
    if (!(mFarClipFlags && 0 < mFarClipFlagSize))
        return false;
    for (s32 i = 0; i < mFarClipFlagSize; i++) {
        const bool* farClipFlag = mFarClipFlags[i];
        if (farClipFlag && *farClipFlag)
            return true;
    }
    return false;
}

void ClippingActorInfo::initViewGroup(const ViewIdHolder* viewIdHolder) {
    if (!viewIdHolder)
        return;
    mViewIdHolder = (ViewIdHolder*)viewIdHolder;
    mFarClipFlagEntries = viewIdHolder->getNumPlacements();
    mFarClipFlags = new const bool*[mFarClipFlagEntries];
    for (s32 i = 0; i < mFarClipFlagEntries; i++)
        mFarClipFlags[i] = nullptr;
}

void ClippingActorInfo::registerViewGroupFarClipFlag(const bool* flag) {
    mFarClipFlags[mFarClipFlagSize] = flag;
    mFarClipFlagSize++;
}

}  // namespace al
