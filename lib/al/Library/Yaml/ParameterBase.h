#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class ParameterObj;
class ParameterList;

class ParameterBase {
public:
    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterObj*, bool);

    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterList*, bool);

    virtual void afterGetParam() {}

    virtual bool isEqual(const ParameterBase*);
    virtual void copy(const ParameterBase*);
    virtual void copyLerp(const ParameterBase*, const ParameterBase*, f32);

    void initializeListNode(const sead::SafeString&, const sead::SafeString&,
                            const sead::SafeString&, ParameterObj*, bool);
    void initializeListNode(const sead::SafeString&, const sead::SafeString&,
                            const sead::SafeString&, ParameterList*, bool);
    void initialize(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                    bool);
    u32 calcHash(const sead::SafeString&);
    void tryGetParam(const ByamlIter&);

    ParameterBase* getNext() const { return mNext; }

    void setNext(ParameterBase* param) { mNext = param; }

private:
    ParameterBase* mNext;
    sead::FixedSafeString<0x40> mParamName;
    s32 mParamHash;
};

}  // namespace al
