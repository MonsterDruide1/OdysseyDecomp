#pragma once

#include <basis/seadTypes.h>
#include <heap/seadHeap.h>

namespace al {

void copyMemoryFast(u32*, const u32*, u32);
void copyMemory(void*, const void*, u32);
bool tryCompressByZlib(u8*, u32*, const u8*, u32);
bool tryDecompressByZlib(u8*, u32*, const u8*, u32);
sead::Heap* getCurrentHeap();

}  // namespace al
