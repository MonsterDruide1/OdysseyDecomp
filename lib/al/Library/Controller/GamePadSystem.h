#pragma once

#include <prim/seadSafeString.h>

namespace al {

class GamePadSystem {
public:
    GamePadSystem();
    void changeSinglePlayMode();
    bool isDisconnectPlayable();
    sead::WFixedSafeString<256> getPadName(u8); // might return a parent type instead of this specific format
    int getPadPlayStyle(u8);  // unknown return type
    void update();
    bool isDisconnectPlayableImpl();
    void setDisconnectFrame(int);
    void setInvalidateDisconnectFrame(int);
    void setPadName(u8, const sead::SafeString&);
    void changeMultiPlayMode(int, int);

private:
    char filler[0x30];
};
static_assert(sizeof(GamePadSystem) == 0x30);

}  // namespace al
