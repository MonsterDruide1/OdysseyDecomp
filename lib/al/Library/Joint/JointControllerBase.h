#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {

class JointControllerBase {
public:
    JointControllerBase(s32);
    void appendJointId(s32);
    bool findNextId(s32*, s32) const;
    bool isExistId(s32) const;

private:
    void* mCallback;
    sead::PtrArray<s32> _8;
    char filler[0x10];
};

}  // namespace al
