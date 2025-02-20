#pragma once

#include <basis/seadTypes.h>
#include <common/aglRenderBuffer.h>

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
    agl::RenderBuffer* mDockedRenderBuffer;
    agl::RenderBuffer* mHandheldRenderBuffer;
    bool mIsDocked;
    agl::DrawContext* mDrawContext;
};

struct GameSystemInfo {
    AudioSystem* mAudioSystem;
    EffectSystem* mEffectSystem;
    LayoutSystem* mLayoutSystem;
    MessageSystem* mMessageSystem;
    NetworkSystem* mNetworkSystem;
    void* field_28;
    GamePadSystem* mGamePadSystem;
    DrawSystemInfo* mDrawSystemInfo;
    FontHolder* mFontHolder;
    NfpDirector* mNfpDirector;
    HtmlViewer* mHtmlViewer;
    ApplicationMessageReceiver* mApplicationMessageReciever;
    WaveVibrationHolder* mWaveVibrationHolder;
};

}  // namespace al
