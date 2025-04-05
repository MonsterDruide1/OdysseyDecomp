#include "Enemy/HackerDepthShadowMapCtrl.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Player/PlayerCollider.h"

HackerDepthShadowMapCtrl::HackerDepthShadowMapCtrl(al::LiveActor* actor, const char* shadowMapName,
                                                   f32 shadowLengthOffset, f32 lerpLength,
                                                   f32 lerpGradationLength)
    : mActor(actor), mShadowMapName(shadowMapName), mShadowLengthOffset(shadowLengthOffset),
      mLerpLength(lerpLength), mLerpGradationLength(lerpGradationLength) {
    al::setEnableDepthShadowMapBottomGradation(actor, shadowMapName, true);
}

void HackerDepthShadowMapCtrl::resetAndUpdate() {
    f32 prevLerpLength = mLerpLength;
    f32 prevLerpGradationLength = mLerpGradationLength;
    mOnGroundTimer = 0;
    mIsActive = false;
    mLerpLength = 1.0f;
    mLerpGradationLength = 1.0f;
    update(nullptr);
    mIsActive = true;
    mLerpLength = prevLerpLength;
    mLerpGradationLength = prevLerpGradationLength;
}

void HackerDepthShadowMapCtrl::update(PlayerCollider* playerCollider) {
    if (playerCollider && mIsActive) {
        if (playerCollider->get_70() >= 0.0f) {
            if (mOnGroundTimer <= 2)
                mOnGroundTimer++;
        } else if (mOnGroundTimer > 0)
            mOnGroundTimer--;
    }

    sead::Vector3f groundNormal = sead::Vector3f::ey;
    f32 groundDistance;

    al::AreaObj* shadowLengthArea = al::tryFindAreaObj(mActor, "GroundShadowLengthReviseArea");

    if (shadowLengthArea) {
        const sead::Vector3f& actorTrans = al::getTrans(mActor);
        sead::Vector3f areaScale = {0.0f, 0.0f, 0.0f};
        al::tryGetScale(&areaScale, shadowLengthArea->getPlacementInfo());

        const sead::Matrix34f& areaMtx = al::getAreaObjBaseMtx(shadowLengthArea);

        sead::Vector3f areaPos;
        areaMtx.getTranslation(areaPos);

        sead::Vector3f areaUp = {0.0f, 0.0f, 0.0f};
        areaMtx.getBase(areaUp, 1);

        sead::Vector3f offset = actorTrans;
        offset -= areaPos;

        al::verticalizeVec(&offset, areaUp, offset);

        sead::Vector3f offsetAreaPos = areaPos + offset;
        sead::Vector3f difference = (offsetAreaPos - al::getTrans(mActor));

        shadowLengthArea->getAreaMtx().getBase(groundNormal, 1);
        groundDistance = difference.length();

    } else if (mIsActive && playerCollider && mOnGroundTimer >= 1) {
        groundNormal = playerCollider->getCollidedGroundNormal();
        groundDistance = 0.0f;
    } else if (mIsActive && !playerCollider && al::isExistActorCollider(mActor) &&
               al::isOnGround(mActor, 3)) {
        groundNormal = al::getCollidedGroundNormal(mActor);
        groundDistance = 0.0f;
    } else {
        groundDistance = 1700.0f;

        sead::Vector3f hitPos;
        sead::Vector3f hitNormal;
        if (alCollisionUtil::getHitPosAndNormalOnArrow(
                mActor, &hitPos, &hitNormal, al::getTrans(mActor) + (sead::Vector3f::ey * 100.0f),
                sead::Vector3f::ey * -1800.0f, nullptr, nullptr)) {
            if (hitPos.y <= al::getTrans(mActor).y) {
                const sead::Vector3f& actorTrans = al::getTrans(mActor);
                groundDistance = actorTrans.y - hitPos.y;
                groundNormal = hitNormal;
            }
        }
    }

    // 1 for wall, 0 for flat ground
    f32 tilt = sead::Mathf::clamp(1.0f - groundNormal.dot(sead::Vector3f::ey), 0.0f, 1.0f);

    f32 gradationTargetLength = groundDistance + (tilt * 300.0f);
    f32 targetLength;
    if (shadowLengthArea)
        targetLength = gradationTargetLength + 40.0f;
    else {
        gradationTargetLength += mShadowLengthOffset;
        targetLength = gradationTargetLength * 1.8f;
    }
    f32 depthShadowLength = al::getDepthShadowMapLength(mActor, mShadowMapName);
    al::setDepthShadowMapLength(
        mActor, depthShadowLength * (1.0f - mLerpLength) + (targetLength * mLerpLength),
        mShadowMapName);

    if (al::isEnableDepthShadowMapBottomGradation(mActor, mShadowMapName)) {
        f32 currentLength = al::getDepthShadowMapLength(mActor, mShadowMapName);
        mGradationLength = ((1.0f - mLerpGradationLength) * mGradationLength) +
                           (gradationTargetLength * mLerpGradationLength);

        al::setDepthShadowMapBottomGradationLength(
            mActor, mShadowMapName,
            sead::Mathf::clampMin((currentLength - mGradationLength) - (currentLength * 0.05f),
                                  0.0f));
    }
}
