#pragma once

#include <prim/seadSafeString.h>

namespace al {
struct ActorInitInfo;
class IUseMessageSystem;
class IUseSceneObjHolder;
class LayoutActor;
class PlacementInfo;
class PlacementId;
}  // namespace al
class CollectBgm;
class GameDataHolderAccessor;

namespace rs {
const char16* searchNpcMessage(const al::ActorInitInfo&, const al::LayoutActor*);
void makeActorMessageLabel(sead::BufferedSafeString*, const al::ActorInitInfo&, const char*);
void makeMessageLabel(sead::BufferedSafeString*, const al::PlacementInfo&, const char*);
void makeMessageLabel(sead::BufferedSafeString*, const al::PlacementId*, const char*);
void setPaneStageMessageActorLabel(al::LayoutActor*, const char*, const al::ActorInitInfo&,
                                   const char*);
bool trySetPaneStageMessageIfExist(al::LayoutActor*, const char*, const char*, const char*);
const char* getPlacementStageName(GameDataHolderAccessor, const al::ActorInitInfo&);
const char16* getStageMessageActorLabel(al::LayoutActor*, const al::ActorInitInfo&, const char*);
bool isExistLabelInSystemMessageWithFileName(const al::IUseMessageSystem*, const char*,
                                             const char*);
bool isExistLabelInStageMessageWithFileName(const al::IUseMessageSystem*, const char*, const char*);
bool trySetPaneSystemMessageIfExist(al::LayoutActor*, const char*, const char*, const char*);
bool trySetPaneStringCheckpos32Name(al::LayoutActor*, const char*, s32, const char*);
const char* getCheckpointLabelPrefix();
void setPaneStringWorldNameForWorldMap(al::LayoutActor*, const char*, s32);
void setPaneStringWorldNameForWorldMapWithoutRuby(al::LayoutActor*, const char*, s32);
void setRaceRecordMessage(al::LayoutActor*, const char*, s32);
void setRaceRecordMessageCsec(al::LayoutActor*, const char*, s32);
void replaceRaceRecordMessageCsec(al::LayoutActor*, const char*, s32, const char16*, const char*);
void setRaceNoRecordMessage(al::LayoutActor*, const char*);
void setMiniGameNoCountMessage(al::LayoutActor*, const char*);
const char16* getWorldShinePictureFont(const al::LayoutActor*, s32, bool);
const char16* getNullShinePictureFont(const al::LayoutActor*);
const char16* getWorldCoinCollectPictureFont(const al::LayoutActor*, s32);
const char16* getWorldCoinCollectPictureFont(const al::LayoutActor*);
const char* getPlacementStageName(GameDataHolderAccessor, const al::PlacementInfo&);
const char16* getCollectBgmMessage(const al::IUseMessageSystem*, const CollectBgm*);
void setPaneCurrentCoinNum(al::LayoutActor*);
bool tryGetMapMainScenarioLabel(sead::BufferedSafeString*, sead::BufferedSafeString*, bool*,
                                const al::IUseSceneObjHolder*);
}  // namespace rs
