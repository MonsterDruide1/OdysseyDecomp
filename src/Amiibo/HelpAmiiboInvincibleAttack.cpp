#include "Amiibo/HelpAmiiboInvincibleAttack.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Amiibo/ItemAmiiboKoopa.h"

HelpAmiiboInvincibleAttack::HelpAmiiboInvincibleAttack(HelpAmiiboDirector* director,
                                                       al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "無敵攻撃お助け") {}

void HelpAmiiboInvincibleAttack::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mItemAmiiboKoopa = new ItemAmiiboKoopa("クッパamiibo");
    al::initCreateActorNoPlacementInfo(mItemAmiiboKoopa, initInfo);
}

bool HelpAmiiboInvincibleAttack::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseKoopa(nfpInfo);
}

void HelpAmiiboInvincibleAttack::activate() {
    HelpAmiiboExecutor::activate();
    al::resetPosition(mItemAmiiboKoopa, al::getTrans(getActor()));
    mItemAmiiboKoopa->appear();
    al::startSe(getDirector(), "AmiiboKoopa");
}

bool HelpAmiiboInvincibleAttack::execute() {
    return al::isDead(mItemAmiiboKoopa);
}

HelpAmiiboType HelpAmiiboInvincibleAttack::getType() const {
    return HelpAmiiboType::Koopa;
}
