#pragma once

#include <basis/seadTypes.h>

namespace al {
class EventFlowChartInitInfo;
class EventFlowNode;

class EventFlowChart {
public:
    EventFlowChart();

    void init(const EventFlowChartInitInfo& info);
    void initAfterPlacement();
    EventFlowNode* findEntryNode(const char* entryName) const;
    EventFlowNode* findNodeById(s32 id) const;
    bool isExistEntry(const char* entryName) const;

private:
    s32 mNodeNum;
    EventFlowNode** mNodes;
    void* mEntryNodeTable;
};

static_assert(sizeof(EventFlowChart) == 0x18);
}  // namespace al
