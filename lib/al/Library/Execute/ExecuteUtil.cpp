#include "Library/Execute/ExecuteUtil.h"

#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteRequestKeeper.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Model/ModelDrawerBase.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Shadow/ActorShadowUtil.h"

namespace al {

void registerExecutorActorUpdate(LiveActor* actor, ExecuteDirector* director,
                                 const char* listName) {
    director->registerActorUpdate(actor, listName);
}

void registerExecutorActorDraw(LiveActor* actor, ExecuteDirector* director, const char* listName) {
    director->registerActorDraw(actor, listName);
}

void registerExecutorLayoutUpdate(LayoutActor* layout, ExecuteDirector* director,
                                  const char* listName) {
    director->registerLayoutUpdate(layout, listName);
}

void registerExecutorLayoutDraw(LayoutActor* layout, ExecuteDirector* director,
                                const char* listName) {
    director->registerLayoutDraw(layout, listName);
}

void registerExecutorUser(IUseExecutor* user, ExecuteDirector* director, const char* listName) {
    director->registerUser(user, listName);
}

void registerExecutorFunctor(const char* listName, ExecuteDirector* director,
                             const FunctorBase& functor) {
    director->registerFunctor(functor, listName);
}

void registerExecutorFunctorDraw(const char* listName, ExecuteDirector* director,
                                 const FunctorBase& functor) {
    director->registerFunctorDraw(functor, listName);
}

}  // namespace al

namespace alActorSystemFunction {

void addToExecutorMovement(al::LiveActor* actor) {
    actor->getExecuteInfo()->getRequestKeeper()->request(
        actor, al::ExecuteRequestKeeper::Request_Movement);
}

void addToExecutorDraw(al::LiveActor* actor) {
    actor->getExecuteInfo()->getRequestKeeper()->request(actor,
                                                         al::ExecuteRequestKeeper::Request_Draw);
}

void removeFromExecutorMovement(al::LiveActor* actor) {
    actor->getExecuteInfo()->getRequestKeeper()->request(
        actor, al::ExecuteRequestKeeper::Request_RemoveFromMovement);
}

void removeFromExecutorDraw(al::LiveActor* actor) {
    actor->getExecuteInfo()->getRequestKeeper()->request(
        actor, al::ExecuteRequestKeeper::Request_RemoveFromDraw);
}

void updateExecutorDrawRec(al::LiveActor* baseActor, al::LiveActor* actor) {
    if (al::isAlive(actor) && actor->getExecuteInfo()) {
        for (s32 i = 0; i < actor->getExecuteInfo()->getDrawerCount(); i++) {
            actor->getExecuteInfo()->getDrawer(i)->updateModel(
                actor->getModelKeeper()->getModelCtrl());
        }
    }

    al::updateDepthShadowMapCtrlShapeVisible(actor);

    if (al::isExistSubActorKeeper(actor)) {
        s32 subActorNum = al::getSubActorNum(actor);
        for (s32 i = 0; i < subActorNum; i++) {
            al::LiveActor* subActor = al::getSubActor(actor, i);
            if (baseActor->getModelKeeper() == subActor->getModelKeeper())
                updateExecutorDrawRec(baseActor, subActor);
        }
    }
}

void updateExecutorDraw(al::LiveActor* actor) {
    updateExecutorDrawRec(actor, actor);
}

}  // namespace alActorSystemFunction

namespace alExecuteFunction {

void updateEffect(const al::ExecuteDirector* director) {
    director->executeList("更新", "エフェクト（前処理）");
    director->executeList("更新", "エフェクト（３Ｄ）");
    director->executeList("更新", "エフェクト（プレイヤー）");
    director->executeList("更新", "エフェクト（Ｚソート）");
    director->executeList("更新", "エフェクト（カメラデモ）");
    director->executeList("更新", "エフェクト（カメラ前エフェクト）");
    director->executeList("更新", "エフェクト（ベース２Ｄ）");
    director->executeList("更新", "エフェクト（２Ｄ）");
    director->executeList("更新", "エフェクト（後処理）");
}

void updateEffectSystem(const al::ExecuteDirector* director) {
    director->executeList("更新", "エフェクト（前処理）");
    director->executeList("更新", "エフェクト（後処理）");
}

void updateEffectPlayer(const al::ExecuteDirector* director) {
    director->executeList("更新", "エフェクト（前処理）");
    director->executeList("更新", "エフェクト（プレイヤー）");
    director->executeList("更新", "エフェクト（ベース２Ｄ）");
    director->executeList("更新", "エフェクト（２Ｄ）");
    director->executeList("更新", "エフェクト（後処理）");
}

void updateEffectDemo(const al::ExecuteDirector* director) {
    director->executeList("更新", "エフェクト（前処理）");
    director->executeList("更新", "エフェクト（カメラデモ）");
    director->executeList("更新", "エフェクト（カメラ前エフェクト）");
    director->executeList("更新", "エフェクト（ベース２Ｄ）");
    director->executeList("更新", "エフェクト（２Ｄ）");
    director->executeList("更新", "エフェクト（後処理）");
}

void updateEffectLayout(const al::ExecuteDirector* director) {
    director->executeList("更新", "エフェクト（前処理）");
    director->executeList("更新", "エフェクト（２Ｄ）");
    director->executeList("更新", "エフェクト（後処理）");
}

}  // namespace alExecuteFunction
