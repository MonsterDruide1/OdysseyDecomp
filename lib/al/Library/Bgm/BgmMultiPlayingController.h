#pragma once

#include <basis/seadTypes.h>

namespace al {

class BgmMultiPlayingController {
public:
    struct PlayParams {
        u8 _0[0x18] = {};
    };

    BgmMultiPlayingController();

    void use();
    void removeAllMultiBgmPlayingInfo();
    void activate(const PlayParams& params, s32 fadeFrame, bool isForce);
    void deactivate(bool isForce, s32 fadeFrame);
    void update();
    void tryAddMultiBgmPlayingInfo(const char* playName, s32 fadeFrame, bool isForce);
    void tryRemoveMultiBgmPlayingInfo(const char* playName);
    void enableMultiBgmPlayingInfo(const char* playName);
    void disableMultiBgmPlayingInfo(const char* playName);

private:
    u8 _0[0x40] = {};
};

static_assert(sizeof(BgmMultiPlayingController::PlayParams) == 0x18);
static_assert(sizeof(BgmMultiPlayingController) == 0x40);

}  // namespace al
