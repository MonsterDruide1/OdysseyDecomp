#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeNpcMoveToLink : public al::EventFlowNode {
public:
    EventFlowNodeNpcMoveToLink(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;

private:
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Quatf mQuat = sead::Quatf::unit;
    bool mIsValidLink = false;
};

static_assert(sizeof(EventFlowNodeNpcMoveToLink) == 0x88);
