#pragma once

namespace al {
class LiveActor;
};

class HackCapThrowParam {
public:
    HackCapThrowParam(al::LiveActor*);

private:
    float* hackThrowHeight;
    float* maxVel;
    float* continuousThrowSpeed;
    int* breakTime;
    float* maxDist;
    int* endpointStopTime;
    int* maxEndpointStopTime;
    float* returnStrength;
    float* maxRetSpeed;
    float* turnAngleLimit;
    float* waterMaxSpeed;
    float* waterDist;
    int* waterBreakTime;
    float* waterMaxRetSpeed;
    float* tornadoDist;
    float* tornadoMaxDist;
    int* tornadoReflectTime;
    float* rollSpeed;
    float* rollDistTop;
    float* rollDistBottom;
    int* rollBrakeTimeTop;
    int* rollBrakeTimeBottom;
    float* rollGroundGroundedPoseTrack;
    float* rollGroundAerialPoseTrack;
};
