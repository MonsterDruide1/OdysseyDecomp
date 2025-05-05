#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;
}  // namespace al

namespace alYamlMacroUtil {

enum class YamlClassId : s32 {
    U8 = 0x53,
    U16 = 0x54,
    S16 = 0x55,
    V2f = 0x56,
    V3f = 0x57,
    Color = 0x58,
    F32 = 0x59,
    S32 = 0x5a,
    Bool = 0x5b,
    String = 0x5c,
};

class IUseYamlParam {
public:
    IUseYamlParam(const char* name);

    virtual void clearPtr() = 0;
    virtual YamlClassId getClassId() const = 0;
    virtual bool isValidPtr() const = 0;

    virtual void setPtr_u8(u8* value) {}

    virtual void setPtr_u16(u16* value) {}

    virtual void setPtr_s16(s16* value) {}

    virtual void setPtr_V2f(sead::Vector2f* value) {}

    virtual void setPtr_V3f(sead::Vector3f* value) {}

    virtual void setPtr_YamlColor(sead::Color4f* value) {}

    virtual void setPtr_s32(s32* value) {}

    virtual void setPtr_f32(f32* value) {}

    virtual void setPtr_bool(bool* value) {}

    virtual void setPtr_YamlString(const char** value) {}

    bool isEqualParamName(const char* name) const;

    const char* getName() const { return mName; }

    IUseYamlParam* getNext() const { return mNext; }

    void setNext(IUseYamlParam* param) { mNext = param; }

private:
    const char* mName;
    IUseYamlParam* mNext = nullptr;
};

class YamlParamGroup {
public:
    YamlParamGroup() = default;

    template <typename T>
    void setParamPtr(const char* name, T* value);

    void addParam(IUseYamlParam* param);
    void readyToSetPtr();
    void readParam(const al::ByamlIter& iter);

    static YamlParamGroup* sCurrent;
private:
    IUseYamlParam* mHeadParam = nullptr;
    IUseYamlParam* mTailParam = nullptr;
};

template <typename T>
class YamlParamBase : public IUseYamlParam {
public:
    YamlParamBase(const char* name);

    T* getParamPtr() { return mValue; }

    void setParamPtr(T* value) { mValue = value; }

    void setParam(T value) { *mValue = value; }

    void clearPtr() override { mValue = nullptr; }

    bool isValidPtr() const override { return mValue != nullptr; }

private:
    T* mValue;
};

class YamlParam_YamlString : public YamlParamBase<const char*> {
public:
    YamlParam_YamlString(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_YamlString(const char** value) override;
};

class YamlParam_V3f : public YamlParamBase<sead::Vector3f> {
public:
    YamlParam_V3f(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_V3f(sead::Vector3f* value) override;
};

class YamlParam_YamlColor : public YamlParamBase<sead::Color4f> {
public:
    YamlParam_YamlColor(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_YamlColor(sead::Color4f* value) override;
};

class YamlParam_bool : public YamlParamBase<bool> {
public:
    YamlParam_bool(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_bool(bool* value) override;
};

class YamlParam_u8 : public YamlParamBase<u8> {
public:
    YamlParam_u8(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_u8(u8* value) override;
};

class YamlParam_f32 : public YamlParamBase<f32> {
public:
    YamlParam_f32(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_f32(f32* value) override;
};

class YamlParam_V2f : public YamlParamBase<sead::Vector2f> {
public:
    YamlParam_V2f(const char* name);

    YamlClassId getClassId() const override;
    void setPtr_V2f(sead::Vector2f* value) override;
};

}  // namespace alYamlMacroUtil
