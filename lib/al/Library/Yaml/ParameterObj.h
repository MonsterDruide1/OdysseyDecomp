#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ParameterBase;
class ByamlIter;
class ParameterArray;

class ParameterObj {
public:
    ParameterObj();

    void pushBackListNode(ParameterBase*);
    bool tryGetParam(const ByamlIter&);
    void addArray(ParameterArray*, const sead::SafeStringBase<char>&);
    bool isEqual(const ParameterObj&) const;
    void copy(const ParameterObj&);
    void copyLerp(const ParameterObj&, const ParameterObj&, f32);
    void findParameter(const char*) const;

    ParameterObj* getNext() const { return mNext; }

    void setNext(ParameterObj* obj) { mNext = obj; }

    void setKey(const sead::SafeString& key) { mKey = key; }

private:
    ParameterBase* mFirstParam;
    ParameterBase* mLastParam;
    ParameterObj* mNext;
    ParameterArray* mParamArray;
    sead::FixedSafeString<0x40> mKey;
};

}  // namespace al
