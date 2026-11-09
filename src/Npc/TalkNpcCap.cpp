#include "Npc/TalkNpcCap.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

void TalkNpcCap::initAttach(const al::LiveActor* actor) {
    _110 = actor->getBaseMtx();
    if (al::isExistModelResourceYaml(actor, "InitPartsFixInfo", mTalkNpcParam->mModelName)) {
        al::ByamlIter iter(
            al::getModelResourceYaml(actor, "InitPartsFixInfo", mTalkNpcParam->mModelName));
        const char* jointName = nullptr;
        if (al::tryGetByamlString(&jointName, iter, "JointName") && jointName)
            _110 = al::getJointMtxPtr(actor, jointName);
        al::tryGetByamlV3f(&_118, iter, "LocalRotate");
        al::tryGetByamlV3f(&_124, iter, "LocalTrans");
        sead::Vector3f scale(1.0f, 1.0f, 1.0f);
        al::tryGetByamlV3f(&scale, iter, "LocalScale");
        _130 = scale.x;
    }
    makeActorAlive();
}
