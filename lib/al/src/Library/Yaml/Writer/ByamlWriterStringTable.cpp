#include "Library/Yaml/Writer/ByamlWriterStringTable.h"

#include <stream/seadStream.h>

#include "Library/Yaml/ByamlUtil.h"

namespace al {

ByamlWriterStringTable::ByamlWriterStringTable() = default;
ByamlWriterStringTable::~ByamlWriterStringTable() {
    while (auto* node = mList.popBack()) {
        node->mList = nullptr;
        delete[] node->mData;
        delete node;
    }
}
inline char* add(const char* string, sead::TList<const char*>& list) {
    s32 length = ((strlen(string) << 32) + 0x100000000LL) >> 32;
    char* array = new char[length];
    strncpy(array, string, length);
    auto* node = new sead::TListNode<const char*>(array);
    list.pushBack(node);
    return array;
}
const char* ByamlWriterStringTable::tryAdd(const char* string) {
    for (auto it = mList.robustBegin(); it != mList.robustEnd(); ++it) {
        int result = strcmp(string, it->mData);
        if (result == 0) {
            return it->mData;
        }
        if (result < 0) {
            s32 length = (s64)((strlen(string) << 32) + 0x100000000LL) >> 32;
            char* array = new char[length];
            char* result = strncpy(array, string, length);
            auto* node = new sead::TListNode<const char*>(result);
            node->mList = &mList;
            mList.insertBefore(&*it, node);
            return result;
        }
    }
    s32 length = (s64)((strlen(string) << 32) + 0x100000000LL) >> 32;
    char* array = new char[length];
    char* result = strncpy(array, string, length);
    auto* node = new sead::TListNode<const char*>(result);
    mList.pushBack(node);
    return result;
}
u32 ByamlWriterStringTable::calcHeaderSize() const {
    return (4 * mList.size()) + 8;
}
u32 ByamlWriterStringTable::calcContentSize() const {
    u32 size = 0;
    for (auto& node : mList) {
        size += strlen(node) + 1;
    }
    return (size + 3) & 0xFFFFFFFC;
}
u32 ByamlWriterStringTable::calcPackSize() const {
    if (isEmpty())
        return 0;
    return calcHeaderSize() + calcContentSize();
}
bool ByamlWriterStringTable::isEmpty() const {
    return mList.size() == 0;
}
u32 ByamlWriterStringTable::calcIndex(const char* data) const {
    s32 i = 0;
    for (auto& node : mList) {
        if (!strcmp(data, node))
            return i;
        i++;
    }
    return -1;
}
void ByamlWriterStringTable::write(sead::WriteStream* stream) const {
    if (isEmpty())
        return;
    stream->writeU8(0xC2);
    alByamlLocalUtil::writeU24(stream, mList.size());

    s32 i = 4 * (mList.size() + 2);

    int v7 = 0;
    auto end = mList.end();
    for (auto it = mList.begin();; i += v7 + 1, ++it) {
        stream->writeU32(i);
        if (it == end)
            break;
        v7 = strlen(*it);
    }

    for (auto node : mList) {
        stream->writeMemBlock(node, strlen(node));
        stream->writeU8(0);
    }

    // calculate padding
    s32 v12 = i < 0 ? i + 3 : i;
    s32 v14 = i - (v12 & 0xFFFFFFFC);
    s32 v15 = 4 - v14;
    s32 v16 = v14 == 0 ? 0 : v15;

    for (s32 j = 0; j < v16; j++) {
        stream->writeU8(0);
    }
}
void ByamlWriterStringTable::print() const {
    for (auto& node : mList) {
        ;
    }
}

}  // namespace al
