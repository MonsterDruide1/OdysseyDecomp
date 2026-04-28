#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

class TalkNpcSceneEventSwitcher : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_TalkNpcSceneEventSwitcher;

    TalkNpcSceneEventSwitcher();

    const char* getSceneObjName() const override;

    void requestAfterDoorSnow(s32 eventId) { mAfterDoorSnowEventId = eventId; }

    void requestVolleyBall(const char* entryName) { mVolleyBallEntryName = entryName; }

    void requestJumpingRope(const char* entryName) { mJumpingRopeEntryName = entryName; }

    void requestRadicon(const char* entryName) { mRadiconEntryName = entryName; }

    s32 getAfterDoorSnowEventId() const { return mAfterDoorSnowEventId; }

    const char* getVolleyBallEntryName() const { return mVolleyBallEntryName; }

    const char* getJumpingRopeEntryName() const { return mJumpingRopeEntryName; }

    const char* getRadiconEntryName() const { return mRadiconEntryName; }

private:
    s32 mAfterDoorSnowEventId = 0;
    s32 _c = 0;
    const char* mVolleyBallEntryName = nullptr;
    const char* mJumpingRopeEntryName = nullptr;
    const char* mRadiconEntryName = nullptr;
};

static_assert(sizeof(TalkNpcSceneEventSwitcher) == 0x28);
