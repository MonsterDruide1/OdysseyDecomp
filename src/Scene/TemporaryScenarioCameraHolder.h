#pragma once

#include <basis/seadTypes.h>

namespace al {
class CameraTicket;
}

struct ScenarioCameraRelationInfo {
    s32 id;
    al::CameraTicket* camera;
};

class TemporaryScenarioCameraHolder {
public:
    TemporaryScenarioCameraHolder(ScenarioCameraRelationInfo* buffer, s32);
    void registInfo(s32 id, al::CameraTicket* ticket);
    al::CameraTicket* tryFindEntranceCameraByShineId(s32 id) const;

private:
    ScenarioCameraRelationInfo* mBuffer;
    s32 mSize = 0;
};
