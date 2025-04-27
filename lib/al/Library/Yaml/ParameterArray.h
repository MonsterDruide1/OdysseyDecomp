#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class ParameterObj;

class ParameterArray {
public:
    ParameterArray();
    void tryGetParam(const ByamlIter& iter);
    bool isEqual(const ParameterArray& array) const;
    void copy(const ParameterArray& array);
    void copyLerp(const ParameterArray& arrayA, const ParameterArray& arrayB, f32 value);
    void addObj(ParameterObj* obj);
    void clearObj();
    void removeObj(ParameterObj* obj);
    bool isExistObj(ParameterObj* obj);

    ParameterObj* getFirstObj() const { return mFirstObj; }

    s32 getSize() const { return mSize; }

private:
    ParameterObj* mFirstObj = nullptr;
    ParameterArray* mNext = nullptr;
    sead::FixedSafeString<0x40> mParamObjKey;
    s32 mSize = 0;
};

}  // namespace al
