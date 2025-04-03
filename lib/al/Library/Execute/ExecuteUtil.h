#pragma once

namespace al {
class LiveActor;
class ExecuteDirector;
class LayoutActor;
class IUseExecutor;
class FunctorBase;

void registerExecutorActorUpdate(LiveActor* actor, ExecuteDirector* director, const char* listName);
void registerExecutorActorDraw(LiveActor* actor, ExecuteDirector* director, const char* listName);
void registerExecutorLayoutUpdate(LayoutActor* layout, ExecuteDirector* director,
                                  const char* listName);
void registerExecutorLayoutDraw(LayoutActor* layout, ExecuteDirector* director,
                                const char* listName);
void registerExecutorUser(IUseExecutor* user, ExecuteDirector* director, const char* listName);
void registerExecutorFunctor(const char* listName, ExecuteDirector* director,
                             const FunctorBase& functor);
void registerExecutorFunctorDraw(const char* listName, ExecuteDirector* director,
                                 const FunctorBase& functor);
}  // namespace al

namespace alActorSystemFunction {
void addToExecutorMovement(al::LiveActor* actor);
void addToExecutorDraw(al::LiveActor* actor);
void removeFromExecutorMovement(al::LiveActor* actor);
void removeFromExecutorDraw(al::LiveActor* actor);
void updateExecutorDraw(al::LiveActor* actor);
}  // namespace alActorSystemFunction

namespace alExecuteFunction {
void updateEffect(const al::ExecuteDirector* director);
void updateEffectSystem(const al::ExecuteDirector* director);
void updateEffectPlayer(const al::ExecuteDirector* director);
void updateEffectDemo(const al::ExecuteDirector* director);
void updateEffectLayout(const al::ExecuteDirector* director);
}  // namespace alExecuteFunction
