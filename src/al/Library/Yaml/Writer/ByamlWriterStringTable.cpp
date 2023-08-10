#include "al/Library/Yaml/Writer/ByamlWriterStringTable.h"

#include <cstring>
#include <stream/seadStream.h>
#include "al/Library/Yaml/ByamlUtil.h"

namespace al {

ByamlWriterStringTable::ByamlWriterStringTable() = default;
ByamlWriterStringTable::~ByamlWriterStringTable() {
    while (auto* node = mList.popBack()) {
        node->mList = nullptr;
        delete[] node->mData;
        delete node;
    }
}
inline char* add(const char* string, sead::TList<const char*> list) {
    int length = ((strlen(string) << 32) + 0x100000000LL) >> 32;
    char* array = new char[length];
    strncpy(array, string, length);
    auto* node = new sead::TListNode<const char*>(array);
    list.pushBack(node);
    return array;
}
const char* ByamlWriterStringTable::tryAdd(
    const char* string) {  // TODO major mismatches, probably due to the inlined functions way of
                           // adding things to `list`.
    for (auto& node : mList) {
        int result = strcmp(string, node);
        if (result == 0)
            return node;
        if (result < 0)
            return add(string, mList);
    }
    return add(string, mList);
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
    int i = 0;
    for (auto& node : mList) {
        if (!strcmp(data, node))
            return i;
        i++;
    }
    return -1;
}
void ByamlWriterStringTable::write(
    sead::WriteStream* stream) const {  // TODO minor mismatch during first `for`-loop
    if (isEmpty())
        return;
    stream->writeU8(0xC2);
    alByamlLocalUtil::writeU24(stream, mList.size());

    int i = 4 * (mList.size() + 2);
    for (auto& node : mList) {
        stream->writeU32(i);
        i += strlen(node) + 1;
    }
    for (auto& node : mList) {
        stream->writeMemBlock(node, strlen(node));
        stream->writeU8(0);
    }

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
