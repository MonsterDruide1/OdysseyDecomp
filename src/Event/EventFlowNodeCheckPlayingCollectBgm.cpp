#include "Event/EventFlowNodeCheckPlayingCollectBgm.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/CollectBgm.h"

EventFlowNodeCheckPlayingCollectBgm::EventFlowNodeCheckPlayingCollectBgm(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckPlayingCollectBgm::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);

    al::tryGetStringArg(&mRequest, *al::getActorInitInfo(info), "Request");
}

s32 EventFlowNodeCheckPlayingCollectBgm::getNextId() const {
    if (mRequest != nullptr && al::isPlayingBgm(getActor(), "CollectBgm")) {
        const char* currentBgmResourceName = al::getCurPlayingBgmResourceName(getActor());

        if (currentBgmResourceName != nullptr) {
            if (CollectBgm::isEqualRequest(currentBgmResourceName, nullptr, mRequest))
                return al::getCaseEventNextId(this, 0);

            for (s32 i = 0; i < CollectBgm::cSituationNameListSize; i++) {
                const char* situationName = CollectBgm::cSituationNameList[i];

                if (al::isStartedBgmSituation(getActor(), situationName) &&
                    CollectBgm::isEqualRequest(currentBgmResourceName, situationName, mRequest)) {
                    return al::getCaseEventNextId(this, 0);
                }
            }

            return al::getCaseEventNextId(this, 1);
        }
    }

    return al::getCaseEventNextId(this, 2);
}

void EventFlowNodeCheckPlayingCollectBgm::start() {
    end();
}
