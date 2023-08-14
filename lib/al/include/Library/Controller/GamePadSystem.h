#pragma once

#include <container/seadBuffer.h>
#include <prim/seadSafeString.h>

namespace al {
class AudioSystem;

class GamePadSystem {
public:
    GamePadSystem();
    void changeSinglePlayMode();
    void changeMultiPlayMode(s32 minControllers, s32 maxControllers);
    bool isDisconnectPlayable();
    void update();
    void setDisconnectFrame(s32 frame);
    void setInvalidateDisconnectFrame(s32 frame);
    void setPadName(u8, const sead::WSafeString&);
    sead::WSafeString getPadName(u8);
    s32 getPadPlayStyle(u8);  // unknown return type

private:
    bool isDisconnectPlayableImpl();

    s32 mMinControllerCount;
    s32 mMaxControllerCount;
    s32 mDisconnectTimer;
    s32 mDisconnectFrame;
    s32 mInvalidateDisconnectFrame;
    sead::Buffer<sead::WFixedSafeString<256>> mControllerNames;
    AudioSystem* mAudioSystem;
};
static_assert(sizeof(GamePadSystem) == 0x30);

}  // namespace al
