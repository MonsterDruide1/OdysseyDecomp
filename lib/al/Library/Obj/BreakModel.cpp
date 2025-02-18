#include "Library/Obj/BreakModel.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
using namespace al;
NERVE_IMPL(BreakModel, Wait);
NERVE_IMPL(BreakModel, Break);

NERVES_MAKE_NOSTRUCT(BreakModel, Wait, Break);
}  // namespace

namespace al {
BreakModel::BreakModel(const LiveActor* rootActor, const char* objName, const char* modelName,
                       const char* fileSuffixName, const sead::Matrix34f* rootMtx,
                       const char* breakActionName)
    : LiveActor(objName), mInitSuffix(fileSuffixName), mParent(rootActor), mRootMtx(rootMtx),
      mModelName(modelName), mBreakAction(breakActionName) {}

void BreakModel::init(const ActorInitInfo& initInfo) {
    initActorWithArchiveName(this, initInfo, mModelName, mInitSuffix);
    initNerve(this, &Wait, 0);
    invalidateClipping(this);

    ByamlIter actorInitFile;

    if (tryGetActorInitFileIter(&actorInitFile, this, "InitBreakModel", mInitSuffix)) {
        ByamlIter dither;
        if (actorInitFile.tryGetIterByKey(&dither, "DitherDisappear")) {
            mIsValid = tryGetByamlKeyBoolOrFalse(dither, "IsValid");
            if (mIsValid)
                mStep = getByamlKeyInt(dither, "Step");
        }
    }

    makeActorDead();
}

void BreakModel::appear() {
    if (mRootMtx)
        updatePoseMtx(this, mRootMtx);
    else
        copyPose(this, mParent);

    if (mBreakAction) {
        startAction(this, mBreakAction);
        if (!isNearZero(1.0f - mActionFrameRate, 0.001f))
            setActionFrameRate(this, mActionFrameRate);
    }
    setNerve(this, &Break);

    LiveActor::makeActorAlive();
}

void BreakModel::exeWait() {}

void BreakModel::exeBreak() {
    if (mIsValid) {
        if (mBreakAction) {
            f32 ditherStart = getActionFrameMax(this, mBreakAction) < mStep ?
                                  0.0f :
                                  getActionFrameMax(this, mBreakAction) - mStep;

            setModelAlphaMask(this, 1.0f - normalize((f32)getNerveStep(this), ditherStart,
                                                     getActionFrameMax(this, mBreakAction)));
        }
    }

    if (isActionEnd(this))
        kill();
}

}  // namespace al
