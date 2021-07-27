#pragma once

#include <nn/g3d/ResFile.h>

namespace agl {
namespace g3d {
class ResFile {
public: //TODO requires implementation of both of these
    static void BindTexture(nn::g3d::ResFile*, nn::g3d::ResFile*);
    static void Cleanup(nn::g3d::ResFile*);
};
};  // namespace g3d
};  // namespace agl