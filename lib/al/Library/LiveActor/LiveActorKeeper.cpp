#include "Library/LiveActor/LiveActorKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorInfo.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/DepthShadowModel.h"
#include "Library/Obj/ModelDrawParts.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Obj/SilhouetteModel.h"
#include "Library/Obj/SimpleCircleShadowXZ.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

SubActorKeeper::SubActorKeeper(LiveActor* rootActor) {
    mRootActor = rootActor;
}

void SubActorKeeper::registerSubActor(LiveActor* subActor, u32 syncType) {
    mBuffer[mCurActorCount] = new SubActorInfo(subActor, static_cast<SubActorSync>(syncType));
    mCurActorCount++;
}

// NON_MATCHING: https://decomp.me/scratch/9V8dS
void SubActorKeeper::init(const ActorInitInfo& initInfo, const char* suffix, s32 maxSubActors) {
    sead::FixedSafeString<0x80> actorInitFileName;
    s32 creatorCount;
    const u8* modelResourceYaml;

    if (isExistModelResource(mRootActor) &&
        !tryGetActorInitFileName(&actorInitFileName, mRootActor, "InitSubActor", suffix))
        createFileNameBySuffix(&actorInitFileName, "InitSubActor", suffix);

    if (isExistModelResource(mRootActor) &&
        isExistModelResourceYaml(mRootActor, actorInitFileName.cstr(), nullptr)) {
        modelResourceYaml = getModelResourceYaml(mRootActor, actorInitFileName.cstr(), nullptr);
        ByamlIter modelResourceIter(modelResourceYaml);
        ByamlIter initInfoIter;
        if (modelResourceIter.tryGetIterByKey(&initInfoIter, "InitInfo")) {
            s32 addActorNum = 0;
            if (initInfoIter.tryGetIntByKey(&addActorNum, "AddActorNum"))
                maxSubActors += addActorNum;
        }
        ByamlIter creatorIter;
        if (modelResourceIter.tryGetIterByKey(&creatorIter, "CreatorList"))
            creatorCount = creatorIter.getSize();
        else
            creatorCount = 0;
    } else {
        modelResourceYaml = nullptr;
        creatorCount = 0;
    }

    s32 actorCount = maxSubActors + creatorCount;
    mMaxActorCount = actorCount;
    mBuffer = new SubActorInfo*[maxSubActors + creatorCount];

    if (actorCount >= 1 && mMaxActorCount >= 2)
        for (s32 i = 1; i < mMaxActorCount; ++i)
            mBuffer[i] = nullptr;

    if (modelResourceYaml && creatorCount > 0) {
        ByamlIter modelResourceIter(modelResourceYaml);
        ByamlIter creatorIter;
        modelResourceIter.tryGetIterByKey(&creatorIter, "CreatorList");

        // TODO: finish the logic for this, it seems like theres some heavy optimizations going on,
        // making it tough to figure out original logic
        for (s32 i = 0; i < creatorCount; ++i) {
            ByamlIter subActorIter;
            creatorIter.tryGetIterByIndex(&subActorIter, i);

            SubActorInfo* actorInfo = new SubActorInfo();
            mBuffer[i] = actorInfo;

            const char* actorObjectName = tryGetByamlKeyStringOrNULL(subActorIter, "ObjectName");
            const char* actorModelName = tryGetByamlKeyStringOrNULL(subActorIter, "ModelName");
            const char* actorSuffix =
                tryGetByamlKeyStringOrNULL(subActorIter, "InitFileSuffixName");
            const char* actorClassName = tryGetByamlKeyStringOrNULL(subActorIter, "ClassName");
            const char* actorCategoryName =
                tryGetByamlKeyStringOrNULL(subActorIter, "CategoryName");

            bool isAlive = false;
            bool isAliveResult = subActorIter.tryGetBoolByKey(&isAlive, "IsAlive");

            bool isUseHostPlacementInfo = true;
            subActorIter.tryGetBoolByKey(&isUseHostPlacementInfo, "IsUseHostPlacementInfo");

            bool isSyncAppear = false;
            bool isGotSyncAppear = tryGetByamlBool(&isSyncAppear, subActorIter, "IsSyncAppear");
            if (isSyncAppear)
                actorInfo->syncType |= SubActorSync::cAppear;

            bool isSyncHide = false;
            bool isGotSyncHide = tryGetByamlBool(&isSyncHide, subActorIter, "IsSyncHide");
            if (isSyncHide)
                actorInfo->syncType |= SubActorSync::cHide;

            if (tryGetByamlKeyBoolOrFalse(subActorIter, "IsSyncAlphaMask"))
                actorInfo->syncType |= SubActorSync::cAlphaMask;

            if (tryGetByamlKeyBoolOrFalse(subActorIter, "IsSyncClipping"))
                actorInfo->syncType |= SubActorSync::cClipping;

            bool isCalcDepthShadowLength = true;
            tryGetByamlBool(&isCalcDepthShadowLength, subActorIter, "IsCalcDepthShadowLength");

            if (actorClassName && !isEqualString(actorClassName, "LiveActor")) {
                if (isEqualString(actorClassName, "PartsModel")) {
                    const char* actorFixFileSuffixName =
                        tryGetByamlKeyStringOrNULL(subActorIter, "FixFileSuffixName");
                    PartsModel* partsModel = new PartsModel(actorObjectName);
                    partsModel->initPartsFixFileNoRegister(mRootActor, initInfo, actorModelName,
                                                           actorSuffix, actorFixFileSuffixName);
                    actorInfo->subActor = partsModel;

                    actorInfo->syncType =
                        (!isGotSyncAppear ? actorInfo->syncType | SubActorSync::cAppear :
                                            actorInfo->syncType) |
                        SubActorSync::cClipping;

                    if (isExistModel(partsModel)) {
                        actorInfo->syncType =
                            (!isGotSyncHide ? actorInfo->syncType | SubActorSync::cHide :
                                              actorInfo->syncType) |
                            SubActorSync::cAlphaMask;
                    }
                } else if (isEqualString(actorClassName, "BreakModel")) {
                    const char* actionName = tryGetByamlKeyStringOrNULL(subActorIter, "ActionName");
                    const char* jointName = tryGetByamlKeyStringOrNULL(subActorIter, "JointName");

                    sead::Matrix34f* jointMtxPtr =
                        jointName ? getJointMtxPtr(mRootActor, jointName) : nullptr;

                    if (!actionName)
                        actionName = "Break";

                    BreakModel* breakModel =
                        new BreakModel(mRootActor, actorObjectName, actorModelName, actorSuffix,
                                       jointMtxPtr, actionName);

                    initCreateActorNoPlacementInfo(breakModel, initInfo);
                    actorInfo->subActor = breakModel;
                } else if (isEqualString(actorClassName, "SilhouetteModel")) {
                    actorInfo->subActor =
                        new SilhouetteModel(mRootActor, initInfo, actorCategoryName);
                } else if (isEqualString(actorClassName, "DepthShadowModel")) {
                    actorInfo->subActor = new DepthShadowModel(
                        mRootActor, initInfo, actorCategoryName ? actorCategoryName : actorSuffix,
                        isCalcDepthShadowLength);

                    continue;
                } else if (isEqualString(actorClassName, "InvincibleModel")) {
                    actorInfo->subActor =
                        new ModelDrawParts("無敵モデル", mRootActor, initInfo, actorCategoryName);
                } else {
                    if (!isEqualString(actorClassName, "SimpleCircleShadowXZ")) {
                        if (isEqualString(actorClassName, "CollisionObj")) {
                            const char* collSuffixName =
                                tryGetByamlKeyStringOrNULL(subActorIter, "CollisionSuffixName");
                            const char* collName =
                                tryGetByamlKeyStringOrNULL(subActorIter, "CollisionName");
                            const char* sensorName =
                                tryGetByamlKeyStringOrNULL(subActorIter, "SensorName");
                            const char* fileSuffixName =
                                tryGetByamlKeyStringOrNULL(subActorIter, "InitFileSuffixName");
                            const char* jointName =
                                tryGetByamlKeyStringOrNULL(subActorIter, "JointName");

                            if (!collName)
                                collName = collSuffixName;

                            const char* newSensorName = sensorName ? sensorName : collSuffixName;

                            if (fileSuffixName)
                                collSuffixName = fileSuffixName;

                            auto* sensor = getHitSensor(mRootActor, newSensorName);

                            actorInfo->subActor = createCollisionObj(
                                mRootActor, initInfo, collName, sensor, jointName, collSuffixName);
                            if (actorObjectName)
                                actorInfo->subActor->setName(actorObjectName);

                            continue;
                        }

                        actorInfo->subActor = new LiveActor(actorObjectName);
                        initActorWithArchiveName(actorInfo->subActor, initInfo, actorModelName,
                                                 actorSuffix);

                        continue;
                    }

                    SimpleCircleShadowXZ* dropShadow = new SimpleCircleShadowXZ(actorObjectName);
                    dropShadow->initSimpleCircleShadow(mRootActor, initInfo, actorModelName,
                                                       actorSuffix);
                    actorInfo->subActor = dropShadow;
                }
            } else {
                actorInfo->subActor = new LiveActor(actorObjectName);

                if (isUseHostPlacementInfo) {
                    initActorWithArchiveName(actorInfo->subActor, initInfo, actorModelName,
                                             actorSuffix);
                } else {
                    initChildActorWithArchiveNameNoPlacementInfo(actorInfo->subActor, initInfo,
                                                                 actorModelName, actorSuffix);
                }
            }

            initActorModelForceCubeMap(actorInfo->subActor, initInfo);

            if (isAliveResult) {
                if (isAlive)
                    actorInfo->subActor->makeActorAlive();
                else
                    actorInfo->subActor->makeActorDead();
            }
            bool isShow = false;
            if (tryGetByamlBool(&isShow, subActorIter, "IsShow") && !isShow)
                hideModel(actorInfo->subActor);
        }
    }
}

SubActorKeeper* SubActorKeeper::create(LiveActor* rootActor) {
    return new SubActorKeeper(rootActor);
}

SubActorKeeper* SubActorKeeper::tryCreate(LiveActor* rootActor, const char* suffix,
                                          s32 maxSubActors) {
    sead::FixedSafeString<0x80> actorInitFileName;

    if (!isExistModelResource(rootActor) ||
        !tryGetActorInitFileName(&actorInitFileName, rootActor, "InitSubActor", suffix))
        return nullptr;

    if (maxSubActors <= 0) {
        if (!isExistModelResource(rootActor))
            return nullptr;

        if (!isExistModelResourceYaml(rootActor, actorInitFileName.cstr(), nullptr))
            return nullptr;
    }

    return new SubActorKeeper(rootActor);
}
}  // namespace al
