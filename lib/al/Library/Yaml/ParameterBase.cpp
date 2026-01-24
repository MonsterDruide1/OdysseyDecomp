#include "Library/Yaml/ParameterBase.h"

#include <codec/seadHashCRC32.h>
#include <math/seadQuat.h>

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

ParameterBase::ParameterBase(const sead::SafeString& name, const sead::SafeString& label,
                             const sead::SafeString& meta, ParameterObj* obj, bool e) {
    initializeListNode(name, label, meta, obj, e);
}

ParameterBase::ParameterBase(const sead::SafeString& name, const sead::SafeString& label,
                             const sead::SafeString& meta, ParameterList* list, bool e) {
    initializeListNode(name, label, meta, list, e);
}

void ParameterBase::afterGetParam() {}

template <>
bool ParameterBase::isEqual_<const char*>(const ParameterBase& parameter) const {
    return isEqualString(getValuePtr<const char>(), parameter.getValuePtr<const char>());
}

template <typename T>
bool ParameterBase::isEqual_(const ParameterBase& parameter) const {
    return *getValuePtr<T>() == *parameter.getValuePtr<T>();
}

bool ParameterBase::isEqual(const ParameterBase& parameter) {
    if ((s32)getParamType() != (s32)parameter.getParamType())
        return false;

    if (mHash != parameter.getHash())
        return false;

    switch (getParamType()) {
    case YamlParamType::Bool:
        return isEqual_<bool>(parameter);

    case YamlParamType::F32:
        return isEqual_<f32>(parameter);

    case YamlParamType::S32:
        return isEqual_<s32>(parameter);

    case YamlParamType::U32:
        return isEqual_<u32>(parameter);

    case YamlParamType::V2f:
        return isEqual_<sead::Vector2f>(parameter);

    case YamlParamType::V2s32:
        return isEqual_<sead::Vector2i>(parameter);

    case YamlParamType::V3f:
        return isEqual_<sead::Vector3f>(parameter);

    case YamlParamType::V4f:
        return isEqual_<sead::Vector4f>(parameter);

    case YamlParamType::Q4f:
        return isEqual_<sead::Quatf>(parameter);

    case YamlParamType::C4f:
        return isEqual_<sead::Color4f>(parameter);

    case YamlParamType::StringRef:
        return isEqual_<const char*>(parameter);

    case YamlParamType::String32:
    case YamlParamType::String64:
    case YamlParamType::String128:
    case YamlParamType::String256:
    case YamlParamType::String512:
    case YamlParamType::String1024:
    case YamlParamType::String2048:
    case YamlParamType::String4096:
        // TODO: Find isEqual_ equivalent
        return isEqualString(getValuePtr<sead::SafeString>()->cstr(),
                             parameter.getValuePtr<sead::SafeString>()->cstr());
    default:
        return false;
    }
}

bool ParameterBase::copy(const ParameterBase& parameter) {
    if ((s32)getParamType() != (s32)parameter.getParamType())
        return false;

    if (mHash != parameter.getHash())
        return false;

    switch (parameter.getParamType()) {
    case YamlParamType::StringRef:
        *getMutableValuePtr<sead::SafeString>() = *parameter.getValuePtr<sead::SafeString>();
        return true;
    default: {
        u8* dest = getMutableValuePtr<u8>();
        const u8* source = parameter.getValuePtr<u8>();
        s32 n = size();
        for (s32 i = 0; i < n; i++) {
            *dest = *source;
            dest++;
            source++;
        }
        return true;
    }
    }
}

template <>
void ParameterBase::copyLerp_<f32>(const ParameterBase& parameterA, const ParameterBase& parameterB,
                                   f32 rate) {
    f32 valueA = *parameterA.getValuePtr<f32>();
    f32 valueB = *parameterB.getValuePtr<f32>();
    setPtrValue(valueA + (valueB - valueA) * rate);
}

template <>
void ParameterBase::copyLerp_<sead::Quatf>(const ParameterBase& parameterA,
                                           const ParameterBase& parameterB, f32 rate) {
    sead::QuatCalcCommon<f32>::slerpTo(*(sead::Quatf*)ptr(), *parameterA.getValuePtr<sead::Quatf>(),
                                       *parameterB.getValuePtr<sead::Quatf>(), rate);
}

