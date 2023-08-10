#include "al/Library/Obj/BreakModel.h"
#include "al/Library/LiveActor/ActorActionFunction.h"
#include "al/Library/LiveActor/ActorClippingFunction.h"
#include "al/Library/LiveActor/ActorInitInfo.h"
#include "al/Library/LiveActor/ActorModelFunction.h"
#include "al/Library/LiveActor/ActorPoseKeeper.h"
#include "al/Library/LiveActor/ActorResourceFunction.h"
#include "al/Library/Math/MathUtil.h"
#include "al/Library/Nerve/NerveSetupUtil.h"
#include "al/Library/Yaml/ByamlIter.h"
#include "al/Library/Yaml/ByamlUtil.h"

namespace {
MAKE_NERVE_NAMESPACE(BreakModel, Wait, al);
MAKE_NERVE_NAMESPACE(BreakModel, Break, al);
}  // namespace

namespace al {
BreakModel::BreakModel(LiveActor const* rootActor, const char* objName, const char* modelName,
                       const char* fileSuffixName, const sead::Matrix34f* rootMtx,
                       const char* breakActionName)
    : LiveActor(objName), mInitSuffix(fileSuffixName), mParent(rootActor), mRootMtx(rootMtx),
      mModelName(modelName), mBreakAction(breakActionName) {}

void BreakModel::init(const ActorInitInfo& initInfo) {
    initActorWithArchiveName(this, initInfo, mModelName, mInitSuffix);
    initNerve(this, &BreakModelNrvWait::sInstance, 0);
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

void BreakModel::appear(void) {
    if (mRootMtx)
        updatePoseMtx(this, mRootMtx);
    else
        copyPose(this, mParent);

    if (mBreakAction) {
        startAction(this, mBreakAction);
        if (!isNearZero(1.0f - mActionFrameRate, 0.001f))
            setActionFrameRate(this, mActionFrameRate);
    }
    setNerve(this, &BreakModelNrvBreak::sInstance);

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
