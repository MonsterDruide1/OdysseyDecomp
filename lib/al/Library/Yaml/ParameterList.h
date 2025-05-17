#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ParameterBase;
class ByamlIter;
class ParameterArray;
class ParameterObj;

class ParameterList {
public:
    ParameterList();

    void addArray(ParameterArray* array, const sead::SafeString& key);
    void addList(ParameterList* list, const sead::SafeString& key);
    void addObj(ParameterObj* obj, const sead::SafeString& key);
    void addParam(ParameterBase* param);
    void clearList();
    void clearObj();
    bool isExistObj(ParameterObj* obj);
    void removeList(ParameterList* list);
    void removeObj(ParameterObj* obj);
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
