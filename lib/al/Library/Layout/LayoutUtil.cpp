#include "Library/Layout/LayoutUtil.h"

#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Layout/LayoutSystem.h"

namespace al {
void initLayoutInitInfo(LayoutInitInfo* info, const LayoutKit* kit, SceneObjHolder* sceneObjHolder,
                        const AudioDirector* audioDirector, const LayoutSystem* layoutSystem,
                        const MessageSystem* messageSystem, const GamePadSystem* gamePadSystem) {
    info->init(kit->getExecuteDirector(), kit->getEffectSystem()->getEffectSystemInfo(),
               sceneObjHolder, audioDirector, nullptr, layoutSystem, messageSystem, gamePadSystem,
               nullptr);
    info->setDrawContext(kit->getDrawContext());
    info->setDrawInfo(kit->getDrawInfo());
}

void executeUpdateList(LayoutKit* kit, const char* tableName, const char* listName) {
    kit->getExecuteDirector()->executeList(tableName, listName);
}

void executeUpdateEffect(LayoutKit* kit) {
    alEffectSystemFunction::updateEffect2D(kit->getEffectSystem());
}

void executeDraw(const LayoutKit* kit, const char* tableName) {
    kit->getExecuteDirector()->draw(tableName);
}

void executeDrawEffect(const LayoutKit* kit) {
    alEffectSystemFunction::drawEffect2D(kit->getEffectSystem(), nullptr);
}
}  // namespace al
