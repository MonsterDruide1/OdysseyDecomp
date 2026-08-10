#pragma once

#include <basis/seadTypes.h>

namespace al {
class ParameterObj;

class IUseRequestParam {
public:
    virtual const char* getParamName() const = 0;
    virtual ParameterObj* getParamObj() = 0;
    virtual const ParameterObj* getParamObj() const = 0;
    virtual bool isEqual(const IUseRequestParam& requestParam) const;
    virtual void copy(const IUseRequestParam& requestParam);
    virtual void copyInterp(const IUseRequestParam& requestParamStart,
                            const IUseRequestParam& requestParamEnd, f32 rate);
};

static_assert(sizeof(IUseRequestParam) == 0x8);

}  // namespace al
