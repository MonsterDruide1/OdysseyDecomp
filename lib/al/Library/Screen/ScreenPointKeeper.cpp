#include "Library/Screen/ScreenPointKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Resource/Resource.h"
#include "Library/Screen/ScreenPointDirector.h"
#include "Library/Screen/ScreenPointTarget.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterBase.h"

namespace al {

bool ScreenPointKeeper::isExistFile(const Resource* resource, const char* fileName) {
    StringTmp<64> path("%s%s.byml", "InitScreenPoint", fileName ?: "");
    return resource->isExistFile(path.cstr());
}

ScreenPointKeeper::ScreenPointKeeper() {
    mParameterIo = new ParameterIo();
    mTargets = new ParameterArray();
    mOptions = new ParameterObj();

    mAddTargetNum =
        new ParameterS32("AddTargetNum", "AddTargetNum", "Min=0, Max=10", mOptions, true);

    mParameterIo->addObj(mOptions, "Options");
    mParameterIo->addArray(mTargets, "Targets");
}

void ScreenPointKeeper::initByYaml(LiveActor* actor, const Resource* resource,
                                   const ActorInitInfo& initInfo, const char* name) {
    initParameterIoAsActorInfo(mParameterIo, actor, "InitScreenPoint", name);
    ByamlIter iter;
    if (!tryGetActorInitFileIter(&iter, resource, "InitScreenPoint", name))
        return;

    mParameterIo->tryGetParam(iter);
    s32 size = mTargets->getSize();
    if (size == 0)
        return;

    initArray(mAddTargetNum->getValue() + size);

    for (s32 i = 0; i < size; i++)
        addTarget(actor, initInfo, "Tmp", 0.0f, getTransPtr(actor), nullptr, sead::Vector3f::zero);

    mParameterIo->tryGetParam(iter);
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->setFollowMtxPtrByJointName(actor);
}

void ScreenPointKeeper::initArray(s32 size) {
    mScreenPointTargets.allocBuffer(size, nullptr);
}

ScreenPointTarget* ScreenPointKeeper::addTarget(LiveActor* actor, const ActorInitInfo& initInfo,
                                                const char* targetName, f32 radius,
                                                const sead::Vector3f* va, const char* jointName,
                                                const sead::Vector3f& vb) {
    ScreenPointTarget* target = new ScreenPointTarget(actor, targetName, radius, va, jointName, vb);
    mScreenPointTargets.pushBack(target);
    mTargets->addObj(target->getParameterObj());

    ScreenPointDirector* director = initInfo.screenPointDirector;
    director->registerTarget(target);
    director->setCheckGroup(target);
    target->setFollowMtxPtrByJointName(actor);
    return target;
}

void ScreenPointKeeper::update() {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->update();
}

void ScreenPointKeeper::validate() {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->validate();
}

const ScreenPointTarget* ScreenPointKeeper::getTarget(s32 index) const {
    return mScreenPointTargets[index];
}

void ScreenPointKeeper::invalidate() {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->invalidate();
}

void ScreenPointKeeper::validateBySystem() {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->validateBySystem();
}

void ScreenPointKeeper::invalidateBySystem() {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        mScreenPointTargets[i]->invalidateBySystem();
}

const ScreenPointTarget* ScreenPointKeeper::getTarget(const char* targetName) const {
    if (mScreenPointTargets.size() == 1)
        return mScreenPointTargets[0];

    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        if (isEqualString(mScreenPointTargets[i]->getTargetName(), targetName))
            return mScreenPointTargets[i];
    return nullptr;
}

bool ScreenPointKeeper::isExistTarget(const char* targetName) const {
    s32 size = mScreenPointTargets.size();
    for (s32 i = 0; i < size; i++)
        if (isEqualString(mScreenPointTargets[i]->getTargetName(), targetName))
            return true;
    return false;
}

}  // namespace al
