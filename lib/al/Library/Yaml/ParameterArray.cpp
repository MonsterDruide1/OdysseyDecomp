#include "Library/Yaml/ParameterArray.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterObj.h"

namespace al {

ParameterArray::ParameterArray() = default;

void ParameterArray::tryGetParam(const ByamlIter& iter) {
    ByamlIter arrayIter;
    iter.tryGetIterByKey(&arrayIter, mKey.cstr());

    if (arrayIter.isValid() && arrayIter.isTypeArray()) {
        mSize = arrayIter.getSize();

        ParameterObj* objEntry = mRootObjNode;
        s32 index = 0;
        while (objEntry) {
            ByamlIter paramIter;
            arrayIter.tryGetIterByIndex(&paramIter, index);
            if (paramIter.isValid()) {
                objEntry->tryGetParam(paramIter);
                index++;
            }
            objEntry = objEntry->getNext();
        }
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
    ParameterObj* objEntry = mRootObjNode;
    ParameterObj* prevObjEntry = nullptr;
    while (objEntry) {
        if (objEntry == obj) {
            if (prevObjEntry) {
                prevObjEntry->setNext(obj->getNext());
                objEntry->setNext(nullptr);
            } else {
                mRootObjNode = obj->getNext();
                obj->setNext(nullptr);
            }
            return;
        }
        prevObjEntry = objEntry;
        objEntry = objEntry->getNext();
    }
}

bool ParameterArray::isExistObj(ParameterObj* obj) {
    ParameterObj* objEntry = mRootObjNode;
    while (objEntry) {
        if (objEntry == obj)
            return true;
        objEntry = objEntry->getNext();
    }
    return false;
}

}  // namespace al
