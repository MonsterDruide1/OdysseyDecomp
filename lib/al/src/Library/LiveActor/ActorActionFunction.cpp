#include "Library/LiveActor/ActorActionFunction.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Project/Action/ActionAnimCtrl.h"

namespace al {
void startAction(LiveActor* actor, const char* actionName) {
    ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();

    if (!actionKeeper || !actionKeeper->startAction(actionName)) {
        tryStartSklAnimIfExist(actor, actionName);
        tryStartMtpAnimIfExist(actor, actionName);
        tryStartMclAnimIfExist(actor, actionName);
        tryStartMtsAnimIfExist(actor, actionName);
        tryStartVisAnimIfExistForAction(actor, actionName);
    }
}
bool tryStartAction(LiveActor* actor, const char* actionName) {
    ActorActionKeeper* actionKeeper = actor->getActorActionKeeper();

    if ((!actionKeeper || !actionKeeper->getAnimCtrl() ||
         !actionKeeper->getAnimCtrl()->isExistAction(actionName))) {
        bool sklStarted = tryStartSklAnimIfExist(actor, actionName);
        bool mtpStarted = tryStartMtpAnimIfExist(actor, actionName);
        bool mclStarted = tryStartMclAnimIfExist(actor, actionName);
        bool mtsStarted = tryStartMtsAnimIfExist(actor, actionName);
        bool visStarted = tryStartVisAnimIfExistForAction(actor, actionName);

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
    actionKeeper->startAction(actionName);
    return true;
}
}  // namespace al