// BUG: N's mistake here. This function never returns true
bool ParameterBase::copyLerp(const ParameterBase& parameterA, const ParameterBase& parameterB,
                             f32 rate) {
    if ((s32)getParamType() != (s32)parameterA.getParamType())
        return false;

    if (mHash != parameterA.getHash())
        return false;

    if ((s32)getParamType() != (s32)parameterB.getParamType())
        return false;

    if (mHash != parameterB.getHash())
        return false;

    switch (getParamType()) {
    case YamlParamType::Bool:
    case YamlParamType::S32:
    case YamlParamType::U32:
    case YamlParamType::StringRef:
    case YamlParamType::String32:
    case YamlParamType::String64:
    case YamlParamType::String128:
    case YamlParamType::String256:
    case YamlParamType::String512:
    case YamlParamType::String1024:
    case YamlParamType::String2048:
    case YamlParamType::String4096:
        if (rate >= 0.5f && !(rate == 0.5f))
            copy(parameterB);
        else
            copy(parameterA);
        return false;

    case YamlParamType::F32:
        copyLerp_<f32>(parameterA, parameterB, rate);
        return false;

    // BUG: N's mistake here. This is YamlParamType::V2f
    case YamlParamType::V3f: {
        sead::Vector2f* val = getMutableValuePtr<sead::Vector2f>();
        const sead::Vector2f* valueA = parameterA.getValuePtr<sead::Vector2f>();
        const sead::Vector2f* valueB = parameterB.getValuePtr<sead::Vector2f>();
        val->x = valueA->x + (valueB->x - valueA->x) * rate;
        val->y = valueA->y + (valueB->y - valueA->y) * rate;
        return false;
    }
    // BUG: N's mistake here. This is YamlParamType::V3f
    case YamlParamType::V2f: {
        sead::Vector3f* val = getMutableValuePtr<sead::Vector3f>();
        const sead::Vector3f* valueA = parameterA.getValuePtr<sead::Vector3f>();
        const sead::Vector3f* valueB = parameterB.getValuePtr<sead::Vector3f>();
        val->x = valueA->x + (valueB->x - valueA->x) * rate;
        val->y = valueA->y + (valueB->y - valueA->y) * rate;
        val->z = valueA->z + (valueB->z - valueA->z) * rate;
        return false;
    }
    case YamlParamType::V4f: {
        sead::Vector4f* val = getMutableValuePtr<sead::Vector4f>();
        const sead::Vector4f* valueA = parameterA.getValuePtr<sead::Vector4f>();
        const sead::Vector4f* valueB = parameterB.getValuePtr<sead::Vector4f>();
        val->x = valueA->x + (valueB->x - valueA->x) * rate;
        val->y = valueA->y + (valueB->y - valueA->y) * rate;
        val->z = valueA->z + (valueB->z - valueA->z) * rate;
        val->w = valueA->w + (valueB->w - valueA->w) * rate;
        return false;
    }
    case YamlParamType::Q4f:
        copyLerp_<sead::Quatf>(parameterA, parameterB, rate);
        return false;

    case YamlParamType::C4f:
        getMutableValuePtr<sead::Color4f>()->setLerp(*parameterA.getValuePtr<sead::Color4f>(),
                                                     *parameterB.getValuePtr<sead::Color4f>(),
                                                     rate);
        return false;
    default:
        return false;
    }
}

void ParameterBase::initializeListNode(const sead::SafeString& name, const sead::SafeString& label,
                                       const sead::SafeString& meta, ParameterObj* obj, bool e) {
    initialize(name, label, meta, e);

    if (obj)
        obj->pushBackListNode(this);
}

void ParameterBase::initializeListNode(const sead::SafeString& name, const sead::SafeString& label,
                                       const sead::SafeString& meta, ParameterList* list, bool e) {
    initialize(name, label, meta, e);

    if (list)
        list->addParam(this);
}

void ParameterBase::initialize(const sead::SafeString& name, const sead::SafeString& label,
                               const sead::SafeString& meta, bool e) {
    mNext = nullptr;
    mName = name;
    mHash = calcHash(name);
}

u32 ParameterBase::calcHash(const sead::SafeString& key) {
    return sead::HashCRC32::calcHash(key.cstr(), key.calcLength());
}

