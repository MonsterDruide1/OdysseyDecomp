#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class ParameterObj;
class ParameterList;

enum class ParameterType : s32 {
    Invalid,
    Bool,
    F32,
    S32,
    U32,
    V2f,
    V2s32,
    V3f,
    V4f,
    Q4f,
    C4f,
    StringRef,
    String32,
    String64,
    String128,
    String256,
};

class ParameterBase {
public:
    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterObj*, bool);

    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterList*, bool);

    virtual const char* getParamTypeStr() const = 0;
    virtual ParameterType getParamType() const = 0;
    virtual const void* ptr() const = 0;
    virtual void* ptr() = 0;
    virtual void afterGetParam();
    virtual s32 getParamSize() = 0;
    virtual bool isEqual(const ParameterBase*);
    virtual bool copy(const ParameterBase*);
    virtual bool copyLerp(const ParameterBase*, const ParameterBase*, f32);

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

    const sead::FixedSafeString<0x40>& getParamName() const { return mName; }

    s32 getHash() const { return mHash; }

private:
    ParameterBase* mNext = nullptr;
    sead::FixedSafeString<0x40> mName;
    s32 mHash;
};

}  // namespace al
