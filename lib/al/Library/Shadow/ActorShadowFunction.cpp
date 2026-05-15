#include "Library/Shadow/ActorShadowUtil.h"

#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Model/ModelCtrl.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Shader/ActorOcclusionKeeper.h"
#include "Library/Shadow/DepthShadowMapCtrl.h"
#include "Library/Shadow/DepthShadowMapDirector.h"
#include "Library/Shadow/ShadowDirector.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Shadow/ShadowMaskCtrl.h"
#include "Library/Shadow/ShadowMaskCube.h"
#include "Library/Shadow/ShadowMaskDirector.h"

#include <math/seadMathCalcCommon.h>

namespace agl::sdw {
void calcSphereDoBoundingConeDegree(sead::Vector3f*, f32*, const sead::Vector3f&,
                                    const sead::Vector3f&, f32, f32, f32);
}

namespace ShadowMaskFunction {
al::ShadowMaskDirector* getShadowMaskDirector(const al::LiveActor*);
}

namespace DepthShadowMapFunction {
al::DepthShadowMapDirector* getDepthShadowMapDirector(const al::LiveActor*);
}

namespace {

al::ShadowMaskCtrl* getShadowMaskCtrl(const al::LiveActor* actor) {
    return actor->getShadowKeeper()->getShadowMaskCtrl();
}

al::DepthShadowMapCtrl* getDepthShadowMapCtrl(const al::LiveActor* actor) {
    return actor->getShadowKeeper()->getDepthShadowMapCtrl();
}

al::ShadowMaskDirector* getShadowMaskDirector(const al::LiveActor* actor) {
    return ShadowMaskFunction::getShadowMaskDirector(actor);
}

al::DepthShadowMapDirector* getDepthShadowMapDirector(const al::LiveActor* actor) {
    return DepthShadowMapFunction::getDepthShadowMapDirector(actor);
}

al::ShadowMaskCube* getCubeMask(al::ShadowMaskBase* mask) {
    // TODO: verify a cast-free typed lookup path for cube-only mask fields.
    return static_cast<al::ShadowMaskCube*>(mask);
}

void showShadowMaskImpl(const al::LiveActor* actor, al::ShadowMaskBase* mask) {
    if (mask->isHidden()) {
        mask->setHidden(false);
        if (mask->isValid())
            getShadowMaskDirector(actor)->registerUpdateShadowMask(mask);
    }
}

void hideShadowMaskImpl(const al::LiveActor* actor, al::ShadowMaskBase* mask) {
    if (!mask->isHidden() && !mask->isIgnoreHide()) {
        mask->setHidden(true);
        if (mask->isValid())
            getShadowMaskDirector(actor)->removeUpdateShadowMask(mask);
    }
}

void validateShadowMaskImpl(const al::LiveActor* actor, al::ShadowMaskBase* mask) {
    if (!mask->isValid()) {
        mask->setValid(true);
        if (!mask->isHidden())
            getShadowMaskDirector(actor)->registerUpdateShadowMask(mask);
    }
}

void invalidateShadowMaskImpl(const al::LiveActor* actor, al::ShadowMaskBase* mask) {
    if (mask->isValid()) {
        mask->setValid(false);
        if (!mask->isHidden())
            getShadowMaskDirector(actor)->removeUpdateShadowMask(mask);
    }
}

void invalidateShadowMaskInCategory(const al::LiveActor* actor,
                                    al::ShadowMaskDrawCategory drawCategory) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        al::ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask && int(mask->getDrawCategory()) == int(drawCategory)) {
            hideShadowMaskImpl(actor, mask);
            mask->setValid(false);
        }
    }
}

void showDepthShadowMapImpl(const al::LiveActor* actor, al::DepthShadowMapInfo* map) {
    if (map->isHidden()) {
        map->setHidden(false);
        if (map->isValid())
            getDepthShadowMapDirector(actor)->registerUpdateDepthShadowMap(map);
    }
}

void hideDepthShadowMapImpl(const al::LiveActor* actor, al::DepthShadowMapInfo* map) {
    if (!map->isHidden()) {
        map->setHidden(true);
        if (map->isValid())
            getDepthShadowMapDirector(actor)->removeUpdateDepthShadowMap(map);
    }
}

