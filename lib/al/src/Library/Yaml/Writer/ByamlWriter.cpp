#include "Library/Yaml/Writer/ByamlWriter.h"

#include <heap/seadHeapMgr.h>
#include <stream/seadStream.h>

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriterBigDataList.h"
#include "Library/Yaml/Writer/ByamlWriterData.h"
#include "Library/Yaml/Writer/ByamlWriterStringTable.h"

namespace al {

ByamlWriter::ByamlWriter(sead::Heap* heap, bool _alwaysFalse)
    : mHeap(heap), _mAlwaysFalse(_alwaysFalse) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mStringTable1 = new ByamlWriterStringTable();
    mStringTable2 = new ByamlWriterStringTable();
    mBigDataList = new ByamlWriterBigDataList();
    mContainerStack = new ByamlWriterContainer*[mContainerStackSize];
}

ByamlWriter::~ByamlWriter() {
    if (_mAlwaysFalse) {
        delete mStringTable1;
        delete mStringTable2;
        delete mBigDataList;
        delete[] mContainerStack;

        for (auto* node : mContainerList) {
            node->deleteData();
        }
        while (auto* node = mContainerList.popBack()) {
            node->mList = nullptr;
            delete node->mData;
            delete node;
        }
    }
}

void ByamlWriter::addBool(bool value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addBool(value);
}
void ByamlWriter::addInt(s32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addInt(value);
}
void ByamlWriter::addUInt(u32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addUInt(value);
}
void ByamlWriter::addFloat(f32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addFloat(value);
}
void ByamlWriter::addInt64(long value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addInt64(value, mBigDataList);
}
void ByamlWriter::addUInt64(u64 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addUInt64(value, mBigDataList);
}
void ByamlWriter::addDouble(double value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addDouble(value, mBigDataList);
}
void ByamlWriter::addString(const char* value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addString(value);
}
void ByamlWriter::addNull() {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addNull();
}

void ByamlWriter::addBool(const char* key, bool value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addBool(key, value);
}
void ByamlWriter::addInt(const char* key, s32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addInt(key, value);
}
void ByamlWriter::addUInt(const char* key, u32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addUInt(key, value);
}
void ByamlWriter::addFloat(const char* key, f32 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addFloat(key, value);
}
void ByamlWriter::addInt64(const char* key, long value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addInt64(key, value, mBigDataList);
}
void ByamlWriter::addUInt64(const char* key, u64 value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addUInt64(key, value, mBigDataList);
}
void ByamlWriter::addDouble(const char* key, double value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addDouble(key, value, mBigDataList);
}
void ByamlWriter::addString(const char* key, const char* value) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addString(key, value);
}
void ByamlWriter::addNull(const char* key) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    mContainerStack[mCurrentContainerIndex]->addNull(key);
}

ByamlWriterContainer* ByamlWriter::getCurrentContainer() {
    return mContainerStack[mCurrentContainerIndex];
}
ByamlWriterArray* ByamlWriter::getArrayCurrentContainer() {
    return static_cast<ByamlWriterArray*>(getCurrentContainer());
}
ByamlWriterHash* ByamlWriter::getHashCurrentContainer() {
    return static_cast<ByamlWriterHash*>(getCurrentContainer());
}

void ByamlWriter::pushContainer(ByamlWriterContainer* container) {
    mCurrentContainerIndex++;
    mContainerStack[mCurrentContainerIndex] = container;
    mContainerList.pushBack(new sead::TListNode<ByamlWriterContainer*>(container));
}
void ByamlWriter::pushHash() {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    auto* hash = new ByamlWriterHash(mStringTable1, mStringTable2);
    if (mCurrentContainerIndex >= 0)
        mContainerStack[mCurrentContainerIndex]->addHash(hash);

    pushContainer(hash);
}
void ByamlWriter::pushArray() {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    auto* array = new ByamlWriterArray(mStringTable2);
    if (mCurrentContainerIndex >= 0)
        mContainerStack[mCurrentContainerIndex]->addArray(array);

    pushContainer(array);
}
void ByamlWriter::pushHash(const char* key) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    auto* hash = new ByamlWriterHash(mStringTable1, mStringTable2);
    mContainerStack[mCurrentContainerIndex]->addHash(key, hash);

    pushContainer(hash);
}
void ByamlWriter::pushArray(const char* key) {
    sead::ScopedCurrentHeapSetter setter{mHeap};
    auto* array = new ByamlWriterArray(mStringTable2);
    mContainerStack[mCurrentContainerIndex]->addArray(key, array);

    pushContainer(array);
}

