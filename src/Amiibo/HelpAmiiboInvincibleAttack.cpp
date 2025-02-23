#include "Amiibo/HelpAmiiboInvincibleAttack.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Amiibo/ItemAmiiboKoopa.h"

HelpAmiiboInvincibleAttack::HelpAmiiboInvincibleAttack(HelpAmiiboDirector* director,
                                                       al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "無敵攻撃お助け") {}

void HelpAmiiboInvincibleAttack::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mActor = new ItemAmiiboKoopa("クッパamiibo");
    al::initCreateActorNoPlacementInfo(mActor, initInfo);
}

bool HelpAmiiboInvincibleAttack::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseKoopa(nfpInfo);
}

void HelpAmiiboInvincibleAttack::activate() {
    HelpAmiiboExecutor::activate();
    al::resetPosition(mActor, al::getTrans(getActor()));
    mActor->appear();
    al::startSe(getDirector(), "AmiiboKoopa");
}

bool HelpAmiiboInvincibleAttack::execute() {
    return al::isDead(mActor);
}

HelpAmiiboType HelpAmiiboInvincibleAttack::getType() const {
    return HelpAmiiboType::Koopa;
}
