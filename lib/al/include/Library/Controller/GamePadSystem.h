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
    int getPadPlayStyle(u8);  // unknown return type

private:
    bool isDisconnectPlayableImpl();

    int mMinControllerCount;
    int mMaxControllerCount;
    int mDisconnectTimer;
    int mDisconnectFrame;
    int mInvalidateDisconnectFrame;
    sead::Buffer<sead::WFixedSafeString<256>> mControllerNames;
    AudioSystem* mAudioSystem;
};
static_assert(sizeof(GamePadSystem) == 0x30);

}  // namespace al