void validateDepthShadowMapImpl(const al::LiveActor* actor, al::DepthShadowMapInfo* map) {
    if (!map->isValid()) {
        map->setValid(true);
        if (!map->isHidden())
            getDepthShadowMapDirector(actor)->registerUpdateDepthShadowMap(map);
    }
}

void invalidateDepthShadowMapImpl(const al::LiveActor* actor, al::DepthShadowMapInfo* map) {
    if (map->isValid()) {
        map->setValid(false);
        if (!map->isHidden())
            getDepthShadowMapDirector(actor)->removeUpdateDepthShadowMap(map);
    }
}

}  // namespace

namespace al {

bool isExistShadow(LiveActor* actor) {
    ShadowMaskCtrl* shadowMaskCtrl = getShadowMaskCtrl(actor);
    if (shadowMaskCtrl && shadowMaskCtrl->getShadowMaskNum() > 0)
        return true;
    return getDepthShadowMapCtrl(actor) != nullptr;
}

bool isExistShadowMaskCtrl(LiveActor* actor) {
    return getShadowMaskCtrl(actor) != nullptr;
}

bool isExistShadowMask(LiveActor* actor, const char* maskName) {
    ShadowMaskCtrl* shadowMaskCtrl = getShadowMaskCtrl(actor);
    if (shadowMaskCtrl)
        return shadowMaskCtrl->findShadowMask(maskName) != nullptr;
    return false;
}

bool isExistDepthShadow(LiveActor* actor) {
    const ModelCtrl* modelCtrl = actor->getModelKeeper()->getModelCtrl();
    if (!modelCtrl)
        return false;
    return modelCtrl->isDepthShadowModel();
}

bool isExistDepthShadowMapCtrl(LiveActor* actor) {
    return getDepthShadowMapCtrl(actor) != nullptr;
}

bool isExistAnyShadowCtrl(LiveActor* actor) {
    return getShadowMaskCtrl(actor) != nullptr || getDepthShadowMapCtrl(actor) != nullptr;
}

bool isHideShadowMask(const LiveActor* actor) {
    ShadowMaskCtrl* shadowMaskCtrl = getShadowMaskCtrl(actor);
    if (shadowMaskCtrl)
        return shadowMaskCtrl->isHide();
    return false;
}

void hideShadow(LiveActor* actor) {
    ShadowKeeper* shadowKeeper = actor->getShadowKeeper();
    if (shadowKeeper)
        shadowKeeper->hide();
}

void showShadow(LiveActor* actor) {
    ShadowKeeper* shadowKeeper = actor->getShadowKeeper();
    if (shadowKeeper)
        shadowKeeper->show();
}

void showShadowMask(LiveActor* actor) {
    getShadowMaskCtrl(actor)->show();
}

void showShadowMask(LiveActor* actor, ShadowMaskBase* mask) {
    showShadowMaskImpl(actor, mask);
}

void showShadowMask(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        showShadowMaskImpl(actor, mask);
}

void showShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask && int(mask->getDrawCategory()) == int(drawCategory))
            showShadowMaskImpl(actor, mask);
    }
}

void hideShadowMask(LiveActor* actor) {
    getShadowMaskCtrl(actor)->hide();
}

void hideShadowMask(LiveActor* actor, ShadowMaskBase* mask) {
    hideShadowMaskImpl(actor, mask);
}

void hideShadowMask(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        hideShadowMaskImpl(actor, mask);
}

void hideShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask && int(mask->getDrawCategory()) == int(drawCategory))
            hideShadowMaskImpl(actor, mask);
    }
}

bool isHideShadowMask(LiveActor* actor, const char* maskName) {
    return getShadowMaskCtrl(actor)->findShadowMask(maskName)->isHidden();
}

void showDepthShadowMap(LiveActor* actor, s32 mapIdx) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->getDepthShadowMapInfo(mapIdx);
    showDepthShadowMapImpl(actor, map);
}

void showDepthShadowMap(const LiveActor* actor, DepthShadowMapInfo* map) {
    showDepthShadowMapImpl(actor, map);
}

void showDepthShadowMap(LiveActor* actor, const char* mapName) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName);
    showDepthShadowMapImpl(actor, map);
}

void hideDepthShadowMap(LiveActor* actor, s32 mapIdx) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->getDepthShadowMapInfo(mapIdx);
    hideDepthShadowMapImpl(actor, map);
}

