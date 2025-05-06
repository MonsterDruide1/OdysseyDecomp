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

    void createMemory(const char*, s32, sead::Heap*, s32, agl::MemoryAttribute);
    Block* findGpuMemInfo(const char*) const;
    void createMemoryWithTmp(const char*, s32, s32, sead::Heap*, s32, agl::MemoryAttribute);
    agl::GPUMemAddrBase allocMemory(const char*, s32, s32);
    agl::GPUMemAddrBase getTmpMemoryAddr(const char*) const;
    u32 getTmpMemorySize(const char*) const;

private:
    sead::PtrArray<Block> mBlocks;
};

static_assert(sizeof(GpuMemAllocator::Block) == 0x78);
static_assert(sizeof(GpuMemAllocator) == 0x10);

}  // namespace al
