#pragma once

#include "Library/Nerve/NerveExecutor.h"
#include "gfx/seadCamera.h"

namespace al {
class CameraPoser;
class CameraTicket;

class CameraInterpole : public NerveExecutor {
public:
    CameraInterpole();
    ~CameraInterpole() override;

    void start(const CameraTicket* ticket, float, int);
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
    const al::CameraPoser* _20 = nullptr;
    s32 _28 = -1;
    bool _2c = false;
    bool pad_2d[3];
    sead::Vector3f _30_pos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _3c_at = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _49_up = {0.0f, 1.0f, 0.0f};
    int pad_54;
    sead::LookAtCamera _48 = {};
    sead::LookAtCamera _B8 = {};
    sead::LookAtCamera _118 = {};
    sead::LookAtCamera _178 = {};
    f32 _1d8 = 30.0f;
    f32 _1dc = 30.0f;
    f32 _1e0 = 0.0f;
    sead::Vector3f _1e4 = {0.0f, 0.0f, 0.0f};
    f32 _1f0 = 0.0f;
    s32 _1f4 = 0;
};

}  // namespace al
