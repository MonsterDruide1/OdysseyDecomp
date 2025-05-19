#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
class ParameterBase;
class ByamlIter;
class ParameterArray;

class ParameterObj {
public:
    ParameterObj();

    void pushBackListNode(ParameterBase* param);
    void tryGetParam(const ByamlIter& iter);
    void addArray(ParameterArray* array, const sead::SafeStringBase<char>& key);
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