void hideDepthShadowMap(const LiveActor* actor, DepthShadowMapInfo* map) {
    hideDepthShadowMapImpl(actor, map);
}

void hideDepthShadowMap(LiveActor* actor, const char* mapName) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName);
    hideDepthShadowMapImpl(actor, map);
}

bool isHideDepthShadowMap(LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->isHidden();
}

bool isHideDepthShadowMap(LiveActor* actor, s32 mapIdx) {
    return getDepthShadowMapCtrl(actor)->getDepthShadowMapInfo(mapIdx)->isHidden();
}

s32 getDepthShadowMapNum(LiveActor* actor) {
    DepthShadowMapCtrl* ctrl = getDepthShadowMapCtrl(actor);
    if (ctrl)
        return ctrl->getDepthShadowMapNum();
    return 0;
}

void validateShadowMask(LiveActor* actor) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        validateShadowMaskImpl(actor, mask);
    }
}

void validateShadowMask(LiveActor* actor, ShadowMaskBase* mask) {
    validateShadowMaskImpl(actor, mask);
}

void validateShadowMask(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        validateShadowMaskImpl(actor, mask);
}

void invalidateShadowMask(LiveActor* actor) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        invalidateShadowMaskImpl(actor, mask);
    }
}

void invalidateShadowMask(LiveActor* actor, ShadowMaskBase* mask) {
    invalidateShadowMaskImpl(actor, mask);
}

void invalidateShadowMask(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        invalidateShadowMaskImpl(actor, mask);
}

void validateDepthShadowMap(LiveActor* actor) {
    for (s32 i = 0; i < getDepthShadowMapNum(actor); i++) {
        DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->getDepthShadowMapInfo(i);
        validateDepthShadowMapImpl(actor, map);
    }
}

void validateDepthShadowMap(LiveActor* actor, DepthShadowMapInfo* map) {
    validateDepthShadowMapImpl(actor, map);
}

void validateDepthShadowMap(LiveActor* actor, const char* mapName) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName);
    validateDepthShadowMapImpl(actor, map);
}

void invalidateDepthShadowMap(LiveActor* actor) {
    for (s32 i = 0; i < getDepthShadowMapNum(actor); i++) {
        DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->getDepthShadowMapInfo(i);
        invalidateDepthShadowMapImpl(actor, map);
    }
}

void invalidateDepthShadowMap(LiveActor* actor, DepthShadowMapInfo* map) {
    invalidateDepthShadowMapImpl(actor, map);
}

void invalidateDepthShadowMap(LiveActor* actor, const char* mapName) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName);
    invalidateDepthShadowMapImpl(actor, map);
}

void invalidateShadowMask(LiveActor* actor, ShadowMaskDrawCategory drawCategory) {
    invalidateShadowMaskInCategory(actor, drawCategory);
}

void invalidateShadowMaskIntensityAll(LiveActor* actor) {
    invalidateShadowMaskInCategory(actor, ShadowMaskDrawCategory::シャドウ);
    invalidateShadowMaskInCategory(actor, ShadowMaskDrawCategory::AO);
    invalidateShadowMaskInCategory(actor, ShadowMaskDrawCategory::ライトスケール);
    invalidateShadowMaskInCategory(actor, ShadowMaskDrawCategory::ライトバッファ加算);
}

void setShadowMaskFixed(LiveActor* actor, bool isFixed) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask)
            mask->setShadowFixed(isFixed);
    }
}

void setShadowMaskDropDir(LiveActor* actor, const sead::Vector3f& dropDir) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask)
            mask->setDropDir(dropDir);
    }
}

void setShadowMaskDropDir(LiveActor* actor, const sead::Vector3f& dropDir, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        mask->setDropDir(dropDir);
}

void setShadowMaskDropDirActorDown(LiveActor* actor) {
    sead::Vector3f dropDir;
    calcUpDir(&dropDir, actor);
    ShadowMaskCtrl* ctrl = getShadowMaskCtrl(actor);
    if (ctrl->getShadowMaskNum() < 1)
        return;

    f32 dropDirX = -dropDir.x;
    f32 dropDirY = -dropDir.y;
    f32 dropDirZ = -dropDir.z;
    for (s32 i = 0;; i++) {
        ShadowMaskBase* mask = ctrl->getShadowMask(i);
        if (mask)
            mask->setDropDir(dropDirX, dropDirY, dropDirZ);
        ctrl = getShadowMaskCtrl(actor);
        if (i + 1 >= ctrl->getShadowMaskNum())
            break;
    }
}

