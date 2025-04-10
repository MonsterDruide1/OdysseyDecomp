#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>

#include "Library/HostIO/HioNode.h"

namespace al {

class JointControllerBase : public HioNode {
public:
    JointControllerBase(s32);

    virtual void calcJointCallback(s32, sead::Matrix34f*) = 0;
    virtual void appendJointId(s32);
    virtual const char* getCtrlTypeName() const = 0;
    virtual bool tryValidateConstraints(u32);
    virtual bool tryInvalidateConstraints(u32);

    bool findNextId(s32*, s32) const;
    bool isExistId(s32) const;

private:
    sead::PtrArray<s32> _8;
    char filler[0x10];
};

}  // namespace al
