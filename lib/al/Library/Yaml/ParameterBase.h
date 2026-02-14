#pragma once

#include <gfx/seadColor.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>
#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class ParameterArray;
class ParameterList;
class ParameterObj;

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

// TODO: rename parameter bool e in constructor
#define PARAM_TYPE_DEF(Name, Type)                                                                 \
    class Parameter##Name : public Parameter<Type> {                                               \
    public:                                                                                        \
        Parameter##Name(const sead::SafeString& name, const sead::SafeString& label,               \
                        const sead::SafeString& meta, ParameterObj* obj, bool e)                   \
            : Parameter(name, label, meta, obj, e) {}                                              \
                                                                                                   \
        Parameter##Name(Type const& value, const sead::SafeString& name,                           \
                        const sead::SafeString& label, const sead::SafeString& meta,               \
                        ParameterObj* obj, bool e)                                                 \
            : Parameter(value, name, label, meta, obj, e) {}                                       \
                                                                                                   \
        Parameter##Name(const sead::SafeString& name, const sead::SafeString& label,               \
                        const sead::SafeString& meta, ParameterList* list, bool e)                 \
            : Parameter(name, label, meta, list, e) {}                                             \
                                                                                                   \
        Parameter##Name(Type const& value, const sead::SafeString& name,                           \
                        const sead::SafeString& label, const sead::SafeString& meta,               \
                        ParameterList* list, bool e)                                               \
            : Parameter(value, name, label, meta, list, e) {}                                      \
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
    static u32 calcHash(const sead::SafeString& key);

    // TODO: rename parameter bool e in all functions
    ParameterBase(bool e) { initialize("default", "parameter", "", e); }

    ParameterBase(const sead::SafeString& name, const sead::SafeString& label,
                  const sead::SafeString& meta, ParameterObj* obj, bool e);

    ParameterBase(const sead::SafeString& name, const sead::SafeString& label,
                  const sead::SafeString& meta, ParameterList* list, bool e);

    virtual const char* getParamTypeStr() const = 0;
    virtual YamlParamType getParamType() const = 0;
    virtual const void* ptr() const = 0;
    virtual void* ptr() = 0;
    virtual void afterGetParam();
    virtual s32 size() const = 0;
    virtual bool isEqual(const ParameterBase& parameter);
    virtual bool copy(const ParameterBase& parameter);
    virtual bool copyLerp(const ParameterBase& parameterA, const ParameterBase& parameterB,
                          f32 rate);

    void initializeListNode(const sead::SafeString& name, const sead::SafeString& label,
                            const sead::SafeString& meta, ParameterObj* obj, bool e);
    void initializeListNode(const sead::SafeString& name, const sead::SafeString& label,
                            const sead::SafeString& meta, ParameterList* list, bool e);
    void initialize(const sead::SafeString& name, const sead::SafeString& label,
                    const sead::SafeString& meta, bool e);
    void tryGetParam(const ByamlIter& iter);

    ParameterBase* getNext() const { return mNext; }

    void setNext(ParameterBase* param) { mNext = param; }

    const sead::SafeString& getParamName() const { return mName; }

    u32 getHash() const { return mHash; }

    template <typename T>
    T* getMutableValuePtr() {
        return reinterpret_cast<T*>(ptr());
    }

    template <typename T>
    const T* getValuePtr() const {
        return reinterpret_cast<const T*>(ptr());
    }

    template <typename T>
    void setPtrValue(T value) {
        *getMutableValuePtr<T>() = value;
    }

private:
    template <typename T>
    bool isEqual_(const ParameterBase& parameter) const;

    template <typename T>
    void copyLerp_(const ParameterBase& parameterA, const ParameterBase& parameterB, f32 rate);

private:
    ParameterBase* mNext;
    sead::FixedSafeString<0x40> mName;
    u32 mHash;
};

template <typename T>
class Parameter : public ParameterBase {
public:
    // TODO: rename parameter bool e in constructor
    Parameter(const sead::SafeString& name, const sead::SafeString& label,
              const sead::SafeString& meta, ParameterObj* obj, bool e)
        : ParameterBase(e) {
        initializeListNode(name, label, meta, obj, e);
        mValue = T();
    }

    Parameter(const T& value, const sead::SafeString& name, const sead::SafeString& label,
              const sead::SafeString& meta, ParameterObj* obj, bool e)
        : ParameterBase(e) {
        initializeListNode(name, label, meta, obj, e);
        mValue = value;
    }

    Parameter(const sead::SafeString& name, const sead::SafeString& label,
              const sead::SafeString& meta, ParameterList* list, bool e)
        : ParameterBase(e) {
        initializeListNode(name, label, meta, list, e);
        mValue = T();
    }

    Parameter(const T& value, const sead::SafeString& name, const sead::SafeString& label,
              const sead::SafeString& meta, ParameterList* list, bool e)
        : ParameterBase(e) {
        initializeListNode(name, label, meta, list, e);
        mValue = value;
    }

    const void* ptr() const override { return &mValue; };

    void* ptr() override { return &mValue; };

    s32 size() const override { return sizeof(T); }

    const char* getParamTypeStr() const override {
        return YamlParamType::text(YamlParamType::Invalid);
    }

    YamlParamType getParamType() const override { return YamlParamType::Invalid; }

    const T& getValue() const { return mValue; }

    void setValue(const T& value) { mValue = value; }

    T* operator->() { return &mValue; }

    const T* operator->() const { return &mValue; }

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
PARAM_TYPE_DEF(Q4f, sead::Quatf)
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

class ParameterObj {
public:
    ParameterObj();

    void pushBackListNode(ParameterBase* param);
    void tryGetParam(const ByamlIter& iter);
    void addArray(ParameterArray* array, const sead::SafeString& key);
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

class ParameterList {
public:
    ParameterList();

    void addParam(ParameterBase* param);
    void addList(ParameterList* list, const sead::SafeString& key);
    void addObj(ParameterObj* obj, const sead::SafeString& key);
    void addArray(ParameterArray* array, const sead::SafeString& key);
    void clearList();
    void clearObj();
    void removeList(ParameterList* list);
    void removeObj(ParameterObj* obj);
    bool isExistObj(ParameterObj* obj);
    void tryGetParam(const ByamlIter& iter);

    ParameterList* getNext() const { return mNext; }

    void setNext(ParameterList* list) { mNext = list; }

    void setKey(const sead::SafeString& key) { mKey = key; }

private:
    ParameterBase* mRootParamNode = nullptr;
    ParameterObj* mRootObjNode = nullptr;
    ParameterList* mRootListNode = nullptr;
    ParameterArray* mRootArrayNode = nullptr;
    ParameterList* mNext = nullptr;
    sead::FixedSafeString<0x40> mKey;
};

class ParameterIo : public ParameterList {
public:
    ParameterIo();
};

}  // namespace al
