#pragma once

#include "Library/Nerve/NerveExecutor.h"
#include "gfx/seadCamera.h"

namespace al {
class CameraPoser;
class CameraTicket;

class CameraInterpole : public NerveExecutor {
public:
    CameraInterpole();

    void start(const CameraTicket* ticket, f32 prevFovyDegree, s32 endInterpoleStep);
    void update(const sead::LookAtCamera& camera);
    bool isActive() const;
    void makeLookAtCamera(sead::LookAtCamera* camera) const;
    void lerpFovyDegree(f32);
    void exeDeactive();
    void exeActiveHermite();
    void exeActiveHermiteDistanceHV();
    void exeActiveRotateAxisY();
    void exeActiveBrigade();

private:
    const al::CameraTicket* mNextTicket = nullptr;
    const al::CameraTicket* mPrevTicket = nullptr;
    al::CameraPoser* mPrevPoser = nullptr;
    s32 mEndInterpoleStep = -1;
    bool mIsRequestCancel = false;
    sead::Vector3f _30_pos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _3c_at = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _49_up = sead::Vector3f::ey;
    sead::LookAtCamera _48;
    sead::LookAtCamera _B8;
    sead::LookAtCamera _118;
    sead::LookAtCamera _178;
    f32 _1d8 = 30.0f;
    f32 mPrevFovyDegree = 30.0f;
    f32 mAngleRotateY = 0.0f;
    sead::Vector3f _1e4 = {0.0f, 0.0f, 0.0f};
    f32 _1f0 = 0.0f;
    f32 _1f4 = 0.0f;
};

}  // namespace al
