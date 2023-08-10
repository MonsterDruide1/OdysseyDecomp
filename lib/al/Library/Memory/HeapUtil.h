#pragma once

#include <heap/seadHeap.h>

namespace al {
sead::Heap* getStationedHeap();
sead::Heap* getCurrentHeap();
}  // namespace al
