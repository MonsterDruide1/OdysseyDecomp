#include "Project/Draw/GpuMemAllocator.h"

#include <common/aglGPUMemBlock.h>

#include "Library/Base/StringUtil.h"

namespace al {

GpuMemAllocator::GpuMemAllocator() {
    mBlocks.allocBuffer(16, nullptr);
}

GpuMemAllocator::~GpuMemAllocator() {
    for (s32 i = 0; i < mBlocks.size(); i++) {
        Block* block = mBlocks[i];
        if (block->addr.isValid()) {
            block->addr.deleteGPUMemBlock();
            block->addr.invalidate();
        }

        if (block->tmpAddr.isValid()) {
            block->tmpAddr.deleteGPUMemBlock();
            block->tmpAddr.invalidate();
        }
    }
}

void GpuMemAllocator::createMemory(const char* name, s32 size, sead::Heap* heap, s32 alignment,
                                   agl::MemoryAttribute attribute) {
    if (findGpuMemInfo(name))
        return;

    Block* block = new Block;
    s64 longSize = size;
    agl::GPUMemBlockT<u8>* memBlock = new (heap) agl::GPUMemBlockT<u8>;
    memBlock->allocBuffer_(longSize, heap, alignment, attribute);
    block->addr = agl::GPUMemAddrBase(*memBlock, 0);
    block->memorySize = size;
    block->name.format("%s", name);

    mBlocks.pushBack(block);
}

GpuMemAllocator::Block* GpuMemAllocator::findGpuMemInfo(const char* name) const {
    for (s32 i = 0; i < mBlocks.size(); i++) {
        Block* block = mBlocks[i];
        if (isEqualString(name, block->name.cstr()))
            return block;
    }
    return nullptr;
}

void GpuMemAllocator::createMemoryWithTmp(const char* name, s32 size, s32 tmpSize, sead::Heap* heap,
                                          s32 alignment, agl::MemoryAttribute attribute) {
    createMemory(name, size, heap, alignment, attribute);
    Block* block = findGpuMemInfo(name);
    s64 longSize = tmpSize;
    agl::GPUMemBlockT<u8>* memBlock = new (heap) agl::GPUMemBlockT<u8>;
    memBlock->allocBuffer_(longSize, heap, alignment, attribute);
    block->tmpAddr = agl::GPUMemAddrBase(*memBlock, 0);
    block->tmpMemorySize = tmpSize;
}

// NON_MATCHING: different implementation of "if A else B"-arithmetic
agl::GPUMemAddrBase GpuMemAllocator::allocMemory(const char* name, s32 size, s32 alignment) {
    Block* block = findGpuMemInfo(name);
    if (!block)
        return {};

    if (block->usedSize + size > block->memorySize)
        return {};

    s32 unalignedSize;
    if (block->usedSize >= 0)
        unalignedSize = block->usedSize + alignment - 1;
    else
        unalignedSize = block->usedSize - alignment + 1;

    s32 alignedSize = unalignedSize / alignment * alignment;
    block->usedSize = alignedSize + size;
    return {block->addr, alignedSize};
}

agl::GPUMemAddrBase GpuMemAllocator::getTmpMemoryAddr(const char* name) const {
    Block* block = findGpuMemInfo(name);
    if (!block)
        return {};

    block->tmpAddr.invalidateCPUCache(block->tmpMemorySize);
    return {block->tmpAddr, 0};
}

u32 GpuMemAllocator::getTmpMemorySize(const char* name) const {
    Block* block = findGpuMemInfo(name);
    if (!block)
        return 0;

    return block->tmpMemorySize;
}

}  // namespace al