void ByamlWriter::pushIter(const ByamlIter& iter) {
    pushLocalIter(iter, nullptr);
}
void ByamlWriter::pushIter(const char* key, const ByamlIter& iter) {
    pushLocalIter(iter, key);
}
void ByamlWriter::pushLocalIter(const ByamlIter& iter, const char* iterKey) {
    if (!iter.isValid())
        return;

    s32 size = iter.getSize();
    if (iter.isTypeHash()) {
        if (iterKey)
            pushHash(iterKey);
        else
            pushHash();
    } else if (iter.isTypeArray()) {
        if (iterKey)
            pushArray(iterKey);
        else
            pushArray();
    } else
        return;

    for (s32 i = 0; i < size; i++) {
        ByamlData data{};
        const char* key = nullptr;
        if (iter.isTypeHash())
            iter.getByamlDataAndKeyName(&data, &key, i);
        else
            iter.getByamlDataByIndex(&data, i);

        if (data.getType() == 0xD0) {
            bool value;
            if (iter.tryConvertBool(&value, &data)) {
                if (key)
                    addBool(key, value);
                else
                    addBool(value);
            }
        }
        if (data.getType() == 0xD1) {
            s32 value;
            if (iter.tryConvertInt(&value, &data)) {
                if (key)
                    addInt(key, value);
                else
                    addInt(value);
            }
        }
        if (data.getType() == 0xD2) {
            f32 value;
            if (iter.tryConvertFloat(&value, &data)) {
                if (key)
                    addFloat(key, value);
                else
                    addFloat(value);
            }
        }
        if (data.getType() == 0xD3) {
            u32 value;
            if (iter.tryConvertUInt(&value, &data)) {
                if (key)
                    addUInt(key, value);
                else
                    addUInt(value);
            }
        }
        if (data.getType() == 0xD4) {
            long value;
            if (iter.tryConvertInt64(&value, &data)) {
                if (key)
                    addInt64(key, value);
                else
                    addInt64(value);
            }
        }
        if (data.getType() == 0xD6) {
            double value;
            if (iter.tryConvertDouble(&value, &data)) {
                if (key)
                    addDouble(key, value);
                else
                    addDouble(value);
            }
        }
        if (data.getType() == 0xD5) {
            u64 value;
            if (iter.tryConvertUInt64(&value, &data)) {
                if (key)
                    addUInt64(key, value);
                else
                    addUInt64(value);
            }
        }
        if (data.getType() == 0xA0) {
            const char* value;
            if (iter.tryConvertString(&value, &data)) {
                if (key)
                    addString(key, value);
                else
                    addString(value);
            }
        }
        if (data.getType() == 0x00) {
            if (key)
                addNull(key);
            else
                addNull();
        }
        if (data.getType() == 0xC0 || data.getType() == 0xC1) {
            ByamlIter value;
            if (iter.tryConvertIter(&value, &data))
                pushLocalIter(value, key);
        }
    }
    pop();
}

void ByamlWriter::pop() {
    mCurrentContainerIndex--;
}
u32 ByamlWriter::calcHeaderSize() const {
    return 16;
}
u32 ByamlWriter::calcPackSize() const {
    u32 size = 16;
    if (mStringTable1)
        size += mStringTable1->calcPackSize();
    if (mStringTable2)
        size += mStringTable2->calcPackSize();
    if (mBigDataList)
        size += mBigDataList->calcPackSize();
    for (auto* container : mContainerList) {
        size += container->calcPackSize();
    }
    return size;
}
// NON_MATCHING: offsetBigDataList increased "too early"
void ByamlWriter::write(sead::WriteStream* stream) {
    stream->writeU16(0x4259);
    stream->writeU16(3);

    s32 sizeStringTable1 = mStringTable1->calcPackSize();
    stream->writeU32(sizeStringTable1 > 0 ? 16 : 0);

    u32 offsetStringTable2 = sizeStringTable1 + 16;
    s32 sizeStringTable2 = mStringTable2->calcPackSize();
    stream->writeU32(sizeStringTable2 > 0 ? offsetStringTable2 : 0);

    u32 offsetBigDataList = mBigDataList->setOffset(offsetStringTable2 + sizeStringTable2);
    stream->writeU32(mContainerList.size() > 0 ? offsetBigDataList : 0);
    mStringTable1->write(stream);
    mStringTable2->write(stream);
    mBigDataList->write(stream);

    for (auto* container : mContainerList) {
        container->setOffset(offsetBigDataList);
        offsetBigDataList += container->calcPackSize();
    }
    for (auto* container : mContainerList) {
        container->writeContainer(stream);
    }
}

void ByamlWriter::print() const {
    if (mStringTable1)
        mStringTable1->print();
    if (mStringTable2)
        mStringTable2->print();
    if (mCurrentContainerIndex >= 0) {
        mContainerStack[0]->print(1);
    }
}

}  // namespace al
