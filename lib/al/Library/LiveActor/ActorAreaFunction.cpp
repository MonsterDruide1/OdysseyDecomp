#include "Library/LiveActor/ActorAreaFunction.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjMtxConnecter.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/Area/ValidatorBase.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"

namespace al {

AreaObj* tryFindAreaObjPlayerOne(const PlayerHolder* holder, const char* name) {
    s32 playerNum = getPlayerNumMax(holder);

    AreaObj* selectedAreaObj = nullptr;
    for (s32 i = 0; i < playerNum; i++) {
        if (isPlayerDead(holder, i))
            continue;
        if (!isAreaTarget(getPlayerActor(holder, i)))
            continue;
        const sead::Vector3f& playerPos = getPlayerPos(holder, i);
        AreaObj* areaObj = tryFindAreaObj(getPlayerActor(holder, i), name, playerPos);
        if (areaObj &&
            (!selectedAreaObj || selectedAreaObj->getPriority() < areaObj->getPriority())) {
            selectedAreaObj = areaObj;
        }
    }

    return selectedAreaObj;
}

AreaObj* tryFindAreaObjPlayerAll(const PlayerHolder* holder, const char* name) {
    s32 playerNum = getPlayerNumMax(holder);
    if (playerNum == 0)
        return nullptr;

    AreaObjGroup* areaGroup = tryFindAreaObjGroup(getPlayerActor(holder, 0), name);
    if (!areaGroup)
        return nullptr;

    s32 groupSize = areaGroup->getSize();
    AreaObj* selectedAreaObj = nullptr;
    for (s32 i = 0; i < groupSize; i++) {
        AreaObj* areaObj = areaGroup->getAreaObj(i);
        AreaObj* currentAreaObject = nullptr;
        for (s32 e = 0; e < playerNum; e++) {
            if (isPlayerDead(holder, e))
                continue;

            const sead::Vector3f& playerPos = getPlayerPos(holder, e);
            currentAreaObject = areaObj;
            if (!areaObj->isInVolume(playerPos)) {
                currentAreaObject = nullptr;
                break;
            }
        }
        if (currentAreaObject) {
            if (!selectedAreaObj ||
                selectedAreaObj->getPriority() < currentAreaObject->getPriority()) {
                selectedAreaObj = currentAreaObject;
            }
        }
    }
    return selectedAreaObj;
}

AreaObj* createAreaObj(const ActorInitInfo& actorInitInfo, const char* name) {
    AreaInitInfo areaInitInfo;
    areaInitInfo.set(*actorInitInfo.placementInfo, actorInitInfo.stageSwitchDirector,
                     actorInitInfo.actorSceneInfo.sceneObjHolder);
    AreaObj* areaObj = new AreaObj(name);
    areaObj->init(areaInitInfo);
    return areaObj;
}

void initAreaInitInfo(AreaInitInfo* areaInitInfo, const ActorInitInfo& actorInitInfo) {
    areaInitInfo->set(*actorInitInfo.placementInfo, actorInitInfo.stageSwitchDirector,
                      actorInitInfo.actorSceneInfo.sceneObjHolder);
}

AreaObj* createLinkArea(const ActorInitInfo& initInfo, const char* name, const char* areaName) {
    const PlacementInfo& actorPlacementInfo = *initInfo.placementInfo;
    PlacementInfo placementInfo;
    getLinksInfoByIndex(&placementInfo, actorPlacementInfo, name, 0);

    AreaInitInfo areaInfo;
    initAreaInitInfo(&areaInfo, placementInfo, initInfo);

    AreaObj* areaObj = new AreaObj(areaName);
    areaObj->init(areaInfo);
    return areaObj;
}

void initAreaInitInfo(AreaInitInfo* areaInitInfo, const PlacementInfo& placementInfo,
                      const ActorInitInfo& actorInitInfo) {
    areaInitInfo->set(placementInfo, actorInitInfo.stageSwitchDirector,
                      actorInitInfo.actorSceneInfo.sceneObjHolder);
}

AreaObj* tryCreateLinkArea(const ActorInitInfo& initInfo, const char* name, const char* areaName) {
    s32 linkChildNum = calcLinkChildNum(initInfo, name);
    if (linkChildNum < 1)
        return nullptr;

    return createLinkArea(initInfo, name, areaName);
}

AreaObjGroup* createLinkAreaGroup(const ActorInitInfo& initInfo, const char* name,
                                  const char* groupName, const char* areaName) {
    s32 linkChildNum = calcLinkChildNum(initInfo, name);
    if (linkChildNum < 1)
        return nullptr;

    AreaObjGroup* areaGroup = new AreaObjGroup(groupName, 0);
    areaGroup->createBuffer(linkChildNum);
    const PlacementInfo& actorPlacementInfo = *initInfo.placementInfo;

    for (s32 i = 0; i < linkChildNum; i++) {
        PlacementInfo placementInfo;
        getLinksInfoByIndex(&placementInfo, actorPlacementInfo, name, i);

        AreaInitInfo areaInfo;
        initAreaInitInfo(&areaInfo, placementInfo, initInfo);

        AreaObj* areaObj = new AreaObj(areaName);
        areaObj->init(areaInfo);

        areaGroup->registerAreaObj(areaObj);
    }
    return areaGroup;
}

AreaObjGroup* createLinkAreaGroup(LiveActor* actor, const ActorInitInfo& initInfo, const char* name,
                                  const char* groupName, const char* areaName) {
    s32 linkChildNum = calcLinkChildNum(initInfo, name);
    if (linkChildNum < 1)
        return nullptr;

    AreaObjGroup* areaGroup = new AreaObjGroup(groupName, 0);
    areaGroup->createBuffer(linkChildNum);
    const PlacementInfo& actorPlacementInfo = *initInfo.placementInfo;

    for (s32 i = 0; i < linkChildNum; i++) {
        PlacementInfo placementInfo;
        getLinksInfoByIndex(&placementInfo, actorPlacementInfo, name, i);

        AreaInitInfo areaInfo;
        areaInfo.set(placementInfo, initInfo.stageSwitchDirector,
                     initInfo.actorSceneInfo.sceneObjHolder);

        AreaObj* areaObj = new AreaObj(areaName);
        areaObj->init(areaInfo);

        areaGroup->registerAreaObj(areaObj);
    }
    return areaGroup;
}

AreaObj* tryFindAreaObj(const LiveActor* actor, const char* name) {
    if (!isAreaTarget(actor))
        return nullptr;

    return tryFindAreaObj(actor, name, getTrans(actor));
}

bool isInAreaObj(const LiveActor* actor, const char* name) {
    if (!isAreaTarget(actor))
        return false;

    return isInAreaObj(actor, name, getTrans(actor));
}

bool isInDeathArea(const LiveActor* actor) {
    if (!isAreaTarget(actor))
        return false;

    return isInDeathArea(actor, getTrans(actor));
}

bool isInWaterArea(const LiveActor* actor) {
    if (!isAreaTarget(actor))
        return false;

    return isInWaterArea(actor, getTrans(actor));
}

bool isInPlayerControlOffArea(const LiveActor* actor) {
    if (!isAreaTarget(actor))
        return false;

    return isInPlayerControlOffArea(actor, getTrans(actor));
}

f32 calcWaterSinkDepth(const LiveActor* actor) {
    return calcWaterSinkDepth(actor, getTrans(actor));
}

void registerAreaHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                         const ActorInitInfo& initInfo) {
    areaObj->getAreaObjDirector()->getMtxConnecterHolder()->registerParentMtx(
        mtx, *initInfo.placementInfo, nullptr);
}

void registerAreaHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                         const ActorInitInfo& initInfo, const ValidatorBase& validator) {
    areaObj->getAreaObjDirector()->getMtxConnecterHolder()->registerParentMtx(
        mtx, *initInfo.placementInfo, validator.fnc_8());
}

void registerAreaHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo) {
    registerAreaHostMtx(actor, actor->getBaseMtx(), initInfo);
}

void registerAreaHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo,
                         const ValidatorBase& validator) {
    registerAreaHostMtx(actor, actor->getBaseMtx(), initInfo, validator);
}

void registerAreaSyncHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                             const ActorInitInfo& initInfo) {
    areaObj->getAreaObjDirector()->getMtxConnecterHolder()->registerSyncParentMtx(
        mtx, *initInfo.placementInfo, nullptr);
}

void registerAreaSyncHostMtx(const IUseAreaObj* areaObj, const sead::Matrix34f* mtx,
                             const ActorInitInfo& initInfo, const ValidatorBase& validator) {
    areaObj->getAreaObjDirector()->getMtxConnecterHolder()->registerSyncParentMtx(
        mtx, *initInfo.placementInfo, validator.fnc_8());
}

void registerAreaSyncHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo) {
    registerAreaSyncHostMtx(actor, actor->getBaseMtx(), initInfo);
}

void registerAreaSyncHostMtx(const LiveActor* actor, const ActorInitInfo& initInfo,
                             const ValidatorBase& validator) {
    registerAreaSyncHostMtx(actor, actor->getBaseMtx(), initInfo, validator);
}

bool tryReviseVelocityInsideAreaObj(sead::Vector3f* velocity, LiveActor* actor,
                                    AreaObjGroup* areaGroup, const AreaObj* areaObj) {
    if (!areaGroup || !areaObj)
        return false;

    if (isInAreaObj(areaGroup, getTrans(actor))) {
        f32 velMagnitude = getVelocity(actor).length();
        sead::Vector3f nextPosition = getTrans(actor) + getVelocity(actor);
        if (isInAreaObj(areaGroup, nextPosition))
            return false;
        calcNearestAreaObjEdgePos(velocity, areaObj, nextPosition);
        sead::Vector3f newVelocity = *velocity - getTrans(actor);
        if (newVelocity.length() > velMagnitude)
            setLength(&newVelocity, velMagnitude);
        setVelocity(actor, newVelocity);
    } else {
        calcNearestAreaObjEdgePos(velocity, areaObj, getTrans(actor));
        setTrans(actor, *velocity);
        setVelocityZero(actor);
    }
    return true;
}

SwitchKeepOnAreaGroup* tryCreateSwitchKeepOnAreaGroup(LiveActor* actor,
                                                      const ActorInitInfo& initInfo) {
    AreaObjGroup* areaGroup =
        createLinkAreaGroup(actor, initInfo, "AreaSwitchKeepOn", "子供スイッチキープエリアグループ",
                            "子供スイッチキープエリア");
    if (!areaGroup)
        return nullptr;

    return new SwitchKeepOnAreaGroup(areaGroup);
}

SwitchOnAreaGroup* tryCreateSwitchOnAreaGroup(LiveActor* actor, const ActorInitInfo& initInfo) {
    AreaObjGroup* areaGroup = createLinkAreaGroup(
        actor, initInfo, "AreaSwitchOn", "子供スイッチOnエリアグループ", "子供スイッチOnエリア");
    if (!areaGroup)
        return nullptr;

    return new SwitchOnAreaGroup(areaGroup);
}

}  // namespace al
