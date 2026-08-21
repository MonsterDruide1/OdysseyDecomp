#pragma once

#include <basis/seadTypes.h>
#include <common/aglRenderBuffer.h>
#include <nn/oe.h>

namespace al {
class EffectSystem;
class LayoutSystem;
class MessageSystem;
class NetworkSystem;
class AudioSystem;
class GamePadSystem;
class HtmlViewer;
class WaveVibrationHolder;
class NfpDirector;
class ApplicationMessageReceiver;
class FontHolder;

struct DrawSystemInfo {
    agl::RenderBuffer* dockedRenderBuffer;
    agl::RenderBuffer* handheldRenderBuffer;
    bool isDocked;
    agl::DrawContext* drawContext;
};

struct GameSystemInfo {
    GameSystemInfo();

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
    ApplicationMessageReceiver* applicationMessageReceiver;
    WaveVibrationHolder* waveVibrationHolder;
};

enum class GpuPerformance {
    unk1 = 0,
    unk2 = 1,
    unk3 = 2,
};

void setGpuPerformance(GpuPerformance, nn::oe::PerformanceMode);

}  // namespace al
