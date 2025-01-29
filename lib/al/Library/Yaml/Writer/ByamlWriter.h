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
    ByamlWriter(sead::Heap*, bool);
    virtual ~ByamlWriter();

    void addBool(bool);
    void addInt(s32);
    void addUInt(u32);
    void addFloat(f32);
    void addInt64(s64);
    void addUInt64(u64);
    void addDouble(f64);
    void addString(const char*);
    void addNull();
    void addBool(const char*, bool);
    void addInt(const char*, s32);
    void addUInt(const char*, u32);
    void addFloat(const char*, f32);
    void addInt64(const char*, s64);
    void addUInt64(const char*, u64);
    void addDouble(const char*, f64);
    void addString(const char*, const char*);
    void addNull(const char*);

    ByamlWriterArray* getArrayCurrentContainer();
    ByamlWriterHash* getHashCurrentContainer();
    ByamlWriterContainer* getCurrentContainer();
    void pushHash();
    void pushContainer(ByamlWriterContainer*);
    void pushArray();
    void pushArray(const char*);
    void pushHash(const char*);
    void pushIter(const ByamlIter&);
    void pushIter(const char*, const ByamlIter&);
    void pushLocalIter(const ByamlIter&, const char*);
    void pop();
    u32 calcHeaderSize() const;
    u32 calcPackSize() const;
    void write(sead::WriteStream*);
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