void setShadowMaskSize(LiveActor* actor, const char* maskName, const sead::Vector3f& size) {
    f32 sizeX = size.x;
    f32 sizeZ = size.z;
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (!mask)
        return;

    if (mask->getShadowMaskType() == ShadowMaskType::Sphere)
        mask->setSizeX(sizeX);
    else if (mask->getShadowMaskType() == ShadowMaskType::Cube) {
        mask->setSizeX(sizeX);
        getCubeMask(mask)->setSizeZ(sizeZ);
    } else if (mask->getShadowMaskType() == ShadowMaskType::Cylinder)
        mask->setSizeX(sizeX);
}

void setShadowMaskSize(LiveActor* actor, const char* maskName, f32 sizeX, f32, f32 sizeZ) {
    f32 shadowSizeZ = sizeZ;
    f32 shadowSizeX = sizeX;
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (!mask)
        return;

    if (mask->getShadowMaskType() == ShadowMaskType::Sphere)
        mask->setSizeX(shadowSizeX);
    else if (mask->getShadowMaskType() == ShadowMaskType::Cube) {
        mask->setSizeX(shadowSizeX);
        getCubeMask(mask)->setSizeZ(shadowSizeZ);
    } else if (mask->getShadowMaskType() == ShadowMaskType::Cylinder)
        mask->setSizeX(shadowSizeX);
}

void setShadowTextureOffset(LiveActor* actor, const char* maskName, f32 offsetX, f32 offsetY) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask && mask->getShadowMaskType() == ShadowMaskType::Cube)
        getCubeMask(mask)->setTextureOffset(offsetX, offsetY);
}

void onShadowTextureScroll(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask && mask->getShadowMaskType() == ShadowMaskType::Cube)
        getCubeMask(mask)->setTextureScroll(true);
}

void offShadowTextureScroll(LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask && mask->getShadowMaskType() == ShadowMaskType::Cube)
        getCubeMask(mask)->setTextureScroll(false);
}

void calcShadowMaskSize(sead::Vector3f* size, LiveActor* actor, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);

    if (mask->getShadowMaskType() == ShadowMaskType::Cube) {
        size->x = mask->getSizeX();
        size->y = 0.0f;
        size->z = getCubeMask(mask)->getSizeZ();
    } else if (mask->getShadowMaskType() == ShadowMaskType::Cylinder) {
        f32 sizeX = mask->getSizeX();
        size->z = sizeX;
        size->y = 0.0f;
        size->x = sizeX;
    }
}

f32 getShadowMaskDropLength(const LiveActor* actor, const char* maskName) {
    return getShadowMaskCtrl(actor)->findShadowMask(maskName)->getDropLength();
}

void setShadowMaskDropLength(LiveActor* actor, f32 dropLength) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask)
            mask->setDropLength(dropLength);
    }
}

void setShadowMaskDropLength(LiveActor* actor, f32 dropLength, const char* maskName) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        mask->setDropLength(dropLength);
}

void setShadowMaskDropLengthScaleWithDrawCategory(LiveActor* actor, f32 dropLengthScale,
                                                  ShadowMaskDrawCategory drawCategory) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask && int(mask->getDrawCategory()) == int(drawCategory))
            mask->scaleDropLength(dropLengthScale);
    }
}

void setShadowMaskDropLengthWithDrawCategory(LiveActor* actor, f32 dropLength,
                                             ShadowMaskDrawCategory drawCategory) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask && int(mask->getDrawCategory()) == int(drawCategory))
            mask->setDropLength(dropLength);
    }
}

void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase* targetMask, const char* maskName,
                                           const sead::Vector3f& planeNormal) {
    ShadowMaskBase* mask = getShadowMaskCtrl(targetMask->getHost())->findShadowMask(maskName);
    setShadowMaskDropLengthEvenWithTarget(targetMask, mask, planeNormal);
}

