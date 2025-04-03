#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ExecuteOrder;

struct ExecuteTable {
    const char* name = nullptr;
    const ExecuteOrder* executeOrders = nullptr;
    s32 executeOrderCount = 0;
};

static_assert(sizeof(ExecuteTable) == 0x18);

}  // namespace al
