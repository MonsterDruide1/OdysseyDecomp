#include "Amiibo/HelpAmiiboPlayerInvicible.h"

#include "Library/Nfp/NfpFunction.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Player/PlayerFunction.h"

HelpAmiiboPlayerInvincible::HelpAmiiboPlayerInvincible(HelpAmiiboDirector* director,
                                                       al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "プレイヤー無敵お助け") {}

bool HelpAmiiboPlayerInvincible::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseMario(nfpInfo) || al::isCharacterIdBaseDrMario(nfpInfo);
}

bool HelpAmiiboPlayerInvincible::isEnableUse() {
    return PlayerFunction::tryActivateAmiiboPreventDamage(getActor());
}

bool HelpAmiiboPlayerInvincible::execute() {
    al::startSe(getDirector(), "AmiiboMario");
    return true;
}

HelpAmiiboType HelpAmiiboPlayerInvincible::getType() const {
    return HelpAmiiboType::Mario;
}
