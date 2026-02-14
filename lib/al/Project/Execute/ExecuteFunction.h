#pragma once

#include <basis/seadTypes.h>

namespace al {
class ExecuteDirector;
struct ExecuteOrder;
}  // namespace al

namespace alExecuteFunction {
void executeDraw(const al::ExecuteDirector* executeDirector, const char* tableName);
void executeDrawList(const al::ExecuteDirector* executeDirector, const char* tableName,
                     const char* listName);
}  // namespace alExecuteFunction

namespace alExecutorFunction {

bool isListName(const al::ExecuteOrder& executeOrder, const char* listName);
bool isUpdateListActor(const al::ExecuteOrder& executeOrder);
bool isDrawListActor(const al::ExecuteOrder& executeOrder);
bool isDrawListActorModel(const al::ExecuteOrder& executeOrder);
bool isDrawListLayout(const al::ExecuteOrder& executeOrder);
s32 calcExecutorListNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize,
                           const char* listName);
s32 calcUpdateListActorNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize);
s32 calcDrawListActorNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize);
s32 calcDrawListActorModelNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize);
s32 calcDrawListLayoutNumMax(const al::ExecuteOrder* executeOrder, s32 executeOrderSize);

}  // namespace alExecutorFunction
