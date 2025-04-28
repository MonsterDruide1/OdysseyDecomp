#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ParameterBase;
class ByamlIter;
class ParameterArray;

class ParameterObj {
public:
    ParameterObj();

    void pushBackListNode(ParameterBase* param);
    void tryGetParam(const ByamlIter& iter);
    void addArray(ParameterArray* array, const sead::SafeStringBase<char>& key);
    bool isEqual(const ParameterObj& obj) const;
    void copy(const ParameterObj& obj);
    void copyLerp(const ParameterObj& objA, const ParameterObj& objB, f32 rate);
    ParameterBase* findParameter(const char* name) const;

    ParameterBase* getRootParam() const { return mRootParam; }

    ParameterObj* getNext() const { return mNext; }

    ParameterArray* getParamArray() const { return mParamArray; }

    void setNext(ParameterObj* obj) { mNext = obj; }

    void setKey(const sead::SafeString& key) { mKey = key; }

private:
    ParameterBase* mRootParam = nullptr;
    ParameterBase* mTailParam = nullptr;
    ParameterObj* mNext = nullptr;
    ParameterArray* mParamArray = nullptr;
    sead::FixedSafeString<0x40> mKey;
};

}  // namespace al
