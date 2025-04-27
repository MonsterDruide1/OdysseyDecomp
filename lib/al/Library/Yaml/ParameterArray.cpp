#include "Library/Yaml/ParameterArray.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterObj.h"

namespace al {

ParameterArray::ParameterArray() = default;

void ParameterArray::tryGetParam(const ByamlIter& iter) {
    ByamlIter arrayIter;
    iter.tryGetIterByKey(&arrayIter, mParamObjKey.cstr());

    if (arrayIter.isValid() && arrayIter.isTypeArray()) {
        mSize = arrayIter.getSize();

        ParameterObj* arrayObj = mFirstObj;
        s32 index = 0;
        while (arrayObj) {
            ByamlIter paramIter;
            arrayIter.tryGetIterByIndex(&paramIter, index);
            if (paramIter.isValid()) {
                arrayObj->tryGetParam(paramIter);
                index++;
            }
            arrayObj = arrayObj->getNext();
        }
    }
}

bool ParameterArray::isEqual(const ParameterArray& array) const {
    if (mSize != array.getSize())
        return false;

    ParameterObj* arrayObj = mFirstObj;
    ParameterObj* obj = array.getFirstObj();

    if (arrayObj == nullptr || obj == nullptr)
        return arrayObj == nullptr && obj == nullptr;

    while (arrayObj && obj) {
        if (!arrayObj->isEqual(*obj))
            return false;
        arrayObj = arrayObj->getNext();
        obj = obj->getNext();
    }
    return true;
}

void ParameterArray::copy(const ParameterArray& array) {
    ParameterObj* obj = array.getFirstObj();
    ParameterObj* arrayObj = mFirstObj;

    while (arrayObj && obj) {
        arrayObj->copy(*obj);
        arrayObj = arrayObj->getNext();
        obj = obj->getNext();
    }
}

void ParameterArray::copyLerp(const ParameterArray& arrayA, const ParameterArray& arrayB,
                              f32 value) {
    ParameterObj* arrayObjB = arrayB.getFirstObj();
    ParameterObj* arrayObjA = arrayA.getFirstObj();
    ParameterObj* arrayObj = mFirstObj;

    while (arrayObj && arrayObjA && arrayObjB) {
        arrayObj->copyLerp(*arrayObjA, *arrayObjB, value);
        arrayObj = arrayObj->getNext();
        arrayObjA = arrayObjA->getNext();
        arrayObjB = arrayObjB->getNext();
    }
}

void ParameterArray::addObj(ParameterObj* obj) {
    obj->setParamObjKey(sead::SafeString::cEmptyString);

    if (!mFirstObj) {
        mFirstObj = obj;
        return;
    }

    ParameterObj* arrayObj = mFirstObj;
    while (arrayObj->getNext())
        arrayObj = arrayObj->getNext();
    arrayObj->setNext(obj);
}

void ParameterArray::clearObj() {
    ParameterObj* arrayObj = mFirstObj;
    while (arrayObj) {
        ParameterObj* next = arrayObj->getNext();
        arrayObj->setNext(nullptr);
        arrayObj = next;
    }
    mFirstObj = nullptr;
}

void ParameterArray::removeObj(ParameterObj* obj) {
    ParameterObj* arrayObj = mFirstObj;
    ParameterObj* prevArrayObj = nullptr;
    while (arrayObj) {
        if (arrayObj == obj) {
            if (prevArrayObj) {
                prevArrayObj->setNext(obj->getNext());
                arrayObj->setNext(nullptr);
            } else {
                mFirstObj = obj->getNext();
                obj->setNext(nullptr);
            }
            return;
        }
        prevArrayObj = arrayObj;
        arrayObj = arrayObj->getNext();
    }
}

bool ParameterArray::isExistObj(ParameterObj* obj) {
    ParameterObj* arrayObj = mFirstObj;
    while (arrayObj) {
        if (arrayObj == obj)
            return true;
        arrayObj = arrayObj->getNext();
    }
    return false;
}

}  // namespace al
