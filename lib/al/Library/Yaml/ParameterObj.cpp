#include "Library/Yaml/ParameterObj.h"

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterBase.h"

namespace al {
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
