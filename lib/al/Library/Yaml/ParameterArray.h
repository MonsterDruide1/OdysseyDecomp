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
    void copyLerp(const ParameterArray& arrayA, const ParameterArray& arrayB, f32 rate);
    void addObj(ParameterObj* obj);
    void clearObj();
    void removeObj(ParameterObj* obj);
    bool isExistObj(ParameterObj* obj);

    ParameterObj* getRootObjNode() const { return mRootObjNode; }

    ParameterArray* getNext() const { return mNext; }

    void setNext(ParameterArray* array) { mNext = array; }

    void setKey(const sead::SafeString& key) { mKey = key; }

    s32 getSize() const { return mSize; }

private:
    ParameterObj* mRootObjNode = nullptr;
    ParameterArray* mNext = nullptr;
    sead::FixedSafeString<0x40> mKey;
    s32 mSize = 0;
};

}  // namespace al
