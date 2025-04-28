#include "Library/Yaml/ParameterList.h"

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterArray.h"
#include "Library/Yaml/ParameterBase.h"
#include "Library/Yaml/ParameterObj.h"

namespace al {

ParameterList::ParameterList() = default;

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

bool ParameterList::isExistObj(ParameterObj* obj) {
    ParameterObj* objEntry = mRootObjNode;
    while (objEntry) {
        if (objEntry == obj)
            return true;
        objEntry = objEntry->getNext();
    }
    return false;
}

void ParameterList::removeList(ParameterList* list) {
    ParameterList* listEntry = mRootListNode;
    ParameterList* prevlistEntry = nullptr;
    while (listEntry) {
        if (listEntry == list) {
            if (prevlistEntry) {
                prevlistEntry->setNext(list->getNext());
                listEntry->setNext(nullptr);
            } else {
                mRootListNode = list->getNext();
                list->setNext(nullptr);
            }
            return;
        }
        prevlistEntry = listEntry;
        listEntry = listEntry->getNext();
    }
}

void ParameterList::removeObj(ParameterObj* obj) {
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
