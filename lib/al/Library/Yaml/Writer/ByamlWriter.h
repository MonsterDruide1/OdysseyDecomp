#pragma once

#include <container/seadTList.h>

namespace sead {
class Heap;
class WriteStream;
}  // namespace sead

namespace al {

class ByamlWriterStringTable;
class ByamlWriterContainer;
class ByamlWriterBigDataList;

class ByamlWriterArray;
class ByamlWriterHash;

class ByamlIter;

class ByamlWriter {
public:
    ByamlWriter(sead::Heap* heap, bool _alwaysFalse);
    virtual ~ByamlWriter();

    void addBool(bool value);
    void addInt(s32 value);
    void addUInt(u32 value);
    void addFloat(f32 value);
    void addInt64(s64 value);
    void addUInt64(u64 value);
    void addDouble(f64 value);
    void addString(const char* value);
    void addNull();
    void addBool(const char* key, bool value);
    void addInt(const char* key, s32 value);
    void addUInt(const char* key, u32 value);
    void addFloat(const char* key, f32 value);
    void addInt64(const char* key, s64 value);
    void addUInt64(const char* key, u64 value);
    void addDouble(const char* key, f64 value);
    void addString(const char* key, const char* value);
    void addNull(const char* key);

    ByamlWriterArray* getArrayCurrentContainer();
    ByamlWriterHash* getHashCurrentContainer();
    ByamlWriterContainer* getCurrentContainer();
    void pushHash();
    void pushContainer(ByamlWriterContainer* container);
    void pushArray();
    void pushArray(const char* key);
    void pushHash(const char* key);
    void pushIter(const ByamlIter& iter);
    void pushIter(const char* key, const ByamlIter& iter);
    void pushLocalIter(const ByamlIter& iter, const char* iterKey);
    void pop();
    u32 calcHeaderSize() const;
    u32 calcPackSize() const;
    void write(sead::WriteStream* stream);
    void print() const;

private:
    sead::Heap* mHeap;
    ByamlWriterStringTable* mStringTable1 = nullptr;
    ByamlWriterStringTable* mStringTable2 = nullptr;
    sead::TList<ByamlWriterContainer*> mContainerList;
    ByamlWriterBigDataList* mBigDataList = nullptr;
    ByamlWriterContainer** mContainerStack = nullptr;
    s32 mContainerStackSize = 64;
    s32 mCurrentContainerIndex = -1;
    bool mAlwaysFalse;
};

}  // namespace al
