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

    void setFuncTable(const NameToCameraParamTransferFunc* functions, s32 count);

    bool tryTransferParam(CameraPoser*, CameraPoser*) const;
    void* tryFindTransferFunc(const char*, const char*) const;  // unknown return type

private:
    const NameToCameraParamTransferFunc* mFunctions;
    s32 mFunctionCount;
};

static_assert(sizeof(CameraParamTransfer) == 0x10);

}  // namespace al
