#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserLookBoard : public CameraPoser {
public:
    CameraPoserLookBoard(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;

    void setLookAtOffset(const sead::Vector3f& offset) { mLookAtOffset = offset; }

private:
    s8 filler[0x140 - sizeof(CameraPoser)];
    sead::Vector3f mLookAtOffset;
    s32 _14c = 0;
};

}  // namespace al

static_assert(sizeof(al::CameraPoserLookBoard) == 0x150, "al::CameraPoserLookBoard size");
