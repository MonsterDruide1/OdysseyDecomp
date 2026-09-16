#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadCamera.h>

namespace al {
class CameraTicket;

class CameraInterpole {
public:
    CameraInterpole();
    ~CameraInterpole();

    void update(const sead::LookAtCamera&);

    void exeActiveHermite();
    void exeActiveHermiteDistanceHV();
    void exeActiveBrigade();
    void exeDeactive();

    void start(const CameraTicket*, f32, s32);
    bool isActive() const;

    void makeLookAtCamera(sead::LookAtCamera*);
    void lerpFovyDegree(f32*);

    f32 getFovyDegree() const { return mFovyDegree; }

    void setTicket(CameraTicket* ticket) { mTicket = ticket; }

    void requestCancel() { mCancelRequested = true; }

    char _0[0x10];
    CameraTicket* mTicket;
    char _18[0x14];
    bool mCancelRequested;
    char _2d[0x1ab];
    f32 mFovyDegree;
    char _1dc[0x1C];
};

static_assert(sizeof(CameraInterpole) == 0x1f8);

}  // namespace al
