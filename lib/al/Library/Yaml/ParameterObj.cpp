#include "Library/Yaml/ParameterObj.h"

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterArray.h"
#include "Library/Yaml/ParameterBase.h"

namespace al {
ParameterObj::ParameterObj() = default;

void ParameterObj::pushBackListNode(ParameterBase* param) {
    if (mTailParam) {
        mTailParam->setNext(param);
        mTailParam = param;
        return;
    }
    mRootParam = param;
    mTailParam = param;
}

// NON_MATCHING: Invalid write to const param? https://decomp.me/scratch/X8WDX
void ParameterObj::tryGetParam(const ByamlIter& iter) {
    ByamlIter iterEntry;

    if (!mKey.isEmpty()) {
        iter.tryGetIterByKey(&iterEntry, mKey.cstr());
        // iter = iterEntry;
        if (!iterEntry.isValid())
            return;
    }

    for (ParameterBase* paramEntry = mRootParam; paramEntry; paramEntry = paramEntry->getNext())
        paramEntry->tryGetParam(iter);

    for (ParameterArray* arrayEntry = mParamArray; arrayEntry; arrayEntry = arrayEntry->getNext())
        arrayEntry->tryGetParam(iter);
}

void ParameterObj::addArray(ParameterArray* array, const sead::SafeStringBase<char>& key) {
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
        while (paramEntry && param) {
            if (!paramEntry->isEqual(param))
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
            // Start Inline array isEqual??? if(!arrayEntry->isEqual(*array)) return false;
            if (arrayEntry->getSize() != array->getSize())
                return false;
            ParameterObj* objEntry = arrayEntry->getRootObjNode();
            ParameterObj* obj = array->getRootObjNode();
            if (!objEntry || !obj) {
                if (objEntry || obj)
                    return false;
            } else {
                while (objEntry && obj) {
                    if (!objEntry->isEqual(*obj))
                        return false;
                    objEntry = objEntry->getNext();
                    obj = obj->getNext();
                }
            }
            // End Inline array isEqual
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
            paramEntry->copy(param);
            paramEntry = paramEntry->getNext();
            param = param->getNext();
        }

    ParameterArray* array = obj.getParamArray();
    ParameterArray* arrayEntry = mParamArray;
    while (arrayEntry && array) {
        // Start Inline array copy??? objEntry->copy(*array);
        ParameterObj* obj = array->getRootObjNode();
        ParameterObj* objEntry = arrayEntry->getRootObjNode();

        while (objEntry && obj) {
            objEntry->copy(*obj);
            objEntry = objEntry->getNext();
            obj = obj->getNext();
        }
        // End Inline array copy
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
                paramEntry->copy(paramA);
                paramEntry = paramEntry->getNext();
                paramA = paramA->getNext();
            }
        } else if (rate >= 1.0f) {
            while (paramEntry && paramB) {
                paramEntry->copy(paramB);
                paramEntry = paramEntry->getNext();
                paramB = paramB->getNext();
            }
        } else {
            while (paramEntry && paramA && paramB) {
                paramEntry->copyLerp(paramA, paramB, rate);
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
        // Start Inline array copyLerp??? arrayEntry->copy(*arrayA,*arrayB, rate);
        ParameterObj* objBEntry = arrayB->getRootObjNode();
        ParameterObj* objAEntry = arrayA->getRootObjNode();
        ParameterObj* objEntry = arrayEntry->getRootObjNode();

        while (objEntry && objAEntry && objBEntry) {
            objEntry->copyLerp(*objAEntry, *objBEntry, rate);
            objEntry = objEntry->getNext();
            objAEntry = objAEntry->getNext();
            objBEntry = objBEntry->getNext();
        }
        // End Inline array copyLerp
        arrayEntry = arrayEntry->getNext();
        arrayA = arrayA->getNext();
        arrayB = arrayB->getNext();
    }
}

// NON_MATCHING: Doesn't use cstr() https://decomp.me/scratch/JwVLn
ParameterBase* ParameterObj::findParameter(const char* name) const {
    ParameterBase* paramEntry = mRootParam;
    while (paramEntry && !isEqualString(name, paramEntry->getParamName().mBuffer))
        paramEntry = paramEntry->getNext();
    return paramEntry;
}

}  // namespace al
