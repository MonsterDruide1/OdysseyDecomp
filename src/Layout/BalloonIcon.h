#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}  // namespace al

class BalloonIcon : public al::LayoutActor {
public:
    BalloonIcon(const char*, const al::LayoutInitInfo&, const sead::Matrix34f*,
                const sead::Vector3f&, const char*, bool, bool, bool);

    void onSnapShotMode();
    void offSnapShotMode();
    void startUpdateDraw();
    void hideAndStopUpdate();
    void exeSleep();
    void exeHide();
    void updateTransAndCheckShow();
    void exeShowAppear();
    void exeShowWait();
    void exeShowEnd();
    void exeEnd();

private:
    u8 _130[0x40];
};

namespace rs {
BalloonIcon* createShopBalloon(const al::LayoutInitInfo&, const sead::Matrix34f*,
                               const sead::Vector3f&);
void setMainScenarioText(BalloonIcon*, const char16*);
}  // namespace rs

static_assert(sizeof(BalloonIcon) == 0x170);
