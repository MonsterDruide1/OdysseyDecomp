#include "Library/Yaml/Writer/ByamlWriterBigDataList.h"

#include "Library/Yaml/Writer/ByamlWriterData.h"

namespace al {

ByamlWriterBigDataList::ByamlWriterBigDataList() = default;
ByamlWriterBigDataList::~ByamlWriterBigDataList() {
    while (auto* node = mList.popBack())
        delete node;
}
void ByamlWriterBigDataList::addData(ByamlWriterBigData* data) {
    mList.pushBack(new sead::TListNode<ByamlWriterBigData*>(data));
}
u32 ByamlWriterBigDataList::calcPackSize() const {
    u32 size = 0;
    for (auto& node : mList)
        size += node->calcBigDataSize();
    return size;
}
s32 ByamlWriterBigDataList::setOffset(s32 offset) {
    for (auto& node : mList) {
        node->setOffset(offset);
        offset += node->calcBigDataSize();
    }
    return offset;
}
void ByamlWriterBigDataList::write(sead::WriteStream* stream) {
    for (auto& node : mList)
        node->writeBigData(stream);
}

}  // namespace al
