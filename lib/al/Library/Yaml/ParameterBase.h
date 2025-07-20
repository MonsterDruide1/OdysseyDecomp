#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>
#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class ParameterObj;
class ParameterList;

// Enum formatted for match purposes
SEAD_ENUM(YamlParamType,
    Invalid ,
    Bool ,
    F32 ,
    S32 ,
    U32 ,
    V2f ,
    V2s32 ,
    V3f ,
    V4f ,
    Q4f ,
    C4f ,
    StringRef ,
    String32 ,
    String64 ,
    String128 ,
    String256 ,
    String512 ,
    String1024 ,
    String2048 ,
    String4096
);

#define PARAM_TYPE_DEF(Name, Type)                                                                 \
    class Parameter##Name : public Parameter<Type> {                                               \
    public:                                                                                        \
        Parameter##Name(const sead::SafeString& a, const sead::SafeString& b,                      \
                        const sead::SafeString& c, ParameterObj* d, bool e)                        \
            : Parameter(a, b, c, d, e) {}                                                          \
        Parameter##Name(const sead::SafeString& a, const sead::SafeString& b,                      \
                        const sead::SafeString& c, ParameterList* d, bool e)                       \
            : Parameter(a, b, c, d, e) {}                                                          \
                                                                                                   \
        const char* getParamTypeStr() const override {                                             \
            return YamlParamType::text(YamlParamType::Name);                                       \
        }                                                                                          \
                                                                                                   \
        YamlParamType getParamType() const override {                                              \
            return YamlParamType::Name;                                                            \
        }                                                                                          \
    };

class ParameterBase {
public:
    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterObj*, bool e) {
        initialize("default", "parameter", "", e);
    }

    ParameterBase(const sead::SafeString&, const sead::SafeString&, const sead::SafeString&,
                  ParameterList*, bool);

    virtual const char* getParamTypeStr() const = 0;
    virtual YamlParamType getParamType() const = 0;
    virtual const void* ptr() const = 0;
    virtual void* ptr() = 0;
    virtual void afterGetParam();
    virtual s32 size() const = 0;
    virtual bool isEqual(const ParameterBase&);
    virtual bool copy(const ParameterBase&);
    virtual bool copyLerp(const ParameterBase&, const ParameterBase&, f32);

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

    const sead::SafeString& getParamName() const { return mName; }

    u32 getHash() const { return mHash; }

    template <typename T>
    T* getValuePtr() {
        return (T*)ptr();
    }

private:
    ParameterBase* mNext;
    sead::FixedSafeString<0x40> mName;
    u32 mHash;
};

template <typename T>
class Parameter : public ParameterBase {
public:
    // TODO: Add proper parameter names
    Parameter(const sead::SafeString& a, const sead::SafeString& b, const sead::SafeString& c,
              ParameterObj* d, bool e)
        : ParameterBase(a, b, c, d, e) {
        initializeListNode(a, b, c, d, e);
        mValue = T();
    }

    // TODO: Add proper parameter names
    Parameter(const sead::SafeString& a, const sead::SafeString& b, const sead::SafeString& c,
              ParameterList* d, bool e)
        : ParameterBase(a, b, c, d, e) {}

    const void* ptr() const override { return &mValue; };

    void* ptr() override { return &mValue; };

    s32 size() const override { return sizeof(T); }

    const char* getParamTypeStr() const override {
        return YamlParamType::text(YamlParamType::Invalid);
    }

    YamlParamType getParamType() const override { return YamlParamType::Invalid; }

    const T& getValue() const { return mValue; }

    void setValue(const T& value) { mValue = value; }

private:
    T mValue = T();
};

PARAM_TYPE_DEF(Bool, bool)
PARAM_TYPE_DEF(F32, f32)
PARAM_TYPE_DEF(S32, s32)
PARAM_TYPE_DEF(U32, u32)
PARAM_TYPE_DEF(V2f, sead::Vector2f)
PARAM_TYPE_DEF(V3f, sead::Vector3f)
PARAM_TYPE_DEF(V4f, sead::Vector4f)
PARAM_TYPE_DEF(C4f, sead::Color4f)
PARAM_TYPE_DEF(StringRef, const char*)
PARAM_TYPE_DEF(String32, sead::FixedSafeString<32>)
PARAM_TYPE_DEF(String64, sead::FixedSafeString<64>)
PARAM_TYPE_DEF(String128, sead::FixedSafeString<128>)
PARAM_TYPE_DEF(String256, sead::FixedSafeString<256>)
PARAM_TYPE_DEF(String512, sead::FixedSafeString<512>)
PARAM_TYPE_DEF(String1024, sead::FixedSafeString<1024>)
PARAM_TYPE_DEF(String2048, sead::FixedSafeString<2048>)
PARAM_TYPE_DEF(String4096, sead::FixedSafeString<4096>)

}  // namespace al
