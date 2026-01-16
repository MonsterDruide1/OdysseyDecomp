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

void initLayoutInitInfo(LayoutInitInfo*, const LayoutKit*, SceneObjHolder*, const AudioDirector*,
                        const LayoutSystem*, const MessageSystem*, const GamePadSystem*);
void setRenderBuffer(LayoutKit*, const agl::RenderBuffer*);
void executeUpdate(LayoutKit*);
void executeUpdateList(LayoutKit*, const char*, const char*);
void executeUpdateEffect(LayoutKit*);
void executeDraw(const LayoutKit*, const char*);
void executeDrawEffect(const LayoutKit*);
}  // namespace al
