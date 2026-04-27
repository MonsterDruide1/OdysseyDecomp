#include "MapObj/CapMessageDirector.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Layout/CapMessageLayout.h"
#include "MapObj/CapMessageGameDataChecker.h"
#include "MapObj/CapMessageKeeper.h"
#include "MapObj/CapMessagePlacement.h"
#include "MapObj/CapMessageShowInfo.h"

namespace {
NERVE_IMPL(CapMessageDirector, ShowSystem)
NERVE_IMPL(CapMessageDirector, Wait)
NERVE_IMPL(CapMessageDirector, DelayPlacement)
NERVE_IMPL(CapMessageDirector, ShowPlacement)
NERVE_IMPL(CapMessageDirector, ShowSystemLow)
NERVE_IMPL(CapMessageDirector, ShowSystemContinue)
NERVE_IMPL(CapMessageDirector, End)

struct CapMessageDirectorNrvGroup {
    CapMessageDirectorNrvWait Wait;
    CapMessageDirectorNrvDelayPlacement DelayPlacement;
    CapMessageDirectorNrvShowPlacement ShowPlacement;
    CapMessageDirectorNrvShowSystemLow ShowSystemLow;
    CapMessageDirectorNrvShowSystemContinue ShowSystemContinue;
    CapMessageDirectorNrvEnd End;
};

CapMessageDirectorNrvShowSystem sCapMessageDirectorNrvShowSystem;
CapMessageDirectorNrvGroup sCapMessageDirectorNrvGroup;

CapMessageGameDataChecker sDefaultCapMessageGameDataChecker;
}  // namespace

CapMessageDirector::CapMessageDirector()
    : al::LiveActor("帽子メッセージディレクター(新)"), mLayout(nullptr), mPlacementGroup(nullptr),
      mCurrentPlacement(nullptr), mMessageKeeper(nullptr), mIsAppearCapMessageValid(true) {
    mPlacementGroup =
        new al::DeriveActorGroup<CapMessagePlacement>("配置帽子メッセージグループ", 32);
}

void CapMessageDirector::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "監視オブジェ");
    al::initNerve(this, &sCapMessageDirectorNrvGroup.Wait, 0);

    mLayout = new CapMessageLayout("帽子メッセージ レイアウト", "TalkMessageInfo",
                                   al::getLayoutInitInfo(info), nullptr, false);
    mMessageKeeper = new CapMessageKeeper();

    makeActorAlive();
}

void CapMessageDirector::exeWait() {
    al::isFirstStep(this);
    tryStartShowPlacement();
}

bool CapMessageDirector::tryStartShowPlacement() {
    if (!mIsAppearCapMessageValid)
        return false;

    if (mPlacementGroup->getActorCount() < 1) {
        mCurrentPlacement = nullptr;
        return false;
    }

    CapMessagePlacement* bestPlacement = nullptr;
    s32 bestPriority = -1;

    for (s32 i = 0; i < mPlacementGroup->getActorCount(); i++) {
        CapMessagePlacement* placement = mPlacementGroup->getDeriveActor(i);

        if (placement->getAppearPriority() > bestPriority && placement->isActive() &&
            placement->checkEnbale()) {
            bestPriority = placement->getAppearPriority();
            bestPlacement = placement;
        }
    }

    mCurrentPlacement = bestPlacement;
    if (bestPlacement == nullptr)
        return false;

    if (bestPlacement->getDelayTime() > 0)
        al::setNerve(this, &sCapMessageDirectorNrvGroup.DelayPlacement);
    else
        al::setNerve(this, &sCapMessageDirectorNrvGroup.ShowPlacement);

    return true;
}

void CapMessageDirector::exeDelayPlacement() {
    al::isFirstStep(this);

    if (!mCurrentPlacement->isActive()) {
        mCurrentPlacement = nullptr;
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
        return;
    }

    CapMessagePlacement* bestPlacement = nullptr;
    if (mPlacementGroup->getActorCount() >= 1) {
        s32 bestPriority = -1;

        for (s32 i = 0; i < mPlacementGroup->getActorCount(); i++) {
            CapMessagePlacement* placement = mPlacementGroup->getDeriveActor(i);

            if (placement->getAppearPriority() > bestPriority && placement->isActive() &&
                placement->checkEnbale()) {
                bestPriority = placement->getAppearPriority();
                bestPlacement = placement;
            }
        }
    }

    if (bestPlacement == nullptr && mCurrentPlacement->isDelayCancel()) {
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
        return;
    }

    if (bestPlacement != nullptr && mCurrentPlacement != bestPlacement) {
        mCurrentPlacement = bestPlacement;
        if (bestPlacement->getDelayTime() > 0)
            al::setNerve(this, &sCapMessageDirectorNrvGroup.DelayPlacement);
        else
            al::setNerve(this, &sCapMessageDirectorNrvGroup.ShowPlacement);
        return;
    }

    if (al::isGreaterEqualStep(this, mCurrentPlacement->getDelayTime()))
        al::setNerve(this, &sCapMessageDirectorNrvGroup.ShowPlacement);
}

CapMessagePlacement* CapMessageDirector::findPlacement() const {
    if (mPlacementGroup->getActorCount() < 1)
        return nullptr;

    CapMessagePlacement* bestPlacement = nullptr;
    s32 bestPriority = -1;

    for (s32 i = 0; i < mPlacementGroup->getActorCount(); i++) {
        CapMessagePlacement* placement = mPlacementGroup->getDeriveActor(i);

        if (placement->getAppearPriority() > bestPriority && placement->isActive() &&
            placement->checkEnbale()) {
            bestPriority = placement->getAppearPriority();
            bestPlacement = placement;
        }
    }

    return bestPlacement;
}

