#pragma once

namespace al {
struct ExecuteOrder;

struct ExecuteTable {
    const char* mName = nullptr;
    const ExecuteOrder* mExecuteOrders = nullptr;
    s32 mExecuteOrderCount = 0;
};

static_assert(sizeof(ExecuteTable) == 0x18);

}  // namespace al
