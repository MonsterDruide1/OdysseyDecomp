#include "Library/LiveActor/ActorActionFunction.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathRandomUtil.h"
#include "Project/Action/ActionAnimCtrl.h"
#include "Project/Action/ActionEffectCtrl.h"

namespace al {
void startAction(LiveActor* actor, const char* string) {
    ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();

    if (!actionKeeper || !actionKeeper->startAction(string)) {
        tryStartSklAnimIfExist(actor, string);
        tryStartMtpAnimIfExist(actor, string);
        tryStartMclAnimIfExist(actor, string);
        tryStartMtsAnimIfExist(actor, string);
        tryStartVisAnimIfExistForAction(actor, string);
    }
}
bool tryStartAction(LiveActor* actor, const char* string) {
    ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();

    if ((!actionKeeper || !actionKeeper->getAnimCtrl() ||
         !actionKeeper->getAnimCtrl()->isExistAction(string))) {
        bool sklStarted = tryStartSklAnimIfExist(actor, string);
        bool mtpStarted = tryStartMtpAnimIfExist(actor, string);
        bool mclStarted = tryStartMclAnimIfExist(actor, string);
        bool mtsStarted = tryStartMtsAnimIfExist(actor, string);
        bool visStarted = tryStartVisAnimIfExistForAction(actor, string);

        if ((!sklStarted && !mtpStarted && !mclStarted && !mtsStarted && !visStarted)) {
            return false;
        }
        actionKeeper = actor->getActorActionKeeper();
        if (!actionKeeper) {
            return true;
        }
    } else {
        actionKeeper = actor->getActorActionKeeper();
    }
    actionKeeper->startAction(string);
    return true;
}
}  // namespace al
