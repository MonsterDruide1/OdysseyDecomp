#pragma once

namespace agl {
class DrawContext;
class RenderBuffer;
}  // namespace agl

namespace al {
class ApplicationMessageReceiver;
class AudioSystem;
class EffectSystem;
class FontHolder;
class GamePadSystem;
class HtmlViewer;
class LayoutSystem;
class MessageSystem;
class NetworkSystem;
class NfpDirector;
class WaveVibrationHolder;

struct DrawSystemInfo {
    agl::RenderBuffer* dockedRenderBuffer;
    agl::RenderBuffer* handheldRenderBuffer;
    bool isDocked;
    agl::DrawContext* drawContext;
};

struct GameSystemInfo {
    AudioSystem* audioSystem;
    EffectSystem* effectSystem;
    LayoutSystem* layoutSystem;
    MessageSystem* messageSystem;
    NetworkSystem* networkSystem;
    void* field_28;
    GamePadSystem* gamePadSystem;
    DrawSystemInfo* drawSystemInfo;
    FontHolder* fontHolder;
    NfpDirector* nfpDirector;
    HtmlViewer* htmlViewer;
    ApplicationMessageReceiver* applicationMessageReciever;
    WaveVibrationHolder* waveVibrationHolder;
};

}  // namespace al