void CapMessageDirector::exeShowPlacement() {
    if (al::isFirstStep(this)) {
        mLayout->start(mCurrentPlacement->getShowInfo());
        mCurrentPlacement->showStart();
    }

    if (!mCurrentPlacement->isActive()) {
        mCurrentPlacement->showEnd();
        mLayout->tryEnd();
        al::setNerve(this, &sCapMessageDirectorNrvGroup.End);
        return;
    }

    bool isEnable = mCurrentPlacement->checkEnbale();
    mLayout->setContinueShow(isEnable && mCurrentPlacement->isKeepDisp());

    if (!mLayout->isAlive()) {
        mCurrentPlacement->showEnd();
        if (!isEnable)
            al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
    }
}

void CapMessageDirector::forceEndInner() {
    mLayout->tryEnd();
    al::setNerve(this, &sCapMessageDirectorNrvGroup.End);
}

void CapMessageDirector::exeShowSystemLow() {
    if (!tryStartShowPlacement() && !mLayout->isAlive())
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
}

void CapMessageDirector::exeShowSystem() {
    if (!mLayout->isAlive())
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
}

void CapMessageDirector::exeShowSystemContinue() {
    if (!mLayout->isAlive())
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
}

void CapMessageDirector::exeEnd() {
    if (!mLayout->isAlive())
        al::setNerve(this, &sCapMessageDirectorNrvGroup.Wait);
}

bool CapMessageDirector::isShow(const char* labelName) const {
    return mLayout->isShow(labelName);
}

bool CapMessageDirector::isDelay(const char* labelName) const {
    if (!al::isNerve(this, &sCapMessageDirectorNrvGroup.DelayPlacement))
        return mLayout->isDelay(labelName);

    if (labelName == nullptr)
        return true;

    if (mCurrentPlacement != nullptr)
        return al::isEqualString(labelName, mCurrentPlacement->getShowInfo()->getLabelName());

    return mLayout->isDelay(labelName);
}

bool CapMessageDirector::isActive(const char* labelName) const {
    if (mLayout->isShow(labelName))
        return true;

    if (al::isNerve(this, &sCapMessageDirectorNrvGroup.DelayPlacement)) {
        if (labelName == nullptr)
            return true;

        if (mCurrentPlacement != nullptr)
            return al::isEqualString(labelName, mCurrentPlacement->getShowInfo()->getLabelName());
    }

    return mLayout->isDelay(labelName);
}

s32 CapMessageDirector::registerCapMessagePlacement(CapMessagePlacement* placement) {
    return mPlacementGroup->registerActor(placement);
}

bool CapMessageDirector::tryShowMessageSystem(const CapMessageShowInfo* showInfo,
                                              const CapMessageGameDataChecker* checker) {
    if (checker != nullptr) {
        if (!checker->check(this))
            return false;
    } else if (!sDefaultCapMessageGameDataChecker.check(this)) {
        return false;
    }

    mLayout->start(showInfo);
    al::setNerve(this, &sCapMessageDirectorNrvShowSystem);
    return true;
}

bool CapMessageDirector::tryCheck(const CapMessageGameDataChecker* checker) const {
    if (checker != nullptr)
        return checker->check(this);

    return sDefaultCapMessageGameDataChecker.check(this);
}

bool CapMessageDirector::tryShowMessageSystemLow(const CapMessageShowInfo* showInfo,
                                                 const CapMessageGameDataChecker* checker) {
    if (checker != nullptr) {
        if (!checker->check(this))
            return false;
    } else if (!sDefaultCapMessageGameDataChecker.check(this)) {
        return false;
    }

    if (!al::isNerve(this, &sCapMessageDirectorNrvGroup.Wait))
        return false;

    mLayout->start(showInfo);
    al::setNerve(this, &sCapMessageDirectorNrvGroup.ShowSystemLow);
    return true;
}

bool CapMessageDirector::tryShowMessageSystemContinue(const CapMessageShowInfo* showInfo,
                                                      const CapMessageGameDataChecker* checker) {
    if (checker != nullptr) {
        if (!checker->check(this))
            return false;
    } else if (!sDefaultCapMessageGameDataChecker.check(this)) {
        return false;
    }

    mLayout->startContinue(showInfo);
    al::setNerve(this, &sCapMessageDirectorNrvGroup.ShowSystemContinue);
    return true;
}

void CapMessageDirector::endCapMessageSystemContinue() {
    if (al::isNerve(this, &sCapMessageDirectorNrvGroup.ShowSystemContinue)) {
        mLayout->tryEnd();
        al::setNerve(this, &sCapMessageDirectorNrvGroup.End);
    }
}

void CapMessageDirector::invalidateAppearCapMessage() {
    if (mLayout->tryEndExistContinueDemo()) {
        if (al::isNerve(this, &sCapMessageDirectorNrvGroup.ShowPlacement) &&
            mCurrentPlacement != nullptr) {
            mCurrentPlacement->showEnd();
        }

        al::setNerve(this, &sCapMessageDirectorNrvGroup.End);
    }

    mIsAppearCapMessageValid = false;
}

void CapMessageDirector::validateAppearCapMessage() {
    mIsAppearCapMessageValid = true;
}

void CapMessageDirector::forceEnd() {
    mLayout->tryEnd();
    al::setNerve(this, &sCapMessageDirectorNrvGroup.End);
}