void setShadowMaskDropLengthEvenWithTarget(ShadowMaskBase* targetMask, const ShadowMaskBase* mask,
                                           const sead::Vector3f& planeNormal) {
    if (!mask || mask == targetMask)
        return;

    sead::Vector3f targetCenter;
    f32 targetCenterX = targetMask->getDropLengthEvenCenterX();
    targetCenter.x = targetCenterX;
    f32 targetCenterY = targetMask->getDropLengthEvenCenterY();
    targetCenter.y = targetCenterY;
    f32 targetCenterZ = targetMask->getDropLengthEvenCenterZ();
    targetCenter.z = targetCenterZ;

    sead::Vector3f targetSize;
    f32 targetSizeX = targetMask->getDropLengthEvenSizeX();
    f32 targetSizeY = targetMask->getDropLengthEvenSizeY();
    f32 targetSizeZ = targetMask->getDropLengthEvenSizeZ();
    targetSize.x = targetSizeX;
    targetSize.y = targetSizeY;
    targetSize.z = targetSizeZ;
    if (isNearZero(targetSize, 0.001f)) {
        targetMask->setDropLength(0.0f);
        return;
    }

    if (isNearZero(targetSize.dot(planeNormal), 0.001f)) {
        targetMask->setDropLength(0.0f);
        return;
    }

    targetCenter += targetSize * 0.5f;

    sead::Vector3f maskPlanePoint;
    f32 maskCenterX = mask->getDropLengthEvenCenterX();
    f32 maskCenterY = mask->getDropLengthEvenCenterY();
    f32 maskCenterZ = mask->getDropLengthEvenCenterZ();
    f32 maskSizeX = mask->getDropLengthEvenSizeX();
    f32 maskSizeY = mask->getDropLengthEvenSizeY();
    f32 maskSizeZ = mask->getDropLengthEvenSizeZ();
    f32 maskSizeHalfX = maskSizeX * 0.5f;
    f32 maskSizeHalfY = maskSizeY * 0.5f;
    f32 maskSizeHalfZ = maskSizeZ * 0.5f;
    f32 maskPlaneX = maskCenterX - maskSizeHalfX;
    maskPlanePoint.x = maskPlaneX;
    f32 maskPlaneY = maskCenterY - maskSizeHalfY;
    maskPlanePoint.y = maskPlaneY;
    f32 maskPlaneZ = maskCenterZ - maskSizeHalfZ;
    maskPlanePoint.z = maskPlaneZ;
    sead::Vector3f maskSize(maskSizeX, maskSizeY, maskSizeZ);
    (void)maskSize.length();

    sead::Vector3f reverseTargetSize = -targetSize;
    f32 distance =
        calcDistanceVecToPlane(reverseTargetSize, targetCenter, planeNormal, maskPlanePoint);
    if (!(distance < 0.0f))
        targetMask->setDropLength(distance);
}

void setShadowMaskDropLengthEvenWithDrawCategory(LiveActor* actor,
                                                 ShadowMaskDrawCategory drawCategory,
                                                 const LiveActor* targetActor,
                                                 const char* maskName) {
    ShadowMaskBase* targetMask = getShadowMaskCtrl(targetActor)->findShadowMask(maskName);
    ShadowMaskCtrl* ctrl = getShadowMaskCtrl(actor);
    s32 lastIdx = ctrl->getShadowMaskNum() - 1;
    if (lastIdx < 0)
        return;

    for (u64 i = 0;; i++) {
        if (u64(u32(ctrl->getShadowMaskNum())) > i) {
            ShadowMaskBase* mask = ctrl->getShadowMask(i);
            if (mask && int(mask->getDrawCategory()) == int(drawCategory))
                setShadowMaskDropLengthEvenWithTarget(mask, targetMask, sead::Vector3f::ey);
        }
        if (lastIdx == s32(i))
            break;
    }
}

void setShadowMaskDropLengthEvenPlaneNormal(const LiveActor* actor,
                                            const sead::Vector3f& planeNormal) {
    for (s32 i = 0; i < getShadowMaskCtrl(actor)->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = getShadowMaskCtrl(actor)->getShadowMask(i);
        if (mask)
            mask->setDropLengthPlaneNormal(planeNormal);
    }
}

s32 getDepthShadowMapWidth(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->getWidth();
}

s32 getDepthShadowMapHeight(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->getHeight();
}

void setDepthShadowMapSize(const LiveActor* actor, s32 width, s32 height, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setShadowTextureSize(width,
                                                                                           height);
}

f32 getDepthShadowMapLength(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->getLength();
}

void setDepthShadowMapLength(const LiveActor* actor, f32 length, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setLength(length);
}

