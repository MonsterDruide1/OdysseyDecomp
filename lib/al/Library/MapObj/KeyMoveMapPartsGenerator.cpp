#include "Library/MapObj/KeyMoveMapPartsGenerator.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/MapObj/KeyMoveMapParts.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_IMPL(KeyMoveMapPartsGenerator, Generate)
NERVE_IMPL(KeyMoveMapPartsGenerator, Delay)

NERVES_MAKE_STRUCT(KeyMoveMapPartsGenerator, Generate, Delay)
}  // namespace

namespace al {
KeyMoveMapPartsGenerator::KeyMoveMapPartsGenerator(const char* name) : LiveActor(name) {}

void KeyMoveMapPartsGenerator::init(const ActorInitInfo& info) {
    initActorSceneInfo(this, info);
    initExecutorMapObjMovement(this, info);
    initActorPoseTQSV(this);

    s32 partsCount = 4;
    tryGetArg(&partsCount, info, "PartsCount");
    tryGetArg(&mGenerateInterval, info, "GenerateInterval");
    tryGetArg(&mDelayTime, info, "DelayTime");

    initSubActorKeeperNoFile(this, info, partsCount);

    mKeyMoveMapPartsGroup =
        new DeriveActorGroup<KeyMoveMapParts>("キー移動マップパーツリスト", partsCount);

    if (calcLinkChildNum(info, "Generate") == 0) {
        makeActorDead();

        return;
    }

    for (s32 i = 0; i < partsCount; i++) {
        KeyMoveMapParts* keyMoveMapParts = new KeyMoveMapParts("キー移動マップマップパーツ");

        initLinksActor(keyMoveMapParts, info, "Generate", 0);

        keyMoveMapParts->setIsStopKill(true);
        keyMoveMapParts->makeActorDead();

        mKeyMoveMapPartsGroup->registerActor(keyMoveMapParts);
    }

    KeyMoveMapParts* keyMoveMapParts = mKeyMoveMapPartsGroup->getDeriveActor(0);
    f32 clippingRadius = 0.0f;
    calcKeyMoveClippingInfo(&mClippingTrans, &clippingRadius, keyMoveMapParts->getKeyPoseKeeper(),
                            500.0f);
    initActorClipping(this, info);
    setClippingInfo(this, clippingRadius, &mClippingTrans);
    initGroupClipping(this, info);

    initNerve(this, &NrvKeyMoveMapPartsGenerator.Generate, 0);
    if (mDelayTime > 0)
        setNerve(this, &NrvKeyMoveMapPartsGenerator.Delay);
    getNerveKeeper()->update();

    initStageSwitch(this, info);

    makeActorAlive();
}

void KeyMoveMapPartsGenerator::exeDelay() {
    if (isGreaterEqualStep(this, mDelayTime - 1))
        setNerve(this, &NrvKeyMoveMapPartsGenerator.Generate);
}

void KeyMoveMapPartsGenerator::exeGenerate() {
    if (isIntervalStep(this, mGenerateInterval, 0)) {
        KeyMoveMapParts* keyMoveMapParts = mKeyMoveMapPartsGroup->tryFindDeadDeriveActor();
        if (keyMoveMapParts != nullptr)
            keyMoveMapParts->appearAndSetStart();
    }
}
}  // namespace al
