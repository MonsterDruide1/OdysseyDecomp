#pragma once

#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class ByamlIter;
class EventFlowEventData;
class EventFlowExecutor;
class EventFlowMovement;
class EventFlowNode;
class EventFlowNodeInitInfo;
class IEventFlowEventReceiver;
class IUseEventFlowData;
class IUseMessageSystem;
class LayoutInitInfo;
class LiveActor;

bool isInvalidUiCollisionCheck(const IUseEventFlowData* user);
bool isExistLookAtJointCtrl(const IUseEventFlowData* user);
s32 initRailMoveType(const LiveActor* actor, const ActorInitInfo& info);
bool isRailMoveLoop(s32);
bool isRailMoveTurn(s32);
bool isRailMoveRestart(s32);
void initEventReceiver(EventFlowExecutor* executor, IEventFlowEventReceiver* receiver);
void initEventFlowNode(EventFlowNode* node, const EventFlowNodeInitInfo& info);
void initEventQuery(EventFlowNode* node, const EventFlowNodeInitInfo& info);
void registerEventCamera(EventFlowNode* node, const char* cameraName);
void initEventNodeMessage(const char16** outMessage, const EventFlowNode* node,
                          const EventFlowNodeInitInfo& info, const char*);
const char* getParamIterKeyString(const EventFlowNodeInitInfo& info, const char* key);
const char* getObjId(const EventFlowNodeInitInfo& info);
const char* getPlacementStageName(const EventFlowNodeInitInfo& info);
ActorInitInfo* getActorInitInfo(const EventFlowNodeInitInfo& info);
void makeParamMessageString(const char16** outMessage, const EventFlowNodeInitInfo& info,
                            const char* key);
void initEventFlowMovement(EventFlowMovement* movement, const EventFlowNodeInitInfo& info);
LayoutInitInfo* getLayoutInitInfo(const EventFlowNodeInitInfo& info);
bool tryGetParamIter(ByamlIter* outIter, const EventFlowNodeInitInfo& info);
bool tryGetParamIterKeyInt(s32* outValue, const EventFlowNodeInitInfo& info, const char* key);
bool tryGetParamIterKeyFloat(f32* outValue, const EventFlowNodeInitInfo& info, const char* key);
bool tryGetParamIterKeyBool(bool* outValue, const EventFlowNodeInitInfo& info, const char* key);
const char* tryGetParamIterKeyString(const EventFlowNodeInitInfo& info, const char* key);
bool getParamIterKeyBool(const EventFlowNodeInitInfo& info, const char* key);
s32 getParamIterKeyInt(const EventFlowNodeInitInfo& info, const char* key);
const char* getNodeIterKeyString(const EventFlowNodeInitInfo& info, const char* key);
const char16* getMessageString(const IUseMessageSystem* user, const ByamlIter& iter);
const char* getParamMessageLabel(const EventFlowNodeInitInfo& info, const char* key);
void restartEventMovement(EventFlowNode* node);
void stopEventMovement(EventFlowNode* node);
void recordActorFront(EventFlowNode* node);
const sead::Vector3f& getRecordActorFront(const EventFlowNode* node);
bool sendEvent(const EventFlowNode* node, const EventFlowEventData* data);
const char* judgeQuery(const EventFlowNode* node, const char* query);
void invalidateClipping(EventFlowNode* node);
void validateClipping(EventFlowNode* node);
s32 getCaseEventNum(const EventFlowNode* node);
s32 getCaseEventNextId(const EventFlowNode* node, s32 caseNum);
s32 findCaseEventNextId(const EventFlowNode* node, const char* caseName);
const char16* getCaseEventMessage(const EventFlowNode* node, s32 caseNum);
bool isTurnMovement(const EventFlowNode* node);
}  // namespace al
