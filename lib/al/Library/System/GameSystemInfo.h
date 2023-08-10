#pragma once

#include <basis/seadTypes.h>
#include <common/aglRenderBuffer.h>

class ProjectNfpDirector;
class ApplicationMessageReceiver;

namespace al {
class EffectSystem;
class LayoutSystem;
class MessageSystem;
class NetworkSystem;
class AudioSystem;
class GamePadSystem;
class HtmlViewer;
class WaveVibrationHolder;

struct GameDrawInfo {
    agl::RenderBuffer* dockedRenderBuffer;
    agl::RenderBuffer* handheldRenderBuffer;
    bool isDocked;
    agl::DrawContext* drawContext;
};

struct GameSystemInfo {
    u64 _0;
    EffectSystem* mEffectSys;
    LayoutSystem* mLayoutSys;
    MessageSystem* mMessageSys;
    NetworkSystem* mNetworkSys;
    AudioSystem* mAudioSys;
    GamePadSystem* mGamePadSys;
    GameDrawInfo* mDrawInfo;
    ProjectNfpDirector* mProjNfpDirector;
    HtmlViewer* mHtmlViewer;
    ApplicationMessageReceiver* mMessageReciever;
    WaveVibrationHolder* mWaveVibrationHolder;
};

}  // namespace al
