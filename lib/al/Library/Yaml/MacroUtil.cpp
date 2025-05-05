#include "Library/Yaml/MacroUtil.h"

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace alYamlMacroUtil {

IUseYamlParam::IUseYamlParam(const char* name) : mName(name) {
    YamlParamGroup::sCurrent->addParam(this);
}

bool IUseYamlParam::isEqualParamName(const char* name) const {
    return al::isEqualString(name, mName);
}

template <typename T>
void YamlParamGroup::setParamPtr(const char* name, T* value) {
    IUseYamlParam* paramEntry = mHeadParam;
    while (paramEntry) {
        if (paramEntry->isEqualParamName(name)) {
            switch (paramEntry->getClassId()) {
            case YamlClassId::U8:
                paramEntry->setPtr_u8(value);
                break;
            case YamlClassId::U16:
                paramEntry->setPtr_u16(value);
                break;
            case YamlClassId::S16:
                paramEntry->setPtr_s16(value);
                break;
            case YamlClassId::V2f:
                paramEntry->setPtr_V2f(value);
                break;
            case YamlClassId::V3f:
                paramEntry->setPtr_V3f(value);
                break;
            case YamlClassId::Color:
                paramEntry->setPtr_YamlColor(value);
                break;
            case YamlClassId::S32:
                paramEntry->setPtr_s32(value);
                break;
            case YamlClassId::F32:
                paramEntry->setPtr_f32(value);
                break;
            case YamlClassId::Bool:
                paramEntry->setPtr_bool(value);
                break;
            case YamlClassId::String:
                paramEntry->setPtr_YamlString(value);
                break;
            default:
                break;
            }
        }
        paramEntry = paramEntry->getNext();
    }
}

void YamlParamGroup::addParam(IUseYamlParam* param) {
    if (!mHeadParam) {
        mHeadParam = param;
        mTailParam = param;
        return;
    }
    mTailParam->setNext(param);
    mTailParam = param;
}

void YamlParamGroup::readyToSetPtr() {
    IUseYamlParam* paramEntry = mHeadParam;
    while (paramEntry) {
        paramEntry->clearPtr();
        paramEntry = paramEntry->getNext();
    }
}

void YamlParamGroup::readParam(const al::ByamlIter& iter) {
    IUseYamlParam* paramEntry = mHeadParam;
    while (paramEntry) {
        switch (paramEntry->getClassId()) {
        case YamlClassId::U8:
            al::tryGetByamlU8(((YamlParamBase<u8>*)paramEntry)->getParamPtr(), iter,
                              paramEntry->getName());
            break;
        case YamlClassId::U16:
            al::tryGetByamlU16(((YamlParamBase<u16>*)paramEntry)->getParamPtr(), iter,
                               paramEntry->getName());
            break;
        case YamlClassId::S16:
            al::tryGetByamlS16(((YamlParamBase<s16>*)paramEntry)->getParamPtr(), iter,
                               paramEntry->getName());
            break;
        case YamlClassId::V2f:
            al::tryGetByamlV2f(((YamlParamBase<sead::Vector2f>*)paramEntry)->getParamPtr(), iter,
                               paramEntry->getName());
            break;
        case YamlClassId::V3f:
            al::tryGetByamlV3f(((YamlParamBase<sead::Vector3f>*)paramEntry)->getParamPtr(), iter,
                               paramEntry->getName());
            break;
        case YamlClassId::Color:
            al::tryGetByamlColor(((YamlParamBase<sead::Color4f>*)paramEntry)->getParamPtr(), iter,
                                 paramEntry->getName());
            break;
        case YamlClassId::F32:
            al::tryGetByamlF32(((YamlParamBase<f32>*)paramEntry)->getParamPtr(), iter,
                               paramEntry->getName());
            break;
        case YamlClassId::S32: {
            s32 value = al::tryGetByamlKeyIntOrZero(iter, paramEntry->getName());
            ((YamlParamBase<s32>*)paramEntry)->setParam(value);
            break;
        }
        case YamlClassId::Bool: {
            bool value = al::tryGetByamlKeyBoolOrFalse(iter, paramEntry->getName());
            ((YamlParamBase<bool>*)paramEntry)->setParam(value);
            break;
        }
        case YamlClassId::String: {
            const char* value = al::tryGetByamlKeyStringOrNULL(iter, paramEntry->getName());
            ((YamlParamBase<const char*>*)paramEntry)->setParam(value);
        }
        }
        paramEntry = paramEntry->getNext();
    }
}

YamlParam_YamlString::YamlParam_YamlString(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_YamlString::getClassId() const {
    return YamlClassId::String;
}

void YamlParam_YamlString::setPtr_YamlString(const char** value) {
    setParamPtr(value);
}

YamlParam_V3f::YamlParam_V3f(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_V3f::getClassId() const {
    return YamlClassId::V3f;
}

void YamlParam_V3f::setPtr_V3f(sead::Vector3f* value) {
    setParamPtr(value);
}

YamlParam_YamlColor::YamlParam_YamlColor(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_YamlColor::getClassId() const {
    return YamlClassId::Color;
}

void YamlParam_YamlColor::setPtr_YamlColor(sead::Color4f* value) {
    setParamPtr(value);
}

YamlParam_bool::YamlParam_bool(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_bool::getClassId() const {
    return YamlClassId::Bool;
}

void YamlParam_bool::setPtr_bool(bool* value) {
    setParamPtr(value);
}

YamlParam_u8::YamlParam_u8(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_u8::getClassId() const {
    return YamlClassId::U8;
}

void YamlParam_u8::setPtr_u8(u8* value) {
    setParamPtr(value);
}

YamlParam_f32::YamlParam_f32(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_f32::getClassId() const {
    return YamlClassId::F32;
}

void YamlParam_f32::setPtr_f32(f32* value) {
    setParamPtr(value);
}

YamlParam_V2f::YamlParam_V2f(const char* name) : YamlParamBase(name) {}

YamlClassId YamlParam_V2f::getClassId() const {
    return YamlClassId::V2f;
}

void YamlParam_V2f::setPtr_V2f(sead::Vector2f* value) {
    setParamPtr(value);
}

}  // namespace alYamlMacroUtil