// NON-MATCHING: https://decomp.me/scratch/yc1tk
void ParameterBase::tryGetParam(const ByamlIter& iter) {
    getParamTypeStr();
    switch (getParamType().value()) {
    case YamlParamType::Bool: {
        bool value;
        if (tryGetByamlBool(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }
    case YamlParamType::F32: {
        f32 value;
        if (tryGetByamlF32(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }
    case YamlParamType::S32: {
        s32 value;
        if (tryGetByamlS32(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }
    case YamlParamType::U32: {
        u32 value;
        if (tryGetByamlU32(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::V2f: {
        sead::Vector2f value;
        if (tryGetByamlV2f(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::V2s32: {
        sead::Vector2i value;
        if (tryGetByamlV2s32(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::V3f: {
        sead::Vector3f value;
        if (tryGetByamlV3f(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::V4f:
    case YamlParamType::Q4f: {
        sead::Vector4f value;
        if (tryGetByamlV4f(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::C4f: {
        sead::Color4f value;
        if (tryGetByamlColor(&value, iter, mName.cstr()))
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::StringRef: {
        const char* value = tryGetByamlKeyStringOrNULL(iter, mName.cstr());
        if (value)
            setPtrValue(value);
        afterGetParam();
        return;
    }

    case YamlParamType::String32:
    case YamlParamType::String64:
    case YamlParamType::String128:
    case YamlParamType::String256:
    case YamlParamType::String512:
    case YamlParamType::String1024:
    case YamlParamType::String2048:
    case YamlParamType::String4096: {
        const char* value = tryGetByamlKeyStringOrNULL(iter, mName.cstr());
        if (value)
            getMutableValuePtr<sead::BufferedSafeString>()->format("%s", value);

        afterGetParam();
        return;
    }
    default:
        return;
    }
}

ParameterObj::ParameterObj() = default;

void ParameterObj::pushBackListNode(ParameterBase* param) {
    if (!mTailParam) {
        mRootParam = param;
        mTailParam = param;
        return;
    }
    mTailParam->setNext(param);
    mTailParam = param;
}

void ParameterObj::tryGetParam(const ByamlIter& iter) {
    const ByamlIter* i = &iter;
    ByamlIter iterEntry;

    if (!mKey.isEmpty()) {
        iter.tryGetIterByKey(&iterEntry, mKey.cstr());
        i = &iterEntry;
        if (!iterEntry.isValid())
            return;
    }

    for (ParameterBase* paramEntry = mRootParam; paramEntry; paramEntry = paramEntry->getNext())
        paramEntry->tryGetParam(*i);

    for (ParameterArray* arrayEntry = mParamArray; arrayEntry; arrayEntry = arrayEntry->getNext())
        arrayEntry->tryGetParam(*i);
}

void ParameterObj::addArray(ParameterArray* array, const sead::SafeString& key) {
    array->setKey(key);

    if (!mParamArray) {
        mParamArray = array;
        return;
    }

    ParameterArray* arrayEntry = mParamArray;
    while (arrayEntry->getNext())
        arrayEntry = arrayEntry->getNext();
    arrayEntry->setNext(array);
}

bool ParameterObj::isEqual(const ParameterObj& obj) const {
    ParameterBase* paramEntry = mRootParam;
    ParameterBase* param = obj.getRootParam();

    if (!paramEntry) {
        if (param)
            return false;
    } else {
        // BUG: Also succeeds if paramEntry != null and param == null
        while (paramEntry && param) {
            if (!paramEntry->isEqual(*param))
                return false;
            paramEntry = paramEntry->getNext();
            param = param->getNext();
        }
    }

    ParameterArray* arrayEntry = mParamArray;
    ParameterArray* array = obj.getParamArray();
    if (!arrayEntry) {
        if (array)
            return false;
    } else {
        if (!array)
            return false;
        while (arrayEntry && array) {
            if (!arrayEntry->isEqual(*array))
                return false;
            arrayEntry = arrayEntry->getNext();
            array = array->getNext();
        }
    }
    return true;
}

void ParameterObj::copy(const ParameterObj& obj) {
    ParameterBase* paramEntry = mRootParam;
    ParameterBase* param = obj.getRootParam();

    if (paramEntry)
        while (paramEntry && param) {
            paramEntry->copy(*param);
            paramEntry = paramEntry->getNext();
            param = param->getNext();
        }

    ParameterArray* array = obj.getParamArray();
    ParameterArray* arrayEntry = mParamArray;
    if (arrayEntry)
        while (arrayEntry && array) {
            arrayEntry->copy(*array);
            arrayEntry = arrayEntry->getNext();
            array = array->getNext();
        }
}

void ParameterObj::copyLerp(const ParameterObj& objA, const ParameterObj& objB, f32 rate) {
    ParameterBase* paramEntry = mRootParam;
    ParameterBase* paramA = objA.getRootParam();
    ParameterBase* paramB = objB.getRootParam();

    if (paramEntry) {
        if (rate <= 0.0f) {
            while (paramEntry && paramA) {
                paramEntry->copy(*paramA);
                paramEntry = paramEntry->getNext();
                paramA = paramA->getNext();
            }
        } else if (rate >= 1.0f) {
            while (paramEntry && paramB) {
                paramEntry->copy(*paramB);
                paramEntry = paramEntry->getNext();
                paramB = paramB->getNext();
            }
        } else {
            while (paramEntry && paramA && paramB) {
                paramEntry->copyLerp(*paramA, *paramB, rate);
                paramEntry = paramEntry->getNext();
                paramA = paramA->getNext();
                paramB = paramB->getNext();
            }
        }
    }

    ParameterArray* arrayB = objB.getParamArray();
    ParameterArray* arrayA = objA.getParamArray();
    ParameterArray* arrayEntry = mParamArray;
    while (arrayEntry && arrayA && arrayB) {
        arrayEntry->copyLerp(*arrayA, *arrayB, rate);
        arrayEntry = arrayEntry->getNext();
        arrayA = arrayA->getNext();
        arrayB = arrayB->getNext();
    }
}

ParameterBase* ParameterObj::findParameter(const char* name) const {
    for (ParameterBase* paramEntry = mRootParam; paramEntry; paramEntry = paramEntry->getNext())
        if (isEqualString(name, paramEntry->getParamName().getStringTop()))
            return paramEntry;
    return nullptr;
}

ParameterArray::ParameterArray() = default;

void ParameterArray::tryGetParam(const ByamlIter& iter) {
    ByamlIter arrayIter;
    iter.tryGetIterByKey(&arrayIter, mKey.cstr());

    if (!arrayIter.isValid() || !arrayIter.isTypeArray())
        return;

    mSize = arrayIter.getSize();

    s32 index = 0;
    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext()) {
        ByamlIter paramIter;
        arrayIter.tryGetIterByIndex(&paramIter, index);
        if (!paramIter.isValid())
            continue;
        objEntry->tryGetParam(paramIter);
        index++;
    }
}

bool ParameterArray::isEqual(const ParameterArray& array) const {
    if (mSize != array.getSize())
        return false;

    ParameterObj* objEntry = mRootObjNode;
    ParameterObj* obj = array.getRootObjNode();

    if (!objEntry || !obj)
        return !objEntry && !obj;

    while (objEntry && obj) {
        if (!objEntry->isEqual(*obj))
            return false;
        objEntry = objEntry->getNext();
        obj = obj->getNext();
    }
    return true;
}

void ParameterArray::copy(const ParameterArray& array) {
    ParameterObj* obj = array.getRootObjNode();
    ParameterObj* objEntry = mRootObjNode;

    while (objEntry && obj) {
        objEntry->copy(*obj);
        objEntry = objEntry->getNext();
        obj = obj->getNext();
    }
}

void ParameterArray::copyLerp(const ParameterArray& arrayA, const ParameterArray& arrayB,
                              f32 rate) {
    ParameterObj* objBEntry = arrayB.getRootObjNode();
    ParameterObj* objAEntry = arrayA.getRootObjNode();
    ParameterObj* objEntry = mRootObjNode;

    while (objEntry && objAEntry && objBEntry) {
        objEntry->copyLerp(*objAEntry, *objBEntry, rate);
        objEntry = objEntry->getNext();
        objAEntry = objAEntry->getNext();
        objBEntry = objBEntry->getNext();
    }
}

void ParameterArray::addObj(ParameterObj* obj) {
    obj->setKey(sead::SafeString::cEmptyString);

    if (!mRootObjNode) {
        mRootObjNode = obj;
        return;
    }

    ParameterObj* objEntry = mRootObjNode;
    while (objEntry->getNext())
        objEntry = objEntry->getNext();
    objEntry->setNext(obj);
}

void ParameterArray::clearObj() {
    ParameterObj* objEntry = mRootObjNode;
    while (objEntry) {
        ParameterObj* next = objEntry->getNext();
        objEntry->setNext(nullptr);
        objEntry = next;
    }
    mRootObjNode = nullptr;
}

void ParameterArray::removeObj(ParameterObj* obj) {
    ParameterObj* prevObjEntry = nullptr;
    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext()) {
        if (objEntry == obj) {
            if (prevObjEntry)
                prevObjEntry->setNext(obj->getNext());
            else
                mRootObjNode = obj->getNext();
            objEntry->setNext(nullptr);
            return;
        }
        prevObjEntry = objEntry;
    }
}

bool ParameterArray::isExistObj(ParameterObj* obj) {
    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext())
        if (objEntry == obj)
            return true;
    return false;
}

ParameterList::ParameterList() = default;

void ParameterList::addParam(ParameterBase* param) {
    if (!mRootParamNode) {
        mRootParamNode = param;
        return;
    }

    ParameterBase* arrayParam = mRootParamNode;
    while (arrayParam->getNext())
        arrayParam = arrayParam->getNext();
    arrayParam->setNext(param);
}

void ParameterList::addList(ParameterList* list, const sead::SafeString& key) {
    list->setKey(key);

    if (!mRootListNode) {
        mRootListNode = list;
        return;
    }

    ParameterList* listEntry = mRootListNode;
    while (listEntry->getNext())
        listEntry = listEntry->getNext();
    listEntry->setNext(list);
}

void ParameterList::addObj(ParameterObj* obj, const sead::SafeString& key) {
    obj->setKey(key);

    if (!mRootObjNode) {
        mRootObjNode = obj;
        return;
    }

    ParameterObj* objEntry = mRootObjNode;
    while (objEntry->getNext())
        objEntry = objEntry->getNext();
    objEntry->setNext(obj);
}

void ParameterList::addArray(ParameterArray* array, const sead::SafeString& key) {
    array->setKey(key);

    if (!mRootArrayNode) {
        mRootArrayNode = array;
        return;
    }

    ParameterArray* arrayEntry = mRootArrayNode;
    while (arrayEntry->getNext())
        arrayEntry = arrayEntry->getNext();
    arrayEntry->setNext(array);
}

void ParameterList::clearList() {
    ParameterList* listEntry = mRootListNode;
    while (listEntry) {
        ParameterList* next = listEntry->getNext();
        listEntry->setNext(nullptr);
        listEntry = next;
    }
    mRootListNode = nullptr;
}

void ParameterList::clearObj() {
    ParameterObj* objEntry = mRootObjNode;
    while (objEntry) {
        ParameterObj* next = objEntry->getNext();
        objEntry->setNext(nullptr);
        objEntry = next;
    }
    mRootObjNode = nullptr;
}

void ParameterList::removeList(ParameterList* list) {
    ParameterList* prevlistEntry = nullptr;
    for (ParameterList* listEntry = mRootListNode; listEntry; listEntry = listEntry->getNext()) {
        if (listEntry == list) {
            if (prevlistEntry)
                prevlistEntry->setNext(list->getNext());
            else
                mRootListNode = list->getNext();
            listEntry->setNext(nullptr);
            return;
        }
        prevlistEntry = listEntry;
    }
}

void ParameterList::removeObj(ParameterObj* obj) {
    ParameterObj* prevObjEntry = nullptr;
    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext()) {
        if (objEntry == obj) {
            if (prevObjEntry)
                prevObjEntry->setNext(obj->getNext());
            else
                mRootObjNode = obj->getNext();
            objEntry->setNext(nullptr);
            return;
        }
        prevObjEntry = objEntry;
    }
}

bool ParameterList::isExistObj(ParameterObj* obj) {
    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext())
        if (objEntry == obj)
            return true;
    return false;
}

void ParameterList::tryGetParam(const ByamlIter& iter) {
    for (ParameterBase* paramEntry = mRootParamNode; paramEntry; paramEntry = paramEntry->getNext())
        paramEntry->tryGetParam(iter);

    for (ParameterObj* objEntry = mRootObjNode; objEntry; objEntry = objEntry->getNext())
        objEntry->tryGetParam(iter);

    for (ParameterArray* arrayEntry = mRootArrayNode; arrayEntry;
         arrayEntry = arrayEntry->getNext()) {
        arrayEntry->tryGetParam(iter);
    }

    for (ParameterList* listEntry = mRootListNode; listEntry; listEntry = listEntry->getNext())
        listEntry->tryGetParam(iter);
}

ParameterIo::ParameterIo() = default;

}  // namespace al
