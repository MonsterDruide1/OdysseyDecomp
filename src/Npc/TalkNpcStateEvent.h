#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseEventFlowData;
}

// Minimal layout decoded from TalkNpc::exeEvent / exeHackEvent /
// exeKinopioBrigadeFirstEvent / tryGetCurrentEvent: the event-flow data
// used by the NPC's talk events lives at +0x28.
class TalkNpcStateEvent {
public:
    bool trySwitchEventEntry(const char* entryName);

private:
    u8 _0[0x28];

public:
    al::IUseEventFlowData* mEventFlowData;
};
