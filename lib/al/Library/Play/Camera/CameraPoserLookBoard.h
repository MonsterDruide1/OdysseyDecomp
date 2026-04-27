#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserLookBoard : public CameraPoser {
public:
    CameraPoserLookBoard(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;

private:
    s8 filler[0x150 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserLookBoard) == 0x150, "al::CameraPoserLookBoard size");
