#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class RootSercherNode {
public:
    RootSercherNode();

    void clear();
    void open();
    void close();
    u32 calcTotalCost() const;
};

class RootSercher : public HioNode {
public:
    RootSercher();

    void init(s32, s32);
    void clear();
    RootSercherNode* tryGetNode(s32, s32) const;
    RootSercherNode* getNode(s32, s32) const;
    RootSercherNode* findMinCostOpenNode() const;
    void closeNode(RootSercherNode*, s32, s32);
    void removeOpenList(RootSercherNode*);
    void openNode(s32, s32, s32, s32, RootSercherNode*);
    bool isEnableNode(s32, s32) const;
    void addOpenList(RootSercherNode*);
    bool start(s32, s32, s32, s32, s32, s32);
    u32 calcNextIndex(s32*, s32*, s32, s32) const;

    virtual u32 calcHeuristicCost(s32, s32, s32, s32) const;
    virtual u32 getConnectCount() const;
    virtual u32 getConnectIndex(s32*, s32*, s32, s32, s32) const;
    virtual bool checkReach(s32, s32) const;
    virtual u32 calcCost(s32, s32, const RootSercherNode*) const;
};

}  // namespace al
