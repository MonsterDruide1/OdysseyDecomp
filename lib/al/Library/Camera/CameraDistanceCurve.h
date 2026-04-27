#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;
struct CameraDistanceAtPoint;

class CameraDistanceCurve {
public:
    CameraDistanceCurve(const char* name, const CameraDistanceAtPoint* points, s32 pointNum);

    static const CameraDistanceCurve* getDefaultCurve();
    static const CameraDistanceCurve* findOrDefaultCurve(const ByamlIter& iter);
    static const CameraDistanceCurve* getRocketFlowerCurve();
    static const CameraDistanceCurve* getGiantWanderBossCurve();
    static const CameraDistanceCurve* getKoopaShellCurve();
    static const CameraDistanceCurve* getTRexPatrolCurve();

    f32 calcDistance(f32 rate) const;
};

}  // namespace al
