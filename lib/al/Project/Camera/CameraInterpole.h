#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadCamera.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class CameraTicket;

class CameraInterpole : public NerveExecutor {
public:
    CameraInterpole();

    void start(const CameraTicket*, f32, s32);
    void update(const sead::LookAtCamera&);
    bool isActive() const;

    void makeLookAtCamera(sead::LookAtCamera*);
    void lerpFovyDegree(f32*);

    void exeDeactive();
    void exeActiveHermite();
    void exeActiveHermiteDistanceHV();
    void exeActiveRotateAxisY();
    void exeActiveBrigade();

    f32 getFovyDegree() const { return mFovyDegree; }

    void setTicket(CameraTicket* ticket) { mTicket = ticket; }

    void requestCancel() { mCancelRequested = true; }

private:
    CameraTicket* mTicket;
    char _18[0x14];
    bool mCancelRequested;
    char _2d[0x1ab];
    f32 mFovyDegree;
    char _1dc[0x1c];
};

static_assert(sizeof(CameraInterpole) == 0x1f8);

}  // namespace al
