#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckPlayingCollectBgm : public al::EventFlowNode {
public:
    EventFlowNodeCheckPlayingCollectBgm(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;

private:
    const char* mRequest = nullptr;
};

static_assert(sizeof(EventFlowNodeCheckPlayingCollectBgm) == 0x70);