void setDepthShadowMapLengthFromActorTransFlag(const LiveActor* actor, bool isSetShadowLength,
                                               const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setLengthFromActorTrans(
        isSetShadowLength);
}

void setDepthShadowMapBoundingBox(const LiveActor* actor, const sead::Vector3f& boundMin,
                                  const sead::Vector3f& boundMax, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setBoundingBox(boundMin,
                                                                                     boundMax);
}

void setDepthShadowMapMaskTypeNone(const LiveActor* actor, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setMaskType(0);
}

void setDepthShadowMapMaskTypeSelf(const LiveActor* actor, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setMaskType(1);
}

void setEnableDepthShadowMapBottomGradation(const LiveActor* actor, const char* mapName,
                                            bool isEnableBottomGradation) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setEnableBottomGradation(
        isEnableBottomGradation);
}

bool isEnableDepthShadowMapBottomGradation(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)
        ->tryFindDepthShadowMapInfo(mapName)
        ->isEnableBottomGradation();
}

void setDepthShadowMapBottomGradationLength(const LiveActor* actor, const char* mapName,
                                            f32 length) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setBottomGradationLength(
        length);
}

f32 getDepthShadowMapBottomGradationLength(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)
        ->tryFindDepthShadowMapInfo(mapName)
        ->getBottomGradationLength();
}

bool isAppendSubActorDepthShadowMap(const LiveActor* actor) {
    return getDepthShadowMapCtrl(actor)->isAppendSubActor();
}

f32 getShadowMaskDropLengthMax(const LiveActor* actor) {
    ShadowMaskCtrl* ctrl = getShadowMaskCtrl(actor);
    f32 maxDropLength = 0.0f;
    for (s32 i = 0; i < ctrl->getShadowMaskNum(); i++) {
        ShadowMaskBase* mask = ctrl->getShadowMask(i);
        if (maxDropLength < mask->getDropLength())
            maxDropLength = mask->getDropLength();
    }
    return maxDropLength;
}

void setShadowMaskColor(const LiveActor* actor, const char* maskName, const sead::Color4f& color) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        mask->setColor(color);
}

const sead::Color4f& getShadowMaskColor(const LiveActor* actor, const char* maskName) {
    return getShadowMaskCtrl(actor)->findShadowMask(maskName)->getColor();
}

void setShadowMaskIntensity(const LiveActor* actor, const char* maskName, f32 intensity) {
    ShadowMaskBase* mask = getShadowMaskCtrl(actor)->findShadowMask(maskName);
    if (mask)
        mask->setIntensity(intensity);
}

f32 getShadowMaskIntensity(const LiveActor* actor, const char* maskName) {
    return getShadowMaskCtrl(actor)->findShadowMask(maskName)->getIntensity();
}

f32 getShadowMaskTextureFixedScale(const LiveActor* actor, const char* maskName) {
    return getCubeMask(getShadowMaskCtrl(actor)->findShadowMask(maskName))->getTextureFixedScale();
}

void setShadowMaskTextureFixedScale(const LiveActor* actor, const char* maskName, f32 scale) {
    getCubeMask(getShadowMaskCtrl(actor)->findShadowMask(maskName))->setTextureFixedScale(scale);
}

const sead::Vector3f& getShadowMaskOffset(const LiveActor* actor, const char* maskName) {
    return getShadowMaskCtrl(actor)->findShadowMask(maskName)->getOffset();
}

void setShadowMaskOffset(const LiveActor* actor, const sead::Vector3f& offset,
                         const char* maskName) {
    getShadowMaskCtrl(actor)->findShadowMask(maskName)->setOffset(offset);
}

bool isExistOcclusionLightPosPtr(const LiveActor* actor, const char* occSphereName) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        return occSphere->isExistLightPosPtr();
    return false;
}

void setOcclusionLightPosPtr(const LiveActor* actor, const char* occSphereName,
                             const sead::Vector3f* lightPosPtr) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        occSphere->setLightPosPtr(lightPosPtr);
}

void setOcclusionAddOffset(const LiveActor* actor, const char* occSphereName,
                           const sead::Vector3f& offset) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        occSphere->setAddOffset(offset);
}

void enableOcclusion(const LiveActor* actor, const char* occSphereName) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        occSphere->setEnable(true);
}

void disableOcclusion(const LiveActor* actor, const char* occSphereName) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        occSphere->setEnable(false);
}

