#pragma once

namespace agl {
class RenderBuffer;

}  // namespace agl

namespace al {
class AudioDirector;
class GamePadSystem;
class LayoutInitInfo;
class LayoutKit;
class LayoutSystem;
class MessageSystem;
class SceneObjHolder;

void initLayoutInitInfo(LayoutInitInfo* info, const LayoutKit* kit, SceneObjHolder* sceneObjHolder,
                        const AudioDirector* audioDirector, const LayoutSystem* layoutSystem,
                        const MessageSystem* messageSystem, const GamePadSystem* gamePadSystem);
void setRenderBuffer(LayoutKit*, const agl::RenderBuffer*);
void executeUpdate(LayoutKit*);
void executeUpdateList(LayoutKit* kit, const char* tableName, const char* listName);
void executeUpdateEffect(LayoutKit* kit);
void executeDraw(const LayoutKit* kit, const char* tableName);
void executeDrawEffect(const LayoutKit* kit);
}  // namespace al
