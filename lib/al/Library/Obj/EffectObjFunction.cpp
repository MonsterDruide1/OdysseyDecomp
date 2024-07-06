#include "Library/Obj/EffectObjFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
void EffectObjFunction::initActorEffectObj(LiveActor* actor, const ActorInitInfo& info) {
    const char* model = nullptr;
    const char* tmp;

    if (!alPlacementFunction::tryGetModelName(&model, info))
        getObjectName(&model, info);

    tmp = model;

    if (isExistArchive(StringTmp<128>("ObjectData/%s", model)))
        initActorWithArchiveName(actor, info, tmp, nullptr);
    else
        FUN_7100969078(actor, info);

    initActorEffectKeeper(actor, info, tmp);
}

void EffectObjFunction::initActorEffectObj(LiveActor* actor, const ActorInitInfo& info,
                                           const char* archiveName) {
    if (isExistArchive(StringTmp<128>("ObjectData/%s")))
        initActorWithArchiveName(actor, info, archiveName, nullptr);
    else
        FUN_7100969078(actor, info);

    initActorEffectKeeper(actor, info, archiveName);
}

void EffectObjFunction::FUN_7100969078(LiveActor* actor, const ActorInitInfo& info) {
    initActorSceneInfo(actor, info);
    initActorPoseTQSV(actor);
    initActorSRT(actor, info);
    initActorClipping(actor, info);
    initGroupClipping(actor, info);
    setClippingInfo(actor, 1000.0f, nullptr);
    initStageSwitch(actor, info);
    initExecutorUpdate(actor, info, "エフェクトオブジェ");

    if (isExistRail(info, "Rail"))
        actor->initRailKeeper(info, "Rail");
}

void EffectObjFunction::initActorEffectObjNoArchive(LiveActor* actor, const ActorInitInfo& info,
                                                    const char* archiveName) {
    FUN_7100969078(actor, info);
    initActorEffectKeeper(actor, info, archiveName);
}
}  // namespace al