bool isEnableOcclusion(const LiveActor* actor, const char* occSphereName) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (occSphere)
        return occSphere->isEnable();
    return false;
}

void calcSphereDoBoundingInfo(sead::Vector3f* outDir, sead::Vector3f* lightDir, f32* outDistance,
                              f32* coneDegree, const LiveActor* actor, const char* occSphereName,
                              f32 scale) {
    ActorOcclusionKeeper* occlusionKeeper = actor->getActorOcclusionKeeper();
    if (!occlusionKeeper)
        return;

    OccSphere* occSphere = occlusionKeeper->getOccSphere(occSphereName);
    if (!occSphere)
        return;

    occlusionKeeper->calcDirectionalLight(lightDir);

    sead::Vector3f connectPos;
    occSphere->calcConnectPos(&connectPos);
    occSphere->tryCalcLightPosDir(lightDir, connectPos);

    f32 systemConeDegree = occlusionKeeper->calcSystemConeDegree();
    *coneDegree = systemConeDegree * scale;
    agl::sdw::calcSphereDoBoundingConeDegree(outDir, outDistance, *lightDir, connectPos,
                                             occSphere->getRadius(), occSphere->getDoRange(),
                                             *coneDegree);
    *outDistance *= occSphere->getBoundingDistanceScale();
}

void calcOcclusionSpherePos(sead::Vector3f* pos, const LiveActor* actor,
                            const char* occSphereName) {
    ActorOcclusionKeeper* occlusionKeeper = actor->getActorOcclusionKeeper();
    if (occlusionKeeper) {
        OccSphere* occSphere = occlusionKeeper->getOccSphere(occSphereName);
        if (occSphere)
            occSphere->calcConnectPos(pos);
    }
}

void changeOcclusionGroup(const LiveActor* actor, const char* occSphereName,
                          const char* groupName) {
    OccSphere* occSphere = actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
    if (!occSphere)
        return;

    if (groupName)
        occSphere->getGroupName().format(groupName);
    else
        occSphere->getGroupName() = sead::SafeString::cEmptyString;
}

OccSphere* getOcclusionSphere(const LiveActor* actor, const char* occSphereName) {
    return actor->getActorOcclusionKeeper()->getOccSphere(occSphereName);
}

void setOcclusionIgnoreHostHide(const LiveActor* actor, bool isIgnoreHostHide) {
    ActorOcclusionKeeper* occlusionKeeper = actor->getActorOcclusionKeeper();
    if (occlusionKeeper)
        occlusionKeeper->setIgnoreHostHide(isIgnoreHostHide);
}

bool findIsInShade(const LiveActor* actor, const sead::Vector3f& pos) {
    return actor->getSceneInfo()
        ->graphicsSystemInfo->getShadowDirector()
        ->getDepthShadowMapDirector()
        ->findIsInShade(pos);
}

void requestDepthShadowMapLightDir(const LiveActor* actor, const sead::Vector3f& lightDir,
                                   const char* mapName) {
    DepthShadowMapInfo* map = getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName);
    map->setRequestLightDir(true);
    map->setLightDir(lightDir);
}

void resetRequestDepthShadowMapLightDir(const LiveActor* actor, const char* mapName) {
    getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->setRequestLightDir(false);
}

bool isRequestDepthShadowMapLightDir(const LiveActor* actor, const char* mapName) {
    return getDepthShadowMapCtrl(actor)->tryFindDepthShadowMapInfo(mapName)->isRequestLightDir();
}

void onDepthShadowModel(LiveActor* actor) {
    actor->getModelKeeper()->getModelCtrl()->setDepthShadowModel(true);
    alActorSystemFunction::updateExecutorDraw(actor);
}

void offDepthShadowModel(LiveActor* actor) {
    actor->getModelKeeper()->getModelCtrl()->setDepthShadowModel(false);
    alActorSystemFunction::updateExecutorDraw(actor);
}

void updateDepthShadowMapCtrlShapeVisible(LiveActor* actor) {
    ShadowKeeper* shadowKeeper = actor->getShadowKeeper();
    if (shadowKeeper) {
        DepthShadowMapCtrl* depthShadowMapCtrl = shadowKeeper->getDepthShadowMapCtrl();
        if (depthShadowMapCtrl)
            depthShadowMapCtrl->updateShapeVisible(nullptr);
    }
}

}  // namespace al
