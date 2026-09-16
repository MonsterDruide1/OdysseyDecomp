#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class CameraPoser;
struct CameraStartInfo;
class NameToCameraParamTransferFunc;

class CameraParamTransfer {
public:
    CameraParamTransfer();

    void setFuncTable(NameToCameraParamTransferFunc, s32);

    void tryTransferParam(CameraPoser*, CameraPoser*) const;
    void tryFindTransferFunc(const char*, const char*) const;
};

}  // namespace al
