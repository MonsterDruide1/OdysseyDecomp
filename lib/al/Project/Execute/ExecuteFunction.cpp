#include "Project/Execute/ExecuteFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecuteDirector.h"
#include "Library/Execute/ExecuteOrder.h"

namespace alExecuteFunction {

void executeDraw(const al::ExecuteDirector* executeDirector, const char* tableName) {
    executeDirector->draw(tableName);
}

void executeDrawList(const al::ExecuteDirector* executeDirector, const char* tableName,
                     const char* listName) {
    executeDirector->drawList(tableName, listName);
}

}  // namespace alExecuteFunction

namespace alExecutorFunction {

bool isListName(const al::ExecuteOrder& executeOrder, const char* listName) {
    return al::isEqualString(executeOrder.executeGroup, listName);
}

bool isUpdateListActor(const al::ExecuteOrder& executeOrder) {
    return isListName(executeOrder, "ActorMovement") || isListName(executeOrder, "ActorCalcAnim") ||
           isListName(executeOrder, "ActorCalcView") ||
           isListName(executeOrder, "ActorMovementCalcAnim");
}

bool isDrawListActor(const al::ExecuteOrder& executeOrder) {
    return isListName(executeOrder, "ActorDraw");
}

bool isDrawListActorModel(const al::ExecuteOrder& executeOrder) {
    return isListName(executeOrder, "ActorModelDraw") ||
           isListName(executeOrder, "ActorModelDrawForwardOnly") ||
           isListName(executeOrder, "ActorModelDrawForwardForce") ||
           isListName(executeOrder, "ActorModelDrawDepthChromakey") ||
           isListName(executeOrder, "ActorModelDrawDitherChromakey") ||
           isListName(executeOrder, "ActorModelDrawDepthOnly") ||
           isListName(executeOrder, "ActorModelDrawCulling") ||
           isListName(executeOrder, "ActorModelDrawDepthForce") ||
           isListName(executeOrder, "ActorModelDrawDepthXlu") ||
           isListName(executeOrder, "ActorModelDrawDepthIndirect") ||
           isListName(executeOrder, "ActorModelDrawDepthDither") ||
           isListName(executeOrder, "ActorModelDrawDepthDitherIndirect") ||
           isListName(executeOrder, "ActorModelDrawDepthShadow") ||
           isListName(executeOrder, "ActorModelDrawDepthShadowMarchingCube") ||
           isListName(executeOrder, "ActorModelDrawStaticDepthShadow") ||
           isListName(executeOrder, "ActorModelDrawDeferred") ||
           isListName(executeOrder, "ActorModelDrawDeferredOnly") ||
           isListName(executeOrder, "ActorModelDrawDeferredOpa") ||
           isListName(executeOrder, "ActorModelDrawDeferredXlu") ||
           isListName(executeOrder, "ActorModelDrawDeferredMarchingCube") ||
           isListName(executeOrder, "ActorModelDrawDeferredFootPrint") ||
           isListName(executeOrder, "ActorModelDrawDeferredSilhouette") ||
           isListName(executeOrder, "ActorModelDrawDeferredSky") ||
           isListName(executeOrder, "ActorModelDrawPostEffectMask") ||
           isListName(executeOrder, "ActorModelDrawWorldAo") ||
           isListName(executeOrder, "ActorModelDrawIndirect") ||
           isListName(executeOrder, "ActorModelDrawIndirectOnly") ||
           isListName(executeOrder, "ActorModelDrawUpdate") ||
           isListName(executeOrder, "ActorModelDrawPlayerChromakey") ||
           isListName(executeOrder, "ActorModelDrawPlayerChromakeyOpa") ||
           isListName(executeOrder, "ActorModelDrawPlayerChromakeyXlu") ||
           isListName(executeOrder, "ActorModelDrawCharacterChromakey");
}

bool isDrawListLayout(const al::ExecuteOrder& executeOrder) {
    return isListName(executeOrder, "LayoutDraw") || isListName(executeOrder, "LayoutDrawBottom");
}

s32 calcExecutorListNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize,
                           const char* listName) {
    s32 count = 0;
    for (s32 i = 0; i < executeOrderSize; i++)
        if (isListName(executeOrder[i], listName))
            count++;
    return count;
}

s32 calcUpdateListActorNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize) {
    s32 count = 0;
    for (s32 i = 0; i < executeOrderSize; i++)
        if (isUpdateListActor(executeOrder[i]))
            count++;
    return count;
}

s32 calcDrawListActorNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize) {
    s32 count = 0;
    for (s32 i = 0; i < executeOrderSize; i++)
        if (isDrawListActor(executeOrder[i]))
            count++;
    return count;
}

s32 calcDrawListActorModelNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize) {
    s32 count = 0;
    for (s32 i = 0; i < executeOrderSize; i++)
        if (isDrawListActorModel(executeOrder[i]))
            count++;
    return count;
}

s32 calcDrawListLayoutNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize) {
    s32 count = 0;
    for (s32 i = 0; i < executeOrderSize; i++)
        if (isDrawListLayout(executeOrder[i]))
            count++;
    return count;
}

}  // namespace alExecutorFunction
