#include "Project/Base/ParamRequestInterp.h"

#include "Library/Yaml/ParameterBase.h"

namespace al {

bool IUseRequestParam::isEqual(const IUseRequestParam& requestParam) const {
    if (!getParamObj())
        return false;

    return getParamObj()->isEqual(*requestParam.getParamObj());
}

void IUseRequestParam::copy(const IUseRequestParam& requestParam) {
    if (!getParamObj())
        return;

    getParamObj()->copy(*requestParam.getParamObj());
}

void IUseRequestParam::copyInterp(const IUseRequestParam& requestParamStart,
                                  const IUseRequestParam& requestParamEnd, f32 rate) {
    if (!getParamObj())
        return;

    getParamObj()->copyLerp(*requestParamStart.getParamObj(), *requestParamEnd.getParamObj(), rate);
}

}  // namespace al
