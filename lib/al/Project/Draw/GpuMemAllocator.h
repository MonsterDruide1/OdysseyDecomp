#pragma once

#include <common/aglGPUCommon.hpp>
#include <common/aglGPUMemAddr.h>
#include <heap/seadHeap.h>

namespace al {

class GpuMemAllocator {
public:
    struct Block {
        agl::GPUMemAddrBase addr;
        agl::GPUMemAddrBase tmpAddr;
        sead::FixedSafeString<32> name = {""};
        s32 usedSize = 0;
        s32 memorySize = 0;
        s32 tmpMemorySize = 0;
    };

    GpuMemAllocator();
    ~GpuMemAllocator();

    void createMemory(const char* name, s32 size, sead::Heap* heap, s32 alignment,
                      agl::MemoryAttribute attribute);
    Block* findGpuMemInfo(const char* name) const;
    void createMemoryWithTmp(const char* name, s32 size, s32 tmpSize, sead::Heap* heap,
                             s32 alignment, agl::MemoryAttribute attribute);
    agl::GPUMemAddrBase allocMemory(const char* name, s32 size, s32 alignment);
    agl::GPUMemAddrBase getTmpMemoryAddr(const char* name) const;
    u32 getTmpMemorySize(const char* name) const;

private:
    sead::PtrArray<Block> mBlocks;
};

static_assert(sizeof(GpuMemAllocator::Block) == 0x78);
static_assert(sizeof(GpuMemAllocator) == 0x10);

}  // namespace al
