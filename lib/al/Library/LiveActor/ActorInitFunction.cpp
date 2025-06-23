#include "Library/LiveActor/ActorInitFunction.h"

#include "ActorModelFunction.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Model/ModelCtrl.h"
#include "Library/Model/ModelKeeper.h"
#include "nn/g3d/ModelObj.h"

namespace al {
void initActorSceneInfo(LiveActor* actor, const ActorInitInfo& info) {
    auto* sceneInfo = new ActorSceneInfo();
    *sceneInfo = info.actorSceneInfo;
    actor->initSceneInfo(sceneInfo);
    info.allActorsGroup->registerActor(actor);
}

void initExecutorUpdate(LiveActor* actor, const ActorInitInfo& info, const char* listName) {
    registerExecutorActorUpdate(actor, info.executeDirector, listName);
}

void initExecutorDraw(LiveActor* actor, const ActorInitInfo& info, const char* listName) {
    registerExecutorActorDraw(actor, info.executeDirector, listName);
}

#define REGISTER_UPDATE(LIST_NAME)                                                                 \
    {                                                                                              \
        initExecutorUpdate(actor, info, LIST_NAME);                                                \
    }
#define REGISTER_BOTH(LIST_NAME)                                                                   \
    {                                                                                              \
        initExecutorUpdate(actor, info, LIST_NAME);                                                \
        initExecutorDraw(actor, info, LIST_NAME);                                                  \
    }

void initExecutorPlayer(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー");
    initExecutorDraw(actor, info, "プレイヤー");
};

void initExecutorPlayerPreMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー[PreMovement]");
}

void initExecutorPlayerMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー[Movement]");
}

void initExecutorPlayerModel(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤーモデル");
    initExecutorDraw(actor, info, "プレイヤーモデル");
}

void initExecutorPlayerDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "プレイヤー装飾");
    initExecutorDraw(actor, info, "プレイヤー装飾");
}

void initExecutorEnemy(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵");
    initExecutorDraw(actor, info, "敵");
}

void initExecutorEnemyMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵[Movement]");
}

void initExecutorEnemyDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵装飾");
    initExecutorDraw(actor, info, "敵装飾");
}

void initExecutorEnemyDecorationMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "敵装飾[Movement]");
}

void initExecutorMapObj(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ");
    initExecutorDraw(actor, info, "地形オブジェ");
}

void initExecutorMapObjMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ[Movement]");
}

void initExecutorMapObjDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "地形オブジェ装飾");
    initExecutorDraw(actor, info, "地形オブジェ装飾");
}

void initExecutorNpcDecoration(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "ＮＰＣ装飾");
    initExecutorDraw(actor, info, "ＮＰＣ");
}

void initExecutorShadowVolume(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "影ボリューム");
    initExecutorDraw(actor, info, "影ボリューム");
}

void initExecutorShadowVolumeFillStencil(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "影ボリュームのフィル");
    initExecutorDraw(actor, info, "影ボリュームのフィル");
}

void initExecutorCollisionMapObjDecorationMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "コリジョン地形装飾[Movement]");
}

void initExecutorWatchObj(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "監視オブジェ");
}

void initExecutorDebugMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "デバッグ[ActorMovement]");
}

bool isUsingPrepassTriangleCulling();

void initExecutorModelUpdate(LiveActor* actor, const ActorInitInfo& info) {
    auto modelCtrl = actor->getModelKeeper()->getModelCtrl();
    s32 calcViewCore = modelCtrl->getCalcViewCore();
    // mismatch here ^

    const char* table;
    if (calcViewCore == 1)
        table = "ビュー更新(コア1)";
    else if (calcViewCore == 2)
        table = "ビュー更新(コア2)";
    else if (modelCtrl->getModelObj()->_8c > 1)
        table = "ビュー更新(コア1)";
    else
        table = "ビュー更新(コア2)";

    al::registerExecutorActorDraw(actor, info.executeDirector, "モデル描画バッファ更新");
    if (al::isUsingPrepassTriangleCulling() && al::isIncludePrepassCullingShape(actor))
        al::registerExecutorActorDraw(actor, info.executeDirector, "カリング");
    al::registerExecutorActorUpdate(actor, info.executeDirector, table);
}

void initExecutorDrcAssistMovement(LiveActor* actor, const ActorInitInfo& info) {
    initExecutorUpdate(actor, info, "DRCアシスト[Movement]");
}

}  // namespace al
